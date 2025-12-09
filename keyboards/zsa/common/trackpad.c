// Copyright 2024 ZSA Technology Labs
// SPDX-License-Identifier: GPL-2.0-or-later

#include "precision_trackpad.h"
#include "drivers/sensors/navigator_trackpad.h"

void precision_trackpad_init_kb(void) {
    // Initialize the navigator trackpad sensor
    navigator_trackpad_device_init();
}

bool precision_trackpad_task_kb(void) {
    // Call the PTP task to process sensor data and send reports
    navigator_trackpad_task();
    return true;  // Activity occurred
}
