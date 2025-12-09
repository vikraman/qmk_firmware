// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "precision_trackpad.h"
#include "timer.h"

// Weak functions that can be overridden by keyboard-specific implementations
__attribute__((weak)) void precision_trackpad_init_kb(void) {}
__attribute__((weak)) bool precision_trackpad_task_kb(void) {
    return false;
}

void precision_trackpad_init(void) {
    precision_trackpad_init_kb();
}

bool precision_trackpad_task(void) {
    static uint32_t last_update = 0;

    // Throttle updates to 125Hz (8ms) to avoid overwhelming USB
    if (timer_elapsed32(last_update) < 8) {
        return false;
    }

    last_update = timer_read32();
    return precision_trackpad_task_kb();
}
