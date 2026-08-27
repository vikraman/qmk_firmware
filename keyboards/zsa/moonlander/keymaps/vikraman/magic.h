#pragma once

#include "quantum.h"

#ifndef ZSA_SAFE_RANGE
#    define ZSA_SAFE_RANGE SAFE_RANGE
#endif

enum {
    MAGIC = ZSA_SAFE_RANGE + 50,
};

// call for every keystroke, updates context buffer
void magic_update_buffer(uint16_t keycode, keyrecord_t *record);

// call when Magic key is pressed
void magic_expand(keyrecord_t *record);
