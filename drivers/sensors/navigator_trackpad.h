// Copyright 2025 ZSA Technology Labs, Inc <contact@zsa.io>
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "report.h"
#include "pointing_device.h"

#    ifndef CIRQUE_PINNACLE_X_LOWER
#        define CIRQUE_PINNACLE_X_LOWER 127 // min "reachable" X value
#    endif
#    ifndef CIRQUE_PINNACLE_X_UPPER
#        define CIRQUE_PINNACLE_X_UPPER 1919 // max "reachable" X value
#    endif
#    ifndef CIRQUE_PINNACLE_Y_LOWER
#        define CIRQUE_PINNACLE_Y_LOWER 63 // min "reachable" Y value
#    endif
#    ifndef CIRQUE_PINNACLE_Y_UPPER
#        define CIRQUE_PINNACLE_Y_UPPER 1471 // max "reachable" Y value
#    endif
#    ifndef CIRQUE_PINNACLE_X_RANGE
#        define CIRQUE_PINNACLE_X_RANGE (CIRQUE_PINNACLE_X_UPPER - CIRQUE_PINNACLE_X_LOWER)
#    endif
#    ifndef CIRQUE_PINNACLE_Y_RANGE
#        define CIRQUE_PINNACLE_Y_RANGE (CIRQUE_PINNACLE_Y_UPPER - CIRQUE_PINNACLE_Y_LOWER)
#    endif

#define NAVIGATOR_TRACKPAD_READ 7
#define NAVIGATOR_TRACKPAD_PROBE 1000

#ifndef NAVIGATOR_TRACKPAD_TAP_MOVE_THRESHOLD
#    define NAVIGATOR_TRACKPAD_TAP_MOVE_THRESHOLD 100  // Max movement (squared) before tap becomes a drag
#endif

#ifndef NAVIGATOR_TRACKPAD_TAP_TIMEOUT
#    define NAVIGATOR_TRACKPAD_TAP_TIMEOUT 200  // Max duration (ms) for a tap
#endif

#ifndef NAVIGATOR_TRACKPAD_TAP_SETTLE_TIME
#    define NAVIGATOR_TRACKPAD_TAP_SETTLE_TIME 30  // Ignore movement during initial contact (ms)
#endif

#ifndef NAVIGATOR_TRACKPAD_MAX_DELTA
#    define NAVIGATOR_TRACKPAD_MAX_DELTA 250  // Max allowed delta per frame to prevent jumps
#endif

#ifndef NAVIGATOR_TRACKPAD_ADDRESS
#    define NAVIGATOR_TRACKPAD_ADDRESS 0x58
#endif

#ifndef NAVIGATOR_TRACKPAD_TIMEOUT
#    define NAVIGATOR_TRACKPAD_TIMEOUT 100
#endif

#define NAVIGATOR_TRACKPAD_PTP_MODE
#if !defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE) && !defined(NAVIGATOR_TRACKPAD_PTP_MODE)
#    define NAVIGATOR_TRACKPAD_PTP_MODE
#endif

#define CGEN6_MAX_PACKET_SIZE 17
#define CGEN6_PTP_REPORT_ID 0x01
#define CGEN6_MOUSE_REPORT_ID 0x06
#define CGEN6_ABSOLUTE_REPORT_ID 0x09

// C3 error codes when reading memory
#define CGEN6_SUCCESS 0x00
#define CGEN6_CKSUM_FAILED 0x01
#define CGEN6_LEN_MISMATCH 0x02
#define CGEN6_I2C_FAILED 0x03

// C3 register addresses
#define CGEN6_REG_BASE 0x20000800
#define CGEN6_HARDWARE_ID CGEN6_REG_BASE + 0x08
#define CGEN6_FIRMWARE_ID CGEN6_REG_BASE + 0x09
#define CGEN6_FIRMWARE_REV CGEN6_REG_BASE + 0x10
#define CGEN6_VENDOR_ID CGEN6_REG_BASE + 0x0A
#define CGEN6_PRODUCT_ID CGEN6_REG_BASE + 0x0C
#define CGEN6_VERSION_ID CGEN6_REG_BASE + 0x0E
#define CGEN6_FEED_CONFIG4 0x200E000B
#define CGEN6_FEED_CONFIG3 0x200E000A
#define CGEN6_SYS_CONFIG1 0x20000008
#define CGEN6_XY_CONFIG 0x20080018
#define CGEN6_SFR_BASE 0x40000008
#define CGEN6_GPIO_BASE 0x00052000
#define CGEN6_I2C_DR 0x61010000

#define CPI_TICKS 7
#define DEFAULT_CPI_TICK 4
#define CPI_1 200
#define CPI_2 400
#define CPI_3 800
#define CPI_4 1024
#define CPI_5 1400
#define CPI_6 1800
#define CPI_7 2048

#ifndef NAVIGATOR_TRACKPAD_SCROLL_DIVIDER
#    define NAVIGATOR_TRACKPAD_SCROLL_DIVIDER 10
#endif

