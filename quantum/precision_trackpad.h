// Copyright 2024 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "precision_trackpad_drivers.h"

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

/**
 * @brief Set the trackpad CPI (counts per inch)
 *
 * @param cpi The desired CPI value
 */
void precision_trackpad_set_cpi(uint16_t cpi);

/**
 * @brief Get the current trackpad CPI
 *
 * @return Current CPI value
 */
uint16_t precision_trackpad_get_cpi(void);

// Driver pointer (set at compile time)
extern const precision_trackpad_driver_t *precision_trackpad_driver;
