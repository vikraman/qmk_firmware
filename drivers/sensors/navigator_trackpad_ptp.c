// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// PTP (Precision Touchpad) mode implementation for Navigator trackpad
// Converts Cirque Gen 6 sensor data to Windows Precision Touchpad HID reports
// Also provides a fallback mouse collection for systems that don't support PTP

#include "navigator_trackpad_ptp.h"
#include "navigator_trackpad_common.h"
#include "precision_trackpad_drivers.h"
#include "quantum.h"
#include "report.h"
#include "timer.h"

#ifdef PRECISION_TRACKPAD_ENABLE

// External declaration for report sending (used for both PTP and mouse reports)
extern void send_trackpad(report_digitizer_t *report);

// Input mode: 0 = Mouse, 3 = PTP
// Defined in usb_main.c
extern uint8_t get_trackpad_input_mode(void);

#define TRACKPAD_INPUT_MODE_MOUSE 0
#define TRACKPAD_INPUT_MODE_PTP   3

// Fallback mouse configuration
#ifndef TRACKPAD_MOUSE_SENSITIVITY
#    define TRACKPAD_MOUSE_SENSITIVITY 1.0f
#endif

// Tap-to-click configuration
#ifndef TRACKPAD_TAP_TERM_MS
#    define TRACKPAD_TAP_TERM_MS 150  // Maximum duration for a tap (ms)
#endif

#ifndef TRACKPAD_TAP_MOVE_THRESHOLD
#    define TRACKPAD_TAP_MOVE_THRESHOLD 30  // Maximum movement during tap (in sensor units, ~1.5% of range)
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)

// Build a finger's 6 bytes into the report buffer
// Format: [conf:1 + tip:1 + pad:6] [contact_id:3 + pad:5] [X_lo] [X_hi] [Y_lo] [Y_hi]
static void build_finger_bytes(uint8_t *buf, uint8_t contact_id, uint16_t x, uint16_t y, bool tip, bool confidence) {
    buf[0] = (confidence ? 0x01 : 0x00) | (tip ? 0x02 : 0x00);
    buf[1] = contact_id & 0x07;  // contact_id in bits 0-2
    buf[2] = x & 0xFF;           // X low byte
    buf[3] = (x >> 8) & 0xFF;    // X high byte
    buf[4] = y & 0xFF;           // Y low byte
    buf[5] = (y >> 8) & 0xFF;    // Y high byte
}

// Fallback mouse state
static struct {
    // Position tracking for relative movement
    bool     tracking;
    uint16_t last_x;
    uint16_t last_y;
    // Tap detection
    bool     tap_pending;
    uint32_t touch_start_time;
    uint16_t touch_start_x;
    uint16_t touch_start_y;
    // Click state
    bool     click_active;
    uint32_t click_release_time;
    // Previous state for change detection
    uint8_t  prev_buttons;
} mouse_state = {0};

// Send fallback mouse report
static void send_mouse_report(int8_t dx, int8_t dy, uint8_t buttons) {
    report_trackpad_mouse_t report = {
        .report_id = TRACKPAD_MOUSE_REPORT_ID,
        .buttons   = buttons,
        .x         = dx,
        .y         = dy
    };
    // Use the same endpoint as PTP - different report ID distinguishes it
    send_trackpad((report_digitizer_t *)&report);
}

// Clamp value to int8_t range
static inline int8_t clamp_to_int8(int32_t value) {
    if (value > 127) return 127;
    if (value < -127) return -127;
    return (int8_t)value;
}

// Minimum time to hold tap-click before releasing (ms)
#ifndef TRACKPAD_TAP_CLICK_HOLD_MS
#    define TRACKPAD_TAP_CLICK_HOLD_MS 20
#endif

