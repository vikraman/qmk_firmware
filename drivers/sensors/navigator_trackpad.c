// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// THIS IS A WORK IN PROGRESS, AS THE TRACKPAD IC's FIRMWARE IS STILL IN DEVELOPMENT
// DO NOT USE THIS CODE IN PRODUCTION

#include <stdint.h>
#include <sys/types.h>
#include <math.h>
#include "navigator_trackpad.h"
#include "i2c_master.h"
#include "quantum.h"
#include "timer.h"


#ifdef PROTOCOL_LUFA
# error "LUFA is not supported yet"
#endif

const pointing_device_driver_t navigator_trackpad_pointing_device_driver = {.init = navigator_trackpad_device_init, .get_report = navigator_trackpad_get_report, .get_cpi = navigator_trackpad_get_cpi, .set_cpi = navigator_trackpad_set_cpi};

deferred_token callback_token = 0;
uint16_t    current_cpi = DEFAULT_CPI_TICK;
uint32_t    gpio_offset_addr;
uint8_t     has_motion = 0;
extern bool set_scrolling;
bool        trackpad_init;

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
cgen6_report_t      ptp_report;
trackpad_gesture_t  gesture = {0};
#endif

// Helper functions to parse ptp reports
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
uint8_t finger_count(cgen6_report_t *report) {
    uint8_t fingers = 0;
    if (report->fingers[0].x != 0 || report->fingers[0].y != 0) {
        fingers++;
    }
    if (report->fingers[1].x != 0 || report->fingers[1].y != 0) {
        fingers++;
    }

    return fingers;
}
#endif

i2c_status_t cirque_gen6_read_report(uint8_t *data, uint16_t cnt) {
    i2c_status_t res = i2c_receive(NAVIGATOR_TRACKPAD_ADDRESS, data, cnt, NAVIGATOR_TRACKPAD_TIMEOUT);
    if (res != I2C_STATUS_SUCCESS) {
        trackpad_init = false;
        return res;
    }
    wait_us(cnt * 15);
    return res;
}

void cirque_gen6_clear(void) {
    uint8_t buf[CGEN6_MAX_PACKET_SIZE];
    for (uint8_t i = 0; i < 5; i++) {
        wait_ms(1);
        if (cirque_gen6_read_report(buf, CGEN6_MAX_PACKET_SIZE) != I2C_STATUS_SUCCESS) {
            break;
        }
    }
}

uint8_t cirque_gen6_read_memory(uint32_t addr, uint8_t *data, uint16_t cnt, bool fast_read) {
    uint8_t  cksum = 0;
    uint8_t  res   = CGEN6_SUCCESS;
    uint8_t  len[2];
    uint16_t read = 0;

    uint8_t preamble[8] = {0x01, 0x09, (uint8_t)(addr & (uint32_t)0x000000FF), (uint8_t)((addr & 0x0000FF00) >> 8), (uint8_t)((addr & 0x00FF0000) >> 16), (uint8_t)((addr & 0xFF000000) >> 24), (uint8_t)(cnt & 0x00FF), (uint8_t)((cnt & 0xFF00) >> 8)};

    // Read the length of the data + 3 bytes (first 2 bytes for the length and the last byte for the checksum)
    // Create a buffer to store the data
    uint8_t buf[cnt + 3];
    if (i2c_transmit_and_receive(NAVIGATOR_TRACKPAD_ADDRESS, preamble, 8, buf, cnt + 3, NAVIGATOR_TRACKPAD_TIMEOUT) != I2C_STATUS_SUCCESS) {
        res |= CGEN6_I2C_FAILED;
        trackpad_init = false;
    }

    // Read the data length
    for (uint8_t i = 0; i < 2; i++) {
        cksum += len[i] = buf[i];
        read++;
    }

    // Populate the data buffer
    for (uint16_t i = 2; i < cnt + 2; i++) {
        cksum += data[i - 2] = buf[i];
        read++;
    }

    if (!fast_read) {
        // Check the checksum
        if (cksum != buf[read]) {
            res |= CGEN6_CKSUM_FAILED;
        }

        // Check the length (incremented first to account for the checksum)
        if (++read != (len[0] | (len[1] << 8))) {
            res |= CGEN6_LEN_MISMATCH;
        }

        wait_ms(1);
    } else {
        wait_us(250);
    }

    return res;
}

