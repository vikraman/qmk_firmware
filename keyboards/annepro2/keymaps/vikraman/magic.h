#pragma once

#include "quantum.h"

// offset past custom_keycodes in keymap.c
enum {
    MAGIC = SAFE_RANGE + 50,
};

// call for every keystroke, updates context buffer
void magic_update_buffer(uint16_t keycode, keyrecord_t *record);

// call when Magic key is pressed
void magic_expand(keyrecord_t *record);
