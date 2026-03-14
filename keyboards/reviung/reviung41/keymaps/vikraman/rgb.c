// Copyright 2025 vikraman
// SPDX-License-Identifier: GPL-2.0-or-later

typedef struct {
    uint8_t rgb_layer;
    uint8_t qmk_layer;
} rgb_layer_binding_t;

typedef struct {
    uint8_t qmk_layer;
    uint8_t rgblight_mode;
} default_anim_binding_t;

#define RGB_LAYER_CONFIG(X)                      \
    X(NUM, _NUM, HSV_ORANGE, num_layer)          \
    X(SYMBOL, _SYMBOL, HSV_YELLOW, symbol_layer) \
    X(NAV, _NAV, HSV_CYAN, nav_layer)            \
    X(MOUSE, _MOUSE, HSV_BLUE, mouse_layer)      \
    X(EMACS, _EMACS, HSV_GREEN, emacs_layer)     \
    X(LATEX, _LATEX, HSV_MAGENTA, latex_layer)   \
    X(GREEK, _GREEK, HSV_PURPLE, greek_layer)    \
    X(ADJUST, _ADJUST, HSV_WHITE, adjust_layer)

#define DEFAULT_ANIMATION_CONFIG(X)          \
    X(_GRAPHITE, RGBLIGHT_MODE_RAINBOW_MOOD) \
    X(_QWERTY, RGBLIGHT_MODE_RAINBOW_SWIRL)

#define RGB_LAYER_ENUM(id, qmk_layer, hsv_color, layer_name) RGBL_##id,
enum { RGB_LAYER_CONFIG(RGB_LAYER_ENUM) };
#undef RGB_LAYER_ENUM

enum {
    RGB_INDICATOR_START    = 0,
    RGB_INDICATOR_COUNT    = RGBLIGHT_LED_COUNT,
    RGB_SWITCH_BLINK_MS    = 120,
    RGB_SWITCH_BLINK_TIMES = 3,
    RGB_DEFAULT_VAL_25PCT  = 64,
    BOOT_BLINK_MS          = 80,
    BOOT_BLINK_TIMES       = 3,
};

static uint8_t  restore_layer   = RGBLIGHT_MAX_LAYERS;
static uint16_t restore_time    = 0;
static bool     restore_pending = false;

#define RGB_LAYER_SEGMENT(id, qmk_layer, hsv_color, layer_name) \
    const rgblight_segment_t PROGMEM layer_name[] =             \
        RGBLIGHT_LAYER_SEGMENTS({RGB_INDICATOR_START, RGB_INDICATOR_COUNT, hsv_color});
RGB_LAYER_CONFIG(RGB_LAYER_SEGMENT)
#undef RGB_LAYER_SEGMENT

const rgblight_segment_t *const PROGMEM rgb_layers[] = {
#define RGB_LAYER_POINTER(id, qmk_layer, hsv_color, layer_name) layer_name,
    RGB_LAYER_CONFIG(RGB_LAYER_POINTER)
#undef RGB_LAYER_POINTER
        NULL,
};

static const rgb_layer_binding_t rgb_layer_bindings[] = {
#define RGB_LAYER_BINDING(id, qmk_layer, hsv_color, layer_name) {RGBL_##id, qmk_layer},
    RGB_LAYER_CONFIG(RGB_LAYER_BINDING)
#undef RGB_LAYER_BINDING
};

static const default_anim_binding_t default_anim_bindings[] = {
#define DEFAULT_ANIM_BINDING(qmk_layer, mode) {qmk_layer, mode},
    DEFAULT_ANIMATION_CONFIG(DEFAULT_ANIM_BINDING)
#undef DEFAULT_ANIM_BINDING
};

#define RGB_LAYER_COUNT ((uint8_t)(sizeof(rgb_layer_bindings) / sizeof(rgb_layer_bindings[0])))
#define DEFAULT_ANIM_COUNT ((uint8_t)(sizeof(default_anim_bindings) / sizeof(default_anim_bindings[0])))

static void set_default_animation(layer_state_t default_state) {
    uint8_t mode = RGBLIGHT_MODE_RAINBOW_SWIRL;

    for (uint8_t i = 0; i < DEFAULT_ANIM_COUNT; i++) {
        if (layer_state_cmp(default_state, default_anim_bindings[i].qmk_layer)) {
            mode = default_anim_bindings[i].rgblight_mode;
            break;
        }
    }

    rgblight_mode_noeeprom(mode);

    // Keep brightness fixed at 25% regardless of EEPROM-stored value.
    rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), RGB_DEFAULT_VAL_25PCT);
}

static uint8_t get_active_indicator_layer(layer_state_t state) {
    for (int8_t i = (int8_t)RGB_LAYER_COUNT - 1; i >= 0; i--) {
        if (layer_state_cmp(state, rgb_layer_bindings[i].qmk_layer)) {
            return rgb_layer_bindings[i].rgb_layer;
        }
    }
    return RGBLIGHT_MAX_LAYERS;
}

static void set_indicator_layers(layer_state_t state) {
    for (uint8_t i = 0; i < RGB_LAYER_COUNT; i++) {
        rgblight_set_layer_state(rgb_layer_bindings[i].rgb_layer,
                                 layer_state_cmp(state, rgb_layer_bindings[i].qmk_layer));
    }
}

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
    set_default_animation(default_layer_state);
    set_indicator_layers(layer_state);
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    set_default_animation(state);
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    static uint8_t previous_active_layer = RGBLIGHT_MAX_LAYERS;

    state = update_tri_layer_state(state, _NAV, _NUM, _ADJUST);
    set_indicator_layers(state);

    uint8_t active_layer = get_active_indicator_layer(state);
    if (active_layer != previous_active_layer) {
        if (active_layer < RGBLIGHT_MAX_LAYERS) {
            rgblight_blink_layer_repeat(active_layer, RGB_SWITCH_BLINK_MS, RGB_SWITCH_BLINK_TIMES);
            restore_layer   = active_layer;
            restore_time    = timer_read() + (RGB_SWITCH_BLINK_MS * RGB_SWITCH_BLINK_TIMES * 2) + 10;
            restore_pending = true;
        }
        previous_active_layer = active_layer;
    }

    return state;
}

void housekeeping_task_user(void) {
    if (!restore_pending) {
        return;
    }

    if (timer_expired(timer_read(), restore_time)) {
        // Re-assert the active layer color after blink helper completes.
        set_indicator_layers(layer_state);
        if (restore_layer < RGBLIGHT_MAX_LAYERS) {
            rgblight_set_layer_state(restore_layer, true);
        }
        rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), RGB_DEFAULT_VAL_25PCT);
        restore_pending = false;
    }
}

bool shutdown_user(bool jump_to_bootloader) {
    if (!jump_to_bootloader) {
        return true;
    }

    // Disable all layer overlays so boot indication is pure red.
    for (uint8_t i = 0; i < RGBLIGHT_MAX_LAYERS; i++) {
        rgblight_set_layer_state(i, false);
    }

    rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_LIGHT);
    rgblight_sethsv_noeeprom(HSV_RED);

    for (uint8_t i = 0; i < BOOT_BLINK_TIMES; i++) {
        rgblight_enable_noeeprom();
        wait_ms(BOOT_BLINK_MS);
        rgblight_disable_noeeprom();
        wait_ms(BOOT_BLINK_MS);
    }

    rgblight_enable_noeeprom();
    rgblight_sethsv_noeeprom(HSV_RED);
    wait_ms(BOOT_BLINK_MS);

    return true;
}