uint8_t cirque_gen6_write_memory(uint32_t addr, uint8_t *data, uint16_t cnt) {
    uint8_t res   = CGEN6_SUCCESS;
    uint8_t cksum = 0, i = 0;
    uint8_t preamble[8] = {0x00, 0x09, (uint8_t)(addr & 0x000000FF), (uint8_t)((addr & 0x0000FF00) >> 8), (uint8_t)((addr & 0x00FF0000) >> 16), (uint8_t)((addr & 0xFF000000) >> 24), (uint8_t)(cnt & 0x00FF), (uint8_t)((cnt & 0xFF00) >> 8)};

    uint8_t buf[cnt + 9];
    // Calculate the checksum
    for (; i < 8; i++) {
        cksum += buf[i] = preamble[i];
    }

    for (i = 0; i < cnt; i++) {
        cksum += buf[i + 8] = data[i];
    }

    buf[cnt + 8] = cksum;

    if (i2c_transmit(NAVIGATOR_TRACKPAD_ADDRESS, buf, cnt + 9, NAVIGATOR_TRACKPAD_TIMEOUT) != I2C_STATUS_SUCCESS) {
        res |= CGEN6_I2C_FAILED;
        trackpad_init = false;
    }

    wait_ms(1);

    return res;
}

uint8_t cirque_gen6_read_reg(uint32_t addr, bool fast_read) {
    uint8_t data;
    uint8_t res = cirque_gen6_read_memory(addr, &data, 1, fast_read);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 8bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return data;
}

uint16_t cirque_gen6_read_reg_16(uint32_t addr) {
    uint8_t buf[2];
    uint8_t res = cirque_gen6_read_memory(addr, buf, 2, false);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 16bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return (buf[1] << 8) | buf[0];
}

uint32_t cirque_gen6_read_reg_32(uint32_t addr) {
    uint8_t buf[4];
    uint8_t res = cirque_gen6_read_memory(addr, buf, 4, false);
    if (res != CGEN6_SUCCESS) {
        printf("Failed to read 32bits from register at address 0x%08X with error 0x%02X\n", (u_int)addr, res);
        return 0;
    }
    return (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];
}

uint8_t cirque_gen6_write_reg(uint32_t addr, uint8_t data) {
    return cirque_gen6_write_memory(addr, &data, 1);
}

uint8_t cirque_gen6_write_reg_16(uint32_t addr, uint16_t data) {
    uint8_t buf[2] = {data & 0xFF, (data >> 8) & 0xFF};
    return cirque_gen6_write_memory(addr, buf, 2);
}

