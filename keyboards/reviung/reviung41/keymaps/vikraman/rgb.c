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

typedef struct {
    uint8_t mod_combo;
} rgb_mod_binding_t;

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

#define MOD_LAYER_CONFIG(X)                         \
    X(MOD_C, 0x01, HSV_RED, mod_c_layer)            \
    X(MOD_S, 0x02, HSV_BLUE, mod_s_layer)           \
    X(MOD_CS, 0x03, HSV_MAGENTA, mod_cs_layer)      \
    X(MOD_A, 0x04, HSV_GREEN, mod_a_layer)          \
    X(MOD_CA, 0x05, HSV_YELLOW, mod_ca_layer)       \
    X(MOD_SA, 0x06, HSV_CYAN, mod_sa_layer)         \
    X(MOD_CSA, 0x07, HSV_PINK, mod_csa_layer)       \
    X(MOD_G, 0x08, HSV_ORANGE, mod_g_layer)         \
    X(MOD_CG, 0x09, HSV_PURPLE, mod_cg_layer)       \
    X(MOD_SG, 0x0A, HSV_AZURE, mod_sg_layer)        \
    X(MOD_CSG, 0x0B, HSV_GOLDENROD, mod_csg_layer)  \
    X(MOD_AG, 0x0C, HSV_SPRINGGREEN, mod_ag_layer)  \
    X(MOD_CAG, 0x0D, HSV_CHARTREUSE, mod_cag_layer) \
    X(MOD_SAG, 0x0E, HSV_TEAL, mod_sag_layer)       \
    X(MOD_CSAG, 0x0F, HSV_WHITE, mod_csag_layer)

#define RGB_LAYER_ENUM(id, qmk_layer, hsv_color, layer_name) RGBL_##id,
enum { RGB_LAYER_CONFIG(RGB_LAYER_ENUM) };
#undef RGB_LAYER_ENUM

enum {
    RGBL_MOD_BASE = RGBL_ADJUST + 1,
};

enum {
    RGB_INDICATOR_START    = (RGBLIGHT_LED_COUNT / 2) - 2,
    RGB_INDICATOR_COUNT    = 4,
    RGB_SWITCH_BLINK_MS    = 80,
    RGB_SWITCH_BLINK_TIMES = 5,
    RGB_DEFAULT_BRIGHTNESS = 64,
    BOOT_BLINK_MS          = 80,
    BOOT_BLINK_TIMES       = 5,
};

static uint8_t  restore_layer      = RGBLIGHT_MAX_LAYERS;
static uint16_t restore_time       = 0;
static bool     restore_pending    = false;
static uint8_t  previous_mod_combo = 0;

#define RGB_LAYER_SEGMENT(id, qmk_layer, hsv_color, layer_name) \
    const rgblight_segment_t PROGMEM layer_name[] =             \
        RGBLIGHT_LAYER_SEGMENTS({RGB_INDICATOR_START, RGB_INDICATOR_COUNT, hsv_color});
RGB_LAYER_CONFIG(RGB_LAYER_SEGMENT)
#undef RGB_LAYER_SEGMENT

#define MOD_LAYER_SEGMENT(id, mod_combo, hsv_color, layer_name) \
    const rgblight_segment_t PROGMEM layer_name[] =             \
        RGBLIGHT_LAYER_SEGMENTS({RGB_INDICATOR_START, RGB_INDICATOR_COUNT, hsv_color});
MOD_LAYER_CONFIG(MOD_LAYER_SEGMENT)
#undef MOD_LAYER_SEGMENT

const rgblight_segment_t *const PROGMEM rgb_layers[] = {
#define RGB_LAYER_POINTER(id, qmk_layer, hsv_color, layer_name) layer_name,
    RGB_LAYER_CONFIG(RGB_LAYER_POINTER)
#undef RGB_LAYER_POINTER
#define MOD_LAYER_POINTER(id, mod_combo, hsv_color, layer_name) layer_name,
        MOD_LAYER_CONFIG(MOD_LAYER_POINTER)
#undef MOD_LAYER_POINTER
            NULL,
};

static const rgb_layer_binding_t rgb_layer_bindings[] = {
#define RGB_LAYER_BINDING(id, qmk_layer, hsv_color, layer_name) {RGBL_##id, qmk_layer},
    RGB_LAYER_CONFIG(RGB_LAYER_BINDING)
#undef RGB_LAYER_BINDING
};

