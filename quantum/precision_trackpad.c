// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "precision_trackpad.h"
#include "timer.h"

// Include the driver implementation
#if defined(PRECISION_TRACKPAD_DRIVER_NAVIGATOR_TRACKPAD)
#    include "drivers/sensors/navigator_trackpad_ptp.h"
#endif

// Driver selection (resolved at compile time by build system)
const precision_trackpad_driver_t *precision_trackpad_driver =
    &PRECISION_TRACKPAD_DRIVER(PRECISION_TRACKPAD_DRIVER_NAME);

void precision_trackpad_init(void) {
    if (precision_trackpad_driver && precision_trackpad_driver->init) {
        precision_trackpad_driver->init();
    }
}

bool precision_trackpad_task(void) {
    // No throttling - let the driver control timing
    // PTP mode needs responsive updates for gesture recognition
    if (precision_trackpad_driver && precision_trackpad_driver->task) {
        return precision_trackpad_driver->task();
    }
    return false;
}

void precision_trackpad_set_cpi(uint16_t cpi) {
    if (precision_trackpad_driver && precision_trackpad_driver->set_cpi) {
        precision_trackpad_driver->set_cpi(cpi);
    }
}

uint16_t precision_trackpad_get_cpi(void) {
    if (precision_trackpad_driver && precision_trackpad_driver->get_cpi) {
        return precision_trackpad_driver->get_cpi();
    }
    return 0;
}
