#include QMK_KEYBOARD_H

// Fallthrough.
#define ____ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE LAYER */
    [0] = LAYOUT(
        KC_PSCR, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,     KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_GRAVE,
        KC_BSPC, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,     KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_ENT,
        KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,     KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_RSFT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_ESC,  MO(1),   KC_SPC,   KC_SPC,  MO(1),   KC_ESC,  KC_RALT, KC_RGUI, KC_RCTL,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   QK_BOOT,  TG(2),   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO
    ),
    /* FN LAYER */
    [1] = LAYOUT(
        KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        KC_COPY,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    ____,
        KC_PASTE,   KC_BSLS, KC_EQL,  KC_LBRC, KC_LPRN, KC_RIGHT, KC_LEFT, KC_RPRN, KC_RBRC, KC_MINS, KC_SLSH, ____,
        ____,       ____,    ____,    KC_DOWN, KC_UP,   ____,     ____,    ____,    ____,    ____,    ____,    ____,
        ____,       ____,    ____,    ____,    ____,    ____,     ____,    ____,    ____,    ____,    ____,    ____,
        ____,       ____,    ____,    ____,    ____,    ____,     ____,    ____,    ____,    ____,    ____,    ____
    ),
    /* WASD LAYER */
    [2] = LAYOUT(
        KC_PSCR, KC_1,  KC_2,    KC_3,   KC_4,  KC_5,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TAB,  KC_T,  KC_Q,    KC_W,   KC_E,  KC_R,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_BSPC, KC_G,  KC_A,    KC_S,   KC_D,  KC_F,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LSFT, KC_B,  KC_Z,    KC_X,   KC_C,  KC_V,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LCTL, KC_NO, KC_LALT, KC_ESC, KC_NO, KC_SPC,    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_NO,   KC_O,  KC_NO,   KC_NO,  KC_NO, QK_REBOOT, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    )
};

#if defined(ENCODER_MAP_ENABLE)

const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    /* BASE LAYER */
    [0] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    /* FN LAYER */
    [1] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
    /* WASD LAYER */
    [2] = { ENCODER_CCW_CW(KC_MS_WH_UP, KC_MS_WH_DOWN), ENCODER_CCW_CW(KC_MS_WH_DOWN, KC_MS_WH_UP) },
};

#endif

#ifdef OLED_ENABLE

// Keys pressed during the last frame.
bool pressed_keys[MATRIX_ROWS][MATRIX_COLS];

// Initial Logo Y position for startup animation.
uint8_t logo_y = 15;

// Typing animation toggle flag.
bool flame_big = false;

// Last oled update.
uint32_t last_update = 0;

/// Check if a new key was pressed.
static bool new_key_down(void) {
    bool new_key_down = false;
    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        for (uint8_t y = 0; y < MATRIX_COLS; y++) {
            bool key_pressed = (matrix_get_row(x) & (1 << y)) > 0;
            new_key_down |= pressed_keys[x][y] != key_pressed;
            pressed_keys[x][y] = key_pressed;
        }
    }
    return new_key_down;
}

/// Render our logo.
static void render_logo(uint8_t wpm, bool new_key) {
    // Default Alacritty logo.
    static const char PROGMEM alacritty_logo[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0,
        0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xfc, 0xff, 0xff, 0x7f, 0x1f,
        0x1f, 0x7f, 0xff, 0xff, 0xfc, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfc, 0xff, 0xff, 0x7f, 0x0f, 0x83, 0xe0, 0xfc,
        0xfc, 0xe0, 0x83, 0x0f, 0x7f, 0xff, 0xff, 0xfc, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xc0, 0xf0, 0xfe, 0xff, 0xff, 0x3f, 0x07, 0x01, 0x00, 0x00, 0x07, 0x3f, 0xff,
        0xff, 0x3f, 0x07, 0x00, 0x00, 0x01, 0x0f, 0x3f, 0xff, 0xff, 0xfe, 0xf0, 0xc0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
        0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
        // Write extra zeroes to clear the screen during startup animation.
        // This is necessary because `oled_clear` introduces tearing.
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };

    // Animation frame for Alacritty logo while typing.
    static const char PROGMEM alacritty_logo_flame_big[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xe0,
        0xe0, 0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0xfc, 0xff, 0xff, 0x7f, 0x1f,
        0x1f, 0x7f, 0xff, 0xff, 0xfc, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xfc, 0xff, 0xff, 0x7f, 0x0f, 0x83, 0xe0, 0xfc,
        0xfc, 0xe0, 0x83, 0x0f, 0x7f, 0xff, 0xff, 0xfc, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xc0, 0xf0, 0xfe, 0xff, 0xff, 0x3f, 0x07, 0x01, 0x00, 0x00, 0x0f, 0xff, 0xff,
        0xff, 0xff, 0x0f, 0x00, 0x00, 0x01, 0x0f, 0x3f, 0xff, 0xff, 0xfe, 0xf0, 0xc0, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f,
        0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
    };

    oled_set_cursor(0, logo_y);

    // Draw Alacritty logo according to current status.
    if (wpm == 0) {
        // Reset animation when WPM hits zero.
        oled_write_raw_P(alacritty_logo, sizeof(alacritty_logo));
        flame_big = false;
    } else {
        // Toggle between big and small flame with every key press.
        if (new_key) {
            flame_big = !flame_big;
        }

        if (flame_big) {
            oled_write_raw_P(alacritty_logo_flame_big, sizeof(alacritty_logo_flame_big));
        } else {
            oled_write_raw_P(alacritty_logo, sizeof(alacritty_logo));
        }
    }

    // Update startup animation.
    if (logo_y > 0) {
        logo_y -= 1;
    }
}

/// Draw text information.
static void render_text(uint8_t wpm) {
    oled_set_cursor(0, 14);
    oled_write_ln_P(PSTR(" WPM "), false);

    oled_set_cursor(1, 15);
    oled_write(get_u8_str(wpm, '0'), false);
}

/// One-time OLED setup.
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    return OLED_ROTATION_270;
}

/// Check if display should be turned off
bool should_timeout(uint8_t wpm, bool new_key) {
    // Use master side to control timeouts using `SPLIT_OLED_ENABLE`.
    if (!is_keyboard_master()) {
        return false;
    }

    uint32_t now = timer_read();

    // Reset timer while typing.
    if (new_key || wpm != 0) {
        last_update = now;
    }

    // Timeout when typing was suspended for at least `OLED_TIMEOUT`.
    return wpm == 0 && now - last_update >= OLED_TIMEOUT;
}

/// Main entry point for OLED control.
bool oled_task_user(void) {
    uint8_t wpm = get_current_wpm();
    bool new_key = new_key_down();

    if (should_timeout(wpm, new_key)) {
        oled_off();
        return false;
    }

    render_logo(wpm, new_key);

    // Render text once animation is done.
    if (logo_y == 0) {
        render_text(wpm);
    }

    oled_render();

    return false;
}

#endif