// Process fallback mouse movement and tap-to-click
static void process_fallback_mouse(cgen6_report_t *sensor_report, bool finger_down, bool prev_finger_down) {
    uint32_t now = timer_read32();
    int8_t dx = 0;
    int8_t dy = 0;
    uint8_t buttons = 0;

    // Handle physical button (from sensor)
    if (sensor_report->buttons & 0x01) {
        buttons |= 0x01;
    }

    // Handle finger down transition (start tracking)
    if (finger_down && !prev_finger_down) {
        mouse_state.tracking = true;
        mouse_state.last_x = sensor_report->fingers[0].x;
        mouse_state.last_y = sensor_report->fingers[0].y;
        // Start tap detection
        mouse_state.tap_pending = true;
        mouse_state.touch_start_time = now;
        mouse_state.touch_start_x = sensor_report->fingers[0].x;
        mouse_state.touch_start_y = sensor_report->fingers[0].y;
    }

    // Handle finger movement (compute delta)
    if (finger_down && mouse_state.tracking) {
        int32_t raw_dx = (int32_t)sensor_report->fingers[0].x - (int32_t)mouse_state.last_x;
        int32_t raw_dy = (int32_t)sensor_report->fingers[0].y - (int32_t)mouse_state.last_y;

        // Check if movement exceeded tap threshold BEFORE applying sensitivity
        // This uses distance from initial touch point
        if (mouse_state.tap_pending) {
            int32_t move_x = (int32_t)sensor_report->fingers[0].x - (int32_t)mouse_state.touch_start_x;
            int32_t move_y = (int32_t)sensor_report->fingers[0].y - (int32_t)mouse_state.touch_start_y;
            int32_t move_dist_sq = move_x * move_x + move_y * move_y;
            if (move_dist_sq > TRACKPAD_TAP_MOVE_THRESHOLD * TRACKPAD_TAP_MOVE_THRESHOLD) {
                mouse_state.tap_pending = false;
            }
            // Also invalidate if any single-frame movement is significant
            if (raw_dx * raw_dx + raw_dy * raw_dy > 16) {  // ~4 units of movement in one frame
                mouse_state.tap_pending = false;
            }
        }

        // Apply sensitivity scaling
        raw_dx = (int32_t)(raw_dx * TRACKPAD_MOUSE_SENSITIVITY);
        raw_dy = (int32_t)(raw_dy * TRACKPAD_MOUSE_SENSITIVITY);

        dx = clamp_to_int8(raw_dx);
        dy = clamp_to_int8(raw_dy);

        mouse_state.last_x = sensor_report->fingers[0].x;
        mouse_state.last_y = sensor_report->fingers[0].y;
    }

    // Handle finger up transition (end tracking, check for tap)
    if (!finger_down && prev_finger_down) {
        mouse_state.tracking = false;

        // Check if this was a tap
        if (mouse_state.tap_pending) {
            uint32_t touch_duration = timer_elapsed32(mouse_state.touch_start_time);
            if (touch_duration <= TRACKPAD_TAP_TERM_MS) {
                // Valid tap - generate click
                mouse_state.click_active = true;
                mouse_state.click_release_time = now;
            }
        }
        mouse_state.tap_pending = false;
    }

    // Handle tap-generated click (button press then release after hold time)
    if (mouse_state.click_active) {
        buttons |= 0x01;
        // Release after holding for TRACKPAD_TAP_CLICK_HOLD_MS
        if (timer_elapsed32(mouse_state.click_release_time) >= TRACKPAD_TAP_CLICK_HOLD_MS) {
            mouse_state.click_active = false;
        }
    }

    // Only send report if there's actual movement or button state changed
    bool buttons_changed = (buttons != mouse_state.prev_buttons);
    bool has_movement = (dx != 0 || dy != 0);

    if (has_movement || buttons_changed) {
        send_mouse_report(dx, dy, buttons);
        mouse_state.prev_buttons = buttons;
    }
}

// Sensor coordinate range (measured empirically)
#define SENSOR_X_MIN 281
#define SENSOR_X_MAX 2018
#define SENSOR_Y_MIN 276
#define SENSOR_Y_MAX 2018

// Scale sensor coordinates to logical range (0 - TRACKPAD_LOGICAL_MAX)
static uint16_t scale_coordinate(uint16_t raw, uint16_t sensor_min, uint16_t sensor_max) {
    // Clamp to sensor range
    if (raw < sensor_min) raw = sensor_min;
    if (raw > sensor_max) raw = sensor_max;
    // Scale to logical range
    return (uint32_t)(raw - sensor_min) * TRACKPAD_LOGICAL_MAX / (sensor_max - sensor_min);
}

