// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// PTP (Precision Touchpad) mode implementation for Navigator trackpad
// Converts Cirque Gen 6 sensor data to Windows Precision Touchpad HID reports

#include "navigator_trackpad_ptp.h"
#include "navigator_trackpad_common.h"
#include "precision_trackpad_drivers.h"
#include "quantum.h"
#include "report.h"

#ifdef PRECISION_TRACKPAD_ENABLE

// External declaration for PTP report sending
extern void send_trackpad(report_trackpad_t *report);

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)

// PTP task function - converts trackpad touches to PTP reports
static bool navigator_trackpad_ptp_task(void) {
    if (!has_motion || !trackpad_init) {
        return false;
    }

    // Create local snapshot to avoid race condition with callback
    cgen6_report_t local_report = ptp_report;

    uint8_t raw_fingers = cirque_gen6_finger_count(&local_report);

    // Initialize PTP report
    report_trackpad_t ptp = {0};
    ptp.report_id = REPORT_ID_TRACKPAD;

    // Sensitivity scaling: Track previous RAW sensor position and accumulated scaled position
    // Apply sensitivity only to the delta from raw sensor, then accumulate
    static uint16_t prev_raw_x[2] = {0, 0};
    static uint16_t prev_raw_y[2] = {0, 0};
    static uint16_t accum_x[2] = {0, 0};
    static uint16_t accum_y[2] = {0, 0};
    static bool     was_touching[2] = {false, false};

    if (local_report.fingers[0].tip) {
        uint16_t raw_x = local_report.fingers[0].x;
        uint16_t raw_y = local_report.fingers[0].y;

        if (was_touching[0]) {
            // Calculate delta from RAW sensor data
            int16_t delta_x = (int16_t)(raw_x - prev_raw_x[0]);
            int16_t delta_y = (int16_t)(raw_y - prev_raw_y[0]);

            // Apply sensitivity to the delta
            delta_x = (int16_t)((float)delta_x * NAVIGATOR_TRACKPAD_SENSITIVITY);
            delta_y = (int16_t)((float)delta_y * NAVIGATOR_TRACKPAD_SENSITIVITY);

            // Add scaled delta to accumulated position
            int32_t new_x = (int32_t)accum_x[0] + delta_x;
            int32_t new_y = (int32_t)accum_y[0] + delta_y;

            // Clamp to valid range
            if (new_x < 0) new_x = 0;
            if (new_x > 4095) new_x = 4095;
            if (new_y < 0) new_y = 0;
            if (new_y > 4095) new_y = 4095;

            accum_x[0] = (uint16_t)new_x;
            accum_y[0] = (uint16_t)new_y;
        } else {
            // First touch - initialize accumulated position to raw position
            accum_x[0] = raw_x;
            accum_y[0] = raw_y;
        }

        ptp.contacts[0].confidence = 1;
        ptp.contacts[0].tip = 1;
        ptp.contacts[0].contact_id = 0;
        ptp.contacts[0].x = accum_x[0];
        ptp.contacts[0].y = accum_y[0];

        prev_raw_x[0] = raw_x;
        prev_raw_y[0] = raw_y;
        was_touching[0] = true;
    } else {
        was_touching[0] = false;
    }

    if (local_report.fingers[1].tip) {
        uint16_t raw_x = local_report.fingers[1].x;
        uint16_t raw_y = local_report.fingers[1].y;

        if (was_touching[1]) {
            // Calculate delta from RAW sensor data
            int16_t delta_x = (int16_t)(raw_x - prev_raw_x[1]);
            int16_t delta_y = (int16_t)(raw_y - prev_raw_y[1]);

            // Apply sensitivity to the delta
            delta_x = (int16_t)((float)delta_x * NAVIGATOR_TRACKPAD_SENSITIVITY);
            delta_y = (int16_t)((float)delta_y * NAVIGATOR_TRACKPAD_SENSITIVITY);

            // Add scaled delta to accumulated position
            int32_t new_x = (int32_t)accum_x[1] + delta_x;
            int32_t new_y = (int32_t)accum_y[1] + delta_y;

            // Clamp to valid range
            if (new_x < 0) new_x = 0;
            if (new_x > 4095) new_x = 4095;
            if (new_y < 0) new_y = 0;
            if (new_y > 4095) new_y = 4095;

            accum_x[1] = (uint16_t)new_x;
            accum_y[1] = (uint16_t)new_y;
        } else {
            // First touch - initialize accumulated position to raw position
            accum_x[1] = raw_x;
            accum_y[1] = raw_y;
        }

        ptp.contacts[1].confidence = 1;
        ptp.contacts[1].tip = 1;
        ptp.contacts[1].contact_id = 1;
        ptp.contacts[1].x = accum_x[1];
        ptp.contacts[1].y = accum_y[1];

        prev_raw_x[1] = raw_x;
        prev_raw_y[1] = raw_y;
        was_touching[1] = true;
    } else {
        was_touching[1] = false;
    }

    // Set scan time, contact count, and buttons (after contacts per Microsoft spec)
    // Use the sensor's scan_time for accurate gesture velocity calculation
    ptp.scan_time = local_report.scan_time;
    ptp.contact_count = raw_fingers;
    ptp.button1 = (local_report.buttons & 0x01) ? 1 : 0;
    ptp.button2 = (local_report.buttons & 0x02) ? 1 : 0;
    ptp.button3 = (local_report.buttons & 0x04) ? 1 : 0;

    if (raw_fingers > 0) {
        send_trackpad(&ptp);
    }

    has_motion = 0;
    return true;
}
#else
// Stub for when PTP mode is not enabled
static bool navigator_trackpad_ptp_task(void) {
    return false;
}
#endif

// Internal init function
static void navigator_trackpad_ptp_init(void) {
    navigator_trackpad_device_init();  // Common init
}

// Driver registration
const precision_trackpad_driver_t navigator_trackpad_precision_trackpad_driver = {
    .init    = navigator_trackpad_ptp_init,
    .task    = navigator_trackpad_ptp_task,
    .set_cpi = navigator_trackpad_set_cpi,
    .get_cpi = navigator_trackpad_get_cpi
};

#endif // PRECISION_TRACKPAD_ENABLE