#ifndef NAVIGATOR_TRACKPAD_SCROLL_MULTIPLIER
#    define NAVIGATOR_TRACKPAD_SCROLL_MULTIPLIER 3
#endif

// Two-finger scrolling (define to enable)
// #define NAVIGATOR_TRACKPAD_SCROLL_WITH_TWO_FINGERS

// Scroll inversion configuration
// Define these to invert scroll direction on respective axes
// #define NAVIGATOR_SCROLL_INVERT_X
// #define NAVIGATOR_SCROLL_INVERT_Y

// Scroll inertia configuration
/*
To enable, add to your config.h:
#define NAVIGATOR_TRACKPAD_SCROLL_INERTIA_ENABLE

Configurable values (all optional):
- NAVIGATOR_TRACKPAD_SCROLL_INERTIA_FRICTION - Higher = stops faster (default: 50, range 1-255)
- NAVIGATOR_TRACKPAD_SCROLL_INERTIA_INTERVAL - Time between glide reports in ms (default: 15)
- NAVIGATOR_TRACKPAD_SCROLL_INERTIA_TRIGGER - Minimum velocity to trigger glide (default: 3)
*/
#ifdef NAVIGATOR_TRACKPAD_SCROLL_INERTIA_ENABLE
#ifndef NAVIGATOR_TRACKPAD_SCROLL_INERTIA_FRICTION
#define NAVIGATOR_TRACKPAD_SCROLL_INERTIA_FRICTION 5  // Higher = stops faster (1-255)
#endif
#ifndef NAVIGATOR_TRACKPAD_SCROLL_INERTIA_INTERVAL
#define NAVIGATOR_TRACKPAD_SCROLL_INERTIA_INTERVAL 7  // Glide report interval in ms
#endif
#ifndef NAVIGATOR_TRACKPAD_SCROLL_INERTIA_TRIGGER
#define NAVIGATOR_TRACKPAD_SCROLL_INERTIA_TRIGGER 1    // Min velocity to trigger glide
#endif

typedef struct {
    int16_t  vx;           // Current X velocity (Q8 fixed point)
    int16_t  vy;           // Current Y velocity (Q8 fixed point)
    int16_t  smooth_vx;    // Smoothed X velocity (Q8 fixed point)
    int16_t  smooth_vy;    // Smoothed Y velocity (Q8 fixed point)
    uint16_t timer;        // Timer for interval tracking
    bool     active;       // Is glide currently active
} scroll_inertia_t;
#endif

#if defined(NAVIGATOR_TRACKPAD_PTP_MODE)
#    ifndef MOUSE_EXTENDED_REPORT
#        define MOUSE_EXTENDED_REPORT
#    endif
typedef struct {
    uint8_t  id;
    uint8_t  confidence;
    uint8_t  tip;
    uint16_t x;
    uint16_t y;
} cgen6_finger_t;

typedef struct {
    cgen6_finger_t fingers[2];
    uint16_t ts;
    uint8_t  contact_count;
    uint8_t  buttons;
} cgen6_report_t;

// Trackpad gesture state machine
typedef enum {
    TP_IDLE,        // No fingers touching
    TP_MOVING,      // One finger movement = mouse cursor
    TP_SCROLLING,   // Two finger movement = scroll
} trackpad_state_t;

typedef struct {
    trackpad_state_t state;
    uint16_t         touch_start_time;  // When finger first touched
    uint16_t         settled_x;         // Position after settle time (for tap threshold)
    uint16_t         settled_y;
    uint16_t         prev_x;            // Previous position (for delta calculation)
    uint16_t         prev_y;
    uint8_t          max_finger_count;  // Max fingers seen during this gesture
    bool             settled;           // Has the settle time elapsed?
    bool             pending_click;     // Need to send a click release next cycle
    uint16_t         last_scroll_end;   // Time when last scroll gesture ended
} trackpad_gesture_t;
#endif

#if defined(NAVIGATOR_TRACKPAD_ABSOLUTE_MODE)
typedef struct {
    uint16_t x;
    uint16_t y;
    uint8_t  palm;
    uint8_t  z;
} finger_data_t;

typedef struct {
    finger_data_t fingers[3]; // Cirque support 5 fingers, we only need 3 for our application
    uint8_t       contact_flags;
    uint8_t       buttons;
} cgen6_report_t;
#endif

#if defined(NAVIGATOR_TRACKPAD_RELATIVE_MODE)
typedef struct {
    uint8_t buttons;
    int8_t  xDelta;
    int8_t  yDelta;
    int8_t  scrollDelta;
    int8_t  panDelta;
} cgen6_report_t;
#endif

const pointing_device_driver_t navigator_trackpad_pointing_device_driver;
void           navigator_trackpad_device_init(void);
report_mouse_t navigator_trackpad_get_report(report_mouse_t mouse_report);
uint16_t       navigator_trackpad_get_cpi(void);
void           navigator_trackpad_set_cpi(uint16_t cpi);
void           restore_cpi(uint8_t cpi);