// PTP task function - synchronous polling with timer-based throttling
static bool navigator_trackpad_ptp_task(void) {
    static uint32_t last_poll_time  = 0;
    static uint32_t last_probe_time = 0;
    static uint8_t  prev_buttons = 0;
    // Track previous finger state to detect lift-offs
    static bool     prev_finger0_tip = false;
    static bool     prev_finger1_tip = false;

    uint32_t now = timer_read32();

    // Throttle polling to NAVIGATOR_TRACKPAD_POLL_INTERVAL_MS
    if (timer_elapsed32(last_poll_time) < NAVIGATOR_TRACKPAD_POLL_INTERVAL_MS) {
        return false;
    }
    last_poll_time = now;

    // Handle disconnected/uninitialized state with slower probe interval
    if (!trackpad_init) {
        if (timer_elapsed32(last_probe_time) < NAVIGATOR_TRACKPAD_PROBE_INTERVAL_MS) {
            return false;
        }
        last_probe_time = now;
        navigator_trackpad_device_init();
        return false;
    }

    // Read the report data into local struct
    cgen6_report_t sensor_report = {0};
    if (!cirque_gen_6_read_report(&sensor_report)) {
        return false;
    }

    // Current finger states - require both tip AND confidence from sensor
    bool finger0_tip = sensor_report.fingers[0].tip && sensor_report.fingers[0].confidence;
    bool finger1_tip = sensor_report.fingers[1].tip && sensor_report.fingers[1].confidence;

    // Determine if each finger should be included in contact_count
    // Include finger if: currently touching OR was touching last frame (lift-off)
    bool finger0_contact = finger0_tip || prev_finger0_tip;
    bool finger1_contact = finger1_tip || prev_finger1_tip;

    uint8_t buttons = sensor_report.buttons & 0x01;
    bool button_changed = (buttons != prev_buttons);

    // Contact count includes fingers that are touching OR lifting off this frame
    uint8_t contact_count = (finger0_contact ? 1 : 0) + (finger1_contact ? 1 : 0);

    // Build report from sensor data using explicit byte manipulation
    // Report format (16 bytes): [report_id] [finger0: 6 bytes] [finger1: 6 bytes] [scan_time: 2 bytes] [count+buttons: 1 byte]
    uint8_t report[16] = {0};

    // Byte 0: Report ID
    report[0] = 0x01;

    // Bytes 1-6: Finger 0 (include if touching or lifting off)
    if (finger0_contact) {
        build_finger_bytes(&report[1], 0,
                           scale_coordinate(sensor_report.fingers[0].x, SENSOR_X_MIN, SENSOR_X_MAX),
                           scale_coordinate(sensor_report.fingers[0].y, SENSOR_Y_MIN, SENSOR_Y_MAX),
                           finger0_tip,
                           sensor_report.fingers[0].confidence);
    }

    // Bytes 7-12: Finger 1 (include if touching or lifting off)
    if (finger1_contact) {
        build_finger_bytes(&report[7], 1,
                           scale_coordinate(sensor_report.fingers[1].x, SENSOR_X_MIN, SENSOR_X_MAX),
                           scale_coordinate(sensor_report.fingers[1].y, SENSOR_Y_MIN, SENSOR_Y_MAX),
                           finger1_tip,
                           sensor_report.fingers[1].confidence);
    }

    // Bytes 13-14: Scan time
    report[13] = sensor_report.scan_time & 0xFF;
    report[14] = (sensor_report.scan_time >> 8) & 0xFF;

    // Byte 15: Contact count (bits 0-3) + buttons (bits 4-6)
    report[15] = (contact_count & 0x0F) | ((buttons & 0x01) << 4);

    // Get current input mode
    uint8_t input_mode = get_trackpad_input_mode();

    // Send PTP report only in PTP mode (mode 3)
    if (input_mode == TRACKPAD_INPUT_MODE_PTP) {
        if (contact_count > 0 || button_changed) {
            send_trackpad((report_digitizer_t *)report);
        }
    }

    // Process fallback mouse only in mouse mode (mode 0)
    if (input_mode == TRACKPAD_INPUT_MODE_MOUSE) {
        process_fallback_mouse(&sensor_report, finger0_tip, prev_finger0_tip);
    }

    // Update previous state
    prev_finger0_tip = finger0_tip;
    prev_finger1_tip = finger1_tip;
    prev_buttons = buttons;

    return contact_count > 0 || button_changed;
}
#else
// Stub for when PTP mode is not enabled
static bool navigator_trackpad_ptp_task(void) {
    return false;
}
#endif

// Internal init function
static void navigator_trackpad_ptp_init(void) {
    navigator_trackpad_device_init();
}

// Driver registration
const precision_trackpad_driver_t navigator_trackpad_precision_trackpad_driver = {
    .init    = navigator_trackpad_ptp_init,
    .task    = navigator_trackpad_ptp_task,
    .set_cpi = navigator_trackpad_set_cpi,
    .get_cpi = navigator_trackpad_get_cpi
};

#endif // PRECISION_TRACKPAD_ENABLE
