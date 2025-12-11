// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// PTP (Precision Touchpad) mode implementation for Navigator trackpad
// Converts Cirque Gen 6 sensor data to Windows Precision Touchpad HID reports

#include "navigator_trackpad_ptp.h"
#include "navigator_trackpad_common.h"
#include "precision_trackpad_drivers.h"
#include "quantum.h"
#include "report.h"
#include "timer.h"

#ifdef PRECISION_TRACKPAD_ENABLE

// External declaration for report sending
extern void send_trackpad(report_digitizer_t *report);

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)

// Build a finger's 6 bytes into the report buffer
// Format: [conf:1 + tip:1 + pad:6] [contact_id:3 + pad:5] [X_lo] [X_hi] [Y_lo] [Y_hi]
// For PTP, we always send contact_id and coordinates. Only tip changes.
static void build_finger_bytes(uint8_t *buf, uint8_t contact_id, uint16_t x, uint16_t y, bool tip) {
    buf[0] = tip ? 0x03 : 0x01;  // confidence=1 always, tip varies
    buf[1] = contact_id & 0x07;  // contact_id in bits 0-2
    buf[2] = x & 0xFF;           // X low byte
    buf[3] = (x >> 8) & 0xFF;    // X high byte
    buf[4] = y & 0xFF;           // Y low byte
    buf[5] = (y >> 8) & 0xFF;    // Y high byte
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

    // Current finger states
    bool finger0_tip = sensor_report.fingers[0].tip;
    bool finger1_tip = sensor_report.fingers[1].tip;

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
                           sensor_report.fingers[0].x,
                           sensor_report.fingers[0].y,
                           finger0_tip);
    }

    // Bytes 7-12: Finger 1 (include if touching or lifting off)
    if (finger1_contact) {
        build_finger_bytes(&report[7], 1,
                           sensor_report.fingers[1].x,
                           sensor_report.fingers[1].y,
                           finger1_tip);
    }

    // Bytes 13-14: Scan time (little-endian)
    report[13] = sensor_report.scan_time & 0xFF;
    report[14] = (sensor_report.scan_time >> 8) & 0xFF;

    // Byte 15: Contact count (bits 0-3) + buttons (bits 4-6)
    report[15] = (contact_count & 0x0F) | ((buttons & 0x01) << 4);

    // Send report if any contacts (including lift-offs) or button changed
    if (contact_count > 0 || button_changed) {
        send_trackpad((report_digitizer_t *)report);
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