static const rgb_mod_binding_t rgb_mod_bindings[] = {
#define MOD_LAYER_BINDING(id, mod_combo, hsv_color, layer_name) {mod_combo},
    MOD_LAYER_CONFIG(MOD_LAYER_BINDING)
#undef MOD_LAYER_BINDING
};

static const default_anim_binding_t default_anim_bindings[] = {
#define DEFAULT_ANIM_BINDING(qmk_layer, mode) {qmk_layer, mode},
    DEFAULT_ANIMATION_CONFIG(DEFAULT_ANIM_BINDING)
#undef DEFAULT_ANIM_BINDING
};

#define RGB_LAYER_COUNT ((uint8_t)(sizeof(rgb_layer_bindings) / sizeof(rgb_layer_bindings[0])))
#define DEFAULT_ANIM_COUNT ((uint8_t)(sizeof(default_anim_bindings) / sizeof(default_anim_bindings[0])))
#define MOD_LAYER_COUNT ((uint8_t)(sizeof(rgb_mod_bindings) / sizeof(rgb_mod_bindings[0])))

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
    rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), RGB_DEFAULT_BRIGHTNESS);
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

static uint8_t get_modifier_combo(void) {
    uint8_t mods  = get_mods() | get_oneshot_mods() | get_weak_mods();
    uint8_t combo = 0;

    if (mods & MOD_MASK_CTRL) {
        combo |= 0x01;
    }
    if (mods & MOD_MASK_SHIFT) {
        combo |= 0x02;
    }
    if (mods & MOD_MASK_ALT) {
        combo |= 0x04;
    }
    if (mods & MOD_MASK_GUI) {
        combo |= 0x08;
    }

    return combo;
}

static void set_modifier_layers(uint8_t combo) {
    for (uint8_t i = 0; i < MOD_LAYER_COUNT; i++) {
        uint8_t layer_index = RGBL_MOD_BASE + i;
        rgblight_set_layer_state(layer_index, combo != 0 && rgb_mod_bindings[i].mod_combo == combo);
    }
}

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
    set_default_animation(default_layer_state);
    set_indicator_layers(layer_state);
    previous_mod_combo = get_modifier_combo();
    set_modifier_layers(previous_mod_combo);
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    static uint8_t previous_active_layer = RGBLIGHT_MAX_LAYERS;

    state = update_tri_layer_state(state, _NAV, _NUM, _ADJUST);
    set_indicator_layers(state);
    set_modifier_layers(get_modifier_combo());

    uint8_t active_layer = get_active_indicator_layer(state);
    if (active_layer != previous_active_layer) {
        if (active_layer < RGBLIGHT_MAX_LAYERS) {
            rgblight_blink_layer_repeat(active_layer, RGB_SWITCH_BLINK_MS, RGB_SWITCH_BLINK_TIMES);
            restore_layer   = active_layer;
            restore_time    = timer_read() + (RGB_SWITCH_BLINK_MS * RGB_SWITCH_BLINK_TIMES * 2) + 10;
            restore_pending = true;
        } else {
            // Back on QWERTY/GRAPHITE: clear any pending layer blink/restore so animation fully takes over.
            rgblight_unblink_all_but_layer(RGBLIGHT_MAX_LAYERS);
            restore_layer   = RGBLIGHT_MAX_LAYERS;
            restore_pending = false;
        }
        previous_active_layer = active_layer;
    }

    return state;
}

void housekeeping_task_user(void) {
    uint8_t mod_combo = get_modifier_combo();
    if (mod_combo != previous_mod_combo) {
        set_modifier_layers(mod_combo);
        previous_mod_combo = mod_combo;
    }

    if (!restore_pending) {
        return;
    }

    if (timer_expired(timer_read(), restore_time)) {
        // Re-assert the active layer color after blink helper completes.
        set_indicator_layers(layer_state);
        set_modifier_layers(previous_mod_combo);
        if (restore_layer < RGBLIGHT_MAX_LAYERS) {
            rgblight_set_layer_state(restore_layer, true);
        }
        rgblight_sethsv_noeeprom(rgblight_get_hue(), rgblight_get_sat(), RGB_DEFAULT_BRIGHTNESS);
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
