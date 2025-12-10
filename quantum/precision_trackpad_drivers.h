// Copyright 2025 ZSA Technology Labs, Inc <@zsa>
// Copyright 2025 Florian Didron <fdidron@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#include <stdint.h>
#include <stdbool.h>

// Precision trackpad driver interface
typedef struct {
    void (*init)(void);
    bool (*task)(void);
    void (*set_cpi)(uint16_t);
    uint16_t (*get_cpi)(void);
} precision_trackpad_driver_t;

// Driver registration macros (mirror pointing_device pattern)
#define PRECISION_TRACKPAD_DRIVER_CONCAT(name) name##_precision_trackpad_driver
#define PRECISION_TRACKPAD_DRIVER(name) PRECISION_TRACKPAD_DRIVER_CONCAT(name)
