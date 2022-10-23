#include QMK_KEYBOARD_H

// Fallthrough.
#define ____ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE LAYER */
    [0] = LAYOUT(
        KC_PSCREEN, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_TAB,     KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,     KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_GRAVE,
        KC_BSPC,    KC_A,    KC_O,    KC_E,    KC_U,    KC_I,     KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_ENT,
        KC_LSFT,    KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,     KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_RSFT,
        KC_LCTL,    KC_LGUI, KC_LALT, KC_ESC,  MO(1),   KC_SPC,   KC_SPC,  MO(1),   KC_ESC,  KC_RALT, KC_RGUI, KC_RCTRL
    ),
    /* FN LAYER */
    [1] = LAYOUT(
        KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        KC_COPY,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    ____,
        KC_PASTE,   KC_BSLS, KC_EQL,  KC_LBRC, KC_LPRN, KC_RIGHT, KC_LEFT, KC_RPRN, KC_RBRC, KC_MINS, KC_SLSH, ____,
        ____,       ____,    ____,    KC_DOWN, KC_UP,   ____,     ____,    ____,    ____,    ____,    ____,    ____,
        ____,       ____,    ____,    ____,    ____,    ____,     ____,    ____,    ____,    ____,    ____,    ____
    )
};

#ifdef OLED_ENABLE

/* static void render_logo(void) { */
/*     static const char PROGMEM qmk_logo[] = { */
/*         0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94, */
/*         0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4, */
/*         0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,0 */
/*     }; */

/*     oled_write_P(qmk_logo, false); */
/* } */

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

bool oled_task_user(void) {
    uint8_t wpm = get_current_wpm();
    oled_write_ln_P(PSTR("WPM:"), false);
    oled_write(get_u8_str(wpm, '0'), false);

    return false;
}

#endif
