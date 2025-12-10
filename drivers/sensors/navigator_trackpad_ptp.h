// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "navigator_trackpad_common.h"
#include "precision_trackpad_drivers.h"
#include "report.h"

// Trackpad sensitivity multiplier (1.0 = native, >1.0 = more sensitive)
// Higher values make cursor/gestures move faster
// Add to your config.h to customize:
// #define NAVIGATOR_TRACKPAD_SENSITIVITY 1.5f
#ifndef NAVIGATOR_TRACKPAD_SENSITIVITY
#    define NAVIGATOR_TRACKPAD_SENSITIVITY 1.0f
#endif

#ifdef PRECISION_TRACKPAD_ENABLE
// Driver registration
extern const precision_trackpad_driver_t navigator_trackpad_precision_trackpad_driver;
#ifndef NAVIGATOR_TRACKPAD_PTP_MODE
#error "NAVIGATOR_TRACKPAD_PTP_MODE must be defined when using the precision trackpad driver"
#endif
#endif

#ifndef NAVIGATOR_TRACKPAD_SENSITIVITY
#    define NAVIGATOR_TRACKPAD_SENSITIVITY 1.3f
#endif
