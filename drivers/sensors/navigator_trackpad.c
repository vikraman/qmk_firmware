// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

// Dispatcher/compatibility layer for Navigator trackpad
// Provides backward compatibility for existing code

#include "navigator_trackpad.h"
#include "navigator_trackpad_common.h"

#ifdef PRECISION_TRACKPAD_ENABLE
    #include "navigator_trackpad_ptp.h"
#else
    #include "navigator_trackpad_mouse.h"
#endif