uint8_t cirque_gen6_write_reg_32(uint32_t addr, uint32_t data) {
    uint8_t buf[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
    return cirque_gen6_write_memory(addr, buf, 4);
}

uint8_t cirque_gen6_set_relative_mode(void) {
    uint8_t feed_config4 = cirque_gen6_read_reg(CGEN6_FEED_CONFIG4, false);
    feed_config4 &= 0xF3;
    return cirque_gen6_write_reg(CGEN6_FEED_CONFIG4, feed_config4);
}

uint8_t cirque_gen6_set_ptp_mode(void) {
    uint8_t feed_config4 = cirque_gen6_read_reg(CGEN6_FEED_CONFIG4, false);
    feed_config4 &= 0xF7;
    feed_config4 |= 0x04;
    return cirque_gen6_write_reg(CGEN6_FEED_CONFIG4, feed_config4);
}

uint8_t cirque_gen6_swap_xy(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG, false);
    if (set) {
        xy_config |= 0x04;
    } else {
        xy_config &= ~0x04;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_invert_y(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG, false);
    if (set) {
        xy_config |= 0x02;
    } else {
        xy_config &= ~0x02;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_invert_x(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG, false);
    if (set) {
        xy_config |= 0x01;
    } else {
        xy_config &= ~0x01;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

uint8_t cirque_gen6_enable_logical_scaling(bool set) {
    uint8_t xy_config = cirque_gen6_read_reg(CGEN6_XY_CONFIG, false);
    if (set) {
        xy_config &= ~0x08;
    } else {
        xy_config |= ~0x08;
    }
    return cirque_gen6_write_reg(CGEN6_XY_CONFIG, xy_config);
}

bool cirque_gen6_get_gpio_state(uint8_t num) {
    uint32_t gpio_states = cirque_gen6_read_reg_32(0x43000000 + gpio_offset_addr + 0x0004);
    return ((gpio_states >> num) & 0x000000001);
}

void cirque_gen_6_read_report(void) {
    uint8_t packet[CGEN6_MAX_PACKET_SIZE];
    if (cirque_gen6_read_report(packet, CGEN6_MAX_PACKET_SIZE) != I2C_STATUS_SUCCESS) {
        return;
    }

    uint8_t report_id = packet[2];
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    if (report_id == CGEN6_PTP_REPORT_ID) {
        ptp_report.fingers[0].id            = (packet[3] & 0xFC) >> 2;
        ptp_report.fingers[0].confidence    = packet[3] & 0x01;
        ptp_report.fingers[0].tip           = (packet[3] & 0x02) >> 1;
        ptp_report.fingers[0].x  = packet[5] << 8 | packet[4];
        ptp_report.fingers[0].y  = packet[7] << 8 | packet[6];
        ptp_report.fingers[1].id            = (packet[8] & 0xFC) >> 2;
        ptp_report.fingers[1].confidence    = packet[8] & 0x01;
        ptp_report.fingers[1].tip           = (packet[8] & 0x02) >> 1;
        ptp_report.fingers[1].x  = packet[10] << 8 | packet[9];
        ptp_report.fingers[1].y  = packet[12] << 8 | packet[11];
        ptp_report.ts       = packet[14] << 8 | packet[13];
        ptp_report.contact_count = packet[15];
        ptp_report.buttons       = packet[16];
    }
#endif
#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    if (report_id == CGEN6_MOUSE_REPORT_ID) {
        ptp_report.buttons           = packet[3];
        ptp_report.xDelta            = packet[4];
        ptp_report.yDelta            = packet[5];
        ptp_report.scrollDelta = packet[6];
        ptp_report.panDelta          = packet[7];

        has_motion = 1;
    }
#endif
}

void dump_ptp_report(void) {
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    printf("PTP Report:\n");
    printf(" ID finger 1: %d\n", ptp_report.fingers[0].id);
    printf(" Confidence finger 1: %d\n", ptp_report.fingers[0].confidence);
    printf(" Tip finger 1: %d\n", ptp_report.fingers[0].tip);
    printf(" X finger 1: %d\n", ptp_report.fingers[0].x);
    printf(" Y finger 1: %d\n", ptp_report.fingers[0].y);
    printf(" ID finger 2: %d\n", ptp_report.fingers[1].id);
    printf(" Confidence finger 2: %d\n", ptp_report.fingers[1].confidence);
    printf(" Tip finger 2: %d\n", ptp_report.fingers[1].tip);
    printf(" X finger 2: %d\n", ptp_report.fingers[1].x);
    printf(" Y finger 2: %d\n", ptp_report.fingers[1].y);
    printf(" Timestamp: %d\n", ptp_report.ts);
    printf(" Contact Count: %d\n", ptp_report.contact_count);
    printf(" Buttons: %d\n", ptp_report.buttons);
    printf(" Fingers: %d\n", finger_count(&ptp_report));
#endif
}

// Check if the DR pin is asserted, if it is there is motion data to sample.
uint8_t cirque_gen6_has_motion(void) {
    return cirque_gen6_read_reg(CGEN6_I2C_DR, true);
}

uint32_t cirque_gen6_read_callback(uint32_t trigger_time, void *cb_arg) {
    if (!trackpad_init) {
        navigator_trackpad_device_init();
        return NAVIGATOR_TRACKPAD_PROBE;
    }
    if (cirque_gen6_has_motion()) {
        has_motion = 1;
        cirque_gen_6_read_report();
    }
    return NAVIGATOR_TRACKPAD_READ;
}

void navigator_trackpad_device_init(void) {
    i2c_init();
    i2c_status_t status = i2c_ping_address(NAVIGATOR_TRACKPAD_ADDRESS, NAVIGATOR_TRACKPAD_TIMEOUT);
    if (status != I2C_STATUS_SUCCESS) {
        trackpad_init = false;
        return;
    }
    cirque_gen6_clear();
    wait_ms(50);
#if defined(NAVIGATOR_TRACKPAD_DEBUG)
    uint8_t  hardwareId  = cirque_gen6_read_reg(CGEN6_HARDWARE_ID, false);
    uint8_t  firmwareId  = cirque_gen6_read_reg(CGEN6_FIRMWARE_ID, false);
    uint16_t vendorId    = cirque_gen6_read_reg_16(CGEN6_VENDOR_ID);
    uint16_t productId   = cirque_gen6_read_reg_16(CGEN6_PRODUCT_ID);
    uint16_t versionId   = cirque_gen6_read_reg_16(CGEN6_FIRMWARE_REV);
    uint32_t firmwareRev = cirque_gen6_read_reg_32(CGEN6_FIRMWARE_REV);

    printf("Touchpad Hardware ID: 0x%02X\n", hardwareId);
    printf("Touchpad Firmware ID: 0x%02X\n", firmwareId);
    printf("Touchpad Vendor ID: 0x%04X\n", vendorId);
    printf("Touchpad Product ID: 0x%04X\n", productId);
    printf("Touchpad Version ID: 0x%04X\n", versionId);

    uint32_t revision           = firmwareRev & 0x00ffffff;
    bool     uncommittedVersion = firmwareRev & 0x80000000;
    bool     branchVersion      = firmwareRev & 0x40000000;
    uint8_t  developerId        = firmwareRev & 0x3f000000;

    printf("Touchpad Firmware Revision: 0x%08X\n", (u_int)revision);
    printf("Touchpad Uncommitted Version: %s\n", uncommittedVersion ? "true" : "false");
    printf("Touchpad Branch Version: %s\n", branchVersion ? "true" : "false");
    printf("Touchpad Developer ID: %d\n", developerId);
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    uint8_t res = cirque_gen6_set_ptp_mode();
#endif
#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    uint8_t res = cirque_gen6_set_relative_mode();
#endif

    if (res != CGEN6_SUCCESS) {
        return;
    }

    // Reset to the default alignment
    cirque_gen6_swap_xy(false);
    cirque_gen6_invert_x(false);
    cirque_gen6_invert_y(false);
    cirque_gen6_swap_xy(true);
    cirque_gen6_invert_x(true);
    cirque_gen6_invert_y(true);
    cirque_gen6_enable_logical_scaling(true);

    trackpad_init = true;
    // Only register the callback for the first time
    if (!callback_token) {
        callback_token = defer_exec(NAVIGATOR_TRACKPAD_READ, cirque_gen6_read_callback, NULL);
    }
}

report_mouse_t navigator_trackpad_get_report(report_mouse_t mouse_report) {
#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    // Handle pending click release from previous cycle
    if (gesture.pending_click) {
        gesture.pending_click = false;
        mouse_report.buttons  = 0;
        return mouse_report;
    }
#endif

    if (!has_motion || !trackpad_init) {
        return mouse_report;
    }

#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
    mouse_report.x       = ptp_report.xDelta;
    mouse_report.y       = ptp_report.yDelta;
    mouse_report.v       = ptp_report.scrollDelta;
    mouse_report.h       = ptp_report.panDelta;
    mouse_report.buttons = ptp_report.buttons;
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
    uint8_t fingers     = finger_count(&ptp_report);
    bool    is_touching = ptp_report.fingers[0].tip;
    bool    was_idle    = (gesture.state == TP_IDLE);

    // Handle finger down - record start position (regardless of current state)
    if (is_touching && was_idle) {
        gesture.touch_start_time = timer_read();
        gesture.prev_x           = ptp_report.fingers[0].x;
        gesture.prev_y           = ptp_report.fingers[0].y;
        gesture.settled_x        = 0;  // Will be set after settle time
        gesture.settled_y        = 0;
        gesture.settled          = false;
        gesture.max_finger_count = fingers;
        gesture.state            = TP_MOVING;
    }

    // Handle finger up - evaluate tap at lift time (libinput style)
    if (!is_touching && !was_idle) {
        uint16_t duration = timer_elapsed(gesture.touch_start_time);

        // Calculate distance from settled position (or treat as no movement if never settled)
        int32_t dist_sq = 0;
        if (gesture.settled) {
            int16_t dx = gesture.prev_x - gesture.settled_x;
            int16_t dy = gesture.prev_y - gesture.settled_y;
            dist_sq    = (int32_t)dx * dx + (int32_t)dy * dy;
        }

#    ifdef NAVIGATOR_TRACKPAD_GESTURE_DEBUG
        printf("LIFT: state=%d, fingers=%d, dist_sq=%ld, duration=%dms, settled=%d\n",
               gesture.state, gesture.max_finger_count, (long)dist_sq, duration, gesture.settled);
#    endif

        // Check tap conditions: short duration AND small movement from settled position
        bool is_tap = (duration <= NAVIGATOR_TRACKPAD_TAP_TIMEOUT) &&
                      (dist_sq <= NAVIGATOR_TRACKPAD_TAP_MOVE_THRESHOLD);

        if (is_tap) {
#    ifdef NAVIGATOR_TRACKPAD_ENABLE_DOUBLE_TAP
            if (gesture.max_finger_count >= 2) {
#        ifdef NAVIGATOR_TRACKPAD_GESTURE_DEBUG
                printf("  -> RIGHT CLICK\n");
#        endif
                mouse_report.x        = 0;
                mouse_report.y        = 0;
                mouse_report.buttons  = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON2);
                gesture.pending_click = true;
            } else
#    endif
#    ifdef NAVIGATOR_TRACKPAD_ENABLE_TAP
            if (gesture.max_finger_count == 1) {
#        ifdef NAVIGATOR_TRACKPAD_GESTURE_DEBUG
                printf("  -> LEFT CLICK\n");
#        endif
                mouse_report.x        = 0;
                mouse_report.y        = 0;
                mouse_report.buttons  = pointing_device_handle_buttons(mouse_report.buttons, true, POINTING_DEVICE_BUTTON1);
                gesture.pending_click = true;
            }
#    endif
        }
#    ifdef NAVIGATOR_TRACKPAD_GESTURE_DEBUG
        else {
            printf("  -> NO TAP (duration=%d/%d, dist=%ld/%d)\n",
                   duration, NAVIGATOR_TRACKPAD_TAP_TIMEOUT,
                   (long)dist_sq, NAVIGATOR_TRACKPAD_TAP_MOVE_THRESHOLD);
        }
#    endif

        gesture.state = TP_IDLE;
        set_scrolling = false;
    }

    // Handle ongoing touch - movement and scrolling
    if (is_touching && !was_idle) {
        // Track max fingers during gesture
        if (fingers > gesture.max_finger_count) {
            gesture.max_finger_count = fingers;
        }

        // Determine mode based on finger count
        if (fingers >= 2 && gesture.state != TP_SCROLLING) {
            gesture.state = TP_SCROLLING;
        }

        uint16_t duration = timer_elapsed(gesture.touch_start_time);

        // Record settled position once settle time elapses
        if (!gesture.settled && duration >= NAVIGATOR_TRACKPAD_TAP_SETTLE_TIME) {
            gesture.settled   = true;
            gesture.settled_x = ptp_report.fingers[0].x;
            gesture.settled_y = ptp_report.fingers[0].y;
        }

        // Check if we should suppress movement (might still be a tap)
        int32_t dist_sq = 0;
        if (gesture.settled) {
            int16_t dx = ptp_report.fingers[0].x - gesture.settled_x;
            int16_t dy = ptp_report.fingers[0].y - gesture.settled_y;
            dist_sq    = (int32_t)dx * dx + (int32_t)dy * dy;
        }

        // Only report movement if: timeout exceeded OR moved beyond tap threshold (after settling)
        bool should_move = (duration > NAVIGATOR_TRACKPAD_TAP_TIMEOUT) ||
                           (gesture.settled && dist_sq > NAVIGATOR_TRACKPAD_TAP_MOVE_THRESHOLD);

        if (should_move) {
            int16_t delta_x = ptp_report.fingers[0].x - gesture.prev_x;
            int16_t delta_y = ptp_report.fingers[0].y - gesture.prev_y;

            if (delta_x != 0 || delta_y != 0) {
                if (gesture.state == TP_SCROLLING) {
                    // Two-finger scroll: output directly to h/v for high-res scrolling
                    // With high-res scrolling enabled, the OS divides by 120 to get ticks
                    // Apply multiplier to adjust scroll speed
                    int16_t scroll_x = delta_x * NAVIGATOR_TRACKPAD_SCROLL_MULTIPLIER;
                    int16_t scroll_y = delta_y * NAVIGATOR_TRACKPAD_SCROLL_MULTIPLIER;

                    // Clamp to int8_t range for the report
                    scroll_x = (scroll_x > 127) ? 127 : ((scroll_x < -127) ? -127 : scroll_x);
                    scroll_y = (scroll_y > 127) ? 127 : ((scroll_y < -127) ? -127 : scroll_y);

#    ifdef NAVIGATOR_TRACKPAD_GESTURE_DEBUG
                    if (scroll_x != 0 || scroll_y != 0) {
                        printf("SCROLL: delta=%d,%d *%d -> h=%d v=%d\n",
                               delta_x, delta_y, NAVIGATOR_TRACKPAD_SCROLL_MULTIPLIER, -scroll_x, scroll_y);
                    }
#    endif

                    mouse_report.h = -scroll_x;  // Invert for natural scrolling
                    mouse_report.v = scroll_y;
                    mouse_report.x = 0;
                    mouse_report.y = 0;
                } else {
                    // One-finger movement: mouse cursor
                    mouse_report.x = (delta_x < 0) ? -powf(-delta_x, 1.2) : powf(delta_x, 1.2);
                    mouse_report.y = (delta_y < 0) ? -powf(-delta_y, 1.2) : powf(delta_y, 1.2);
                }
            }
        }

        gesture.prev_x = ptp_report.fingers[0].x;
        gesture.prev_y = ptp_report.fingers[0].y;
    }
#endif

    has_motion = 0;
    return mouse_report;
}

void set_cirque_cpi(void) {
    // traverse the sequence by compairing the cpi_x value with the current cpi_x value
    // set the cpi to the next value in the sequence
    switch (current_cpi) {
        case CPI_1: {
            current_cpi = CPI_2;
            break;
        }
        case CPI_2: {
            current_cpi = CPI_3;
            break;
        }
        case CPI_3: {
            current_cpi = CPI_4;
            break;
        }
        case CPI_4: {
            current_cpi = CPI_5;
            break;
        }
        case CPI_5: {
            current_cpi = CPI_6;
            break;
        }
        case CPI_6: {
            current_cpi = CPI_7;
            break;
        }
        case CPI_7: {
            current_cpi = CPI_1;
            break;
        }
        default: {
            current_cpi = CPI_4;
            break;
        }
    }
}

uint16_t navigator_trackpad_get_cpi(void) {
    return current_cpi;
}

void restore_cpi(uint8_t cpi) {
    current_cpi = cpi;
    set_cirque_cpi();
}

void navigator_trackpad_set_cpi(uint16_t cpi) {
    if (cpi == 0) { // Decrease one tick
        if (current_cpi > 1) {
            current_cpi--;
        }
    } else {
        if (current_cpi < CPI_TICKS) {
            current_cpi++;
        }
    }
    set_cirque_cpi();
};
