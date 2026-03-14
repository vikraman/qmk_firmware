// Copyright 2025 vikraman
// SPDX-License-Identifier: GPL-2.0-or-later

enum {
    RGBL_QWERTY,
    RGBL_GRAPHITE,
    RGBL_NUM,
    RGBL_SYMBOL,
    RGBL_NAV,
    RGBL_MOUSE,
    RGBL_EMACS,
    RGBL_LATEX,
    RGBL_GREEK,
    RGBL_ADJUST,
};

const rgblight_segment_t PROGMEM qwerty_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_WHITE}
);
const rgblight_segment_t PROGMEM graphite_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_ORANGE}
);
const rgblight_segment_t PROGMEM num_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_RED}
);
const rgblight_segment_t PROGMEM symbol_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_GOLDENROD}
);
const rgblight_segment_t PROGMEM nav_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_BLUE}
);
const rgblight_segment_t PROGMEM mouse_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_PINK}
);
const rgblight_segment_t PROGMEM emacs_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_GREEN}
);
const rgblight_segment_t PROGMEM latex_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_TEAL}
);
const rgblight_segment_t PROGMEM greek_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_PURPLE}
);
const rgblight_segment_t PROGMEM adjust_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, RGBLIGHT_LED_COUNT, HSV_YELLOW}
);

const rgblight_segment_t *const PROGMEM rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    qwerty_layer,
    graphite_layer,
    num_layer,
    symbol_layer,
    nav_layer,
    mouse_layer,
    emacs_layer,
    latex_layer,
    greek_layer,
    adjust_layer
);

static void update_layer_colors(layer_state_t default_state, layer_state_t state) {
    rgblight_set_layer_state(RGBL_QWERTY, layer_state_cmp(default_state, _QWERTY));
    rgblight_set_layer_state(RGBL_GRAPHITE, layer_state_cmp(default_state, _GRAPHITE));
    rgblight_set_layer_state(RGBL_NUM, layer_state_cmp(state, _NUM));
    rgblight_set_layer_state(RGBL_SYMBOL, layer_state_cmp(state, _SYMBOL));
    rgblight_set_layer_state(RGBL_NAV, layer_state_cmp(state, _NAV));
    rgblight_set_layer_state(RGBL_MOUSE, layer_state_cmp(state, _MOUSE));
    rgblight_set_layer_state(RGBL_EMACS, layer_state_cmp(state, _EMACS));
    rgblight_set_layer_state(RGBL_LATEX, layer_state_cmp(state, _LATEX));
    rgblight_set_layer_state(RGBL_GREEK, layer_state_cmp(state, _GREEK));
    rgblight_set_layer_state(RGBL_ADJUST, layer_state_cmp(state, _ADJUST));
}

void keyboard_post_init_user(void) {
    rgblight_layers = rgb_layers;
    update_layer_colors(default_layer_state, layer_state);
}

layer_state_t default_layer_state_set_user(layer_state_t state) {
    update_layer_colors(state, layer_state);
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _NAV, _NUM, _ADJUST);
    update_layer_colors(default_layer_state, state);
    return state;
}
