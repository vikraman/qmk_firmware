// Copyright 2024 ZSA Technology Labs
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include "quantum.h"

#ifdef PRECISION_TRACKPAD_ENABLE

// Initialize the trackpad
void trackpad_init(void);

// Update trackpad state and send reports
void trackpad_task(void);

#endif // PRECISION_TRACKPAD_ENABLE
