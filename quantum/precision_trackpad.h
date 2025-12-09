// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Initialize the precision trackpad
 *
 * Called during keyboard initialization to set up the trackpad hardware
 * and prepare it for use.
 */
void precision_trackpad_init(void);

/**
 * @brief Process trackpad input and send reports
 *
 * Called periodically from keyboard_task() to read sensor data,
 * process touches/gestures, and send HID reports to the host.
 *
 * @return true if trackpad activity occurred, false otherwise
 */
bool precision_trackpad_task(void);
