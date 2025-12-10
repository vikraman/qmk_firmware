// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Dispatcher/compatibility layer for Navigator trackpad
// Includes the appropriate implementation based on build configuration

#include "navigator_trackpad_common.h"

#ifdef PRECISION_TRACKPAD_ENABLE
    #include "navigator_trackpad_ptp.h"
#else
    #include "navigator_trackpad_mouse.h"
#endif
