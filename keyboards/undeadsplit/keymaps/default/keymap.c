#include QMK_KEYBOARD_H

#include "transactions.h"
#include "analog.h"
#include "print.h"

// Fallthrough.
#define ____ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* BASE LAYER */
    [0] = LAYOUT(
        KC_PSCR, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_DEL,
        KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,     KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_GRAVE,
        KC_BSPC, KC_A,    KC_O,    KC_E,    KC_U,    KC_I,     KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_ENT,
        KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,     KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_RSFT,
        KC_LCTL, KC_LGUI, KC_LALT, KC_ESC,  MO(1),   KC_SPC,   KC_SPC,  MO(1),   KC_ESC,  KC_RALT, KC_RGUI, KC_RCTL
    ),
    /* FN LAYER */
    [1] = LAYOUT(
        KC_F1,      KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,    KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        KC_COPY,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    DF(2),
        KC_PASTE,   KC_BSLS, KC_EQL,  KC_LBRC, KC_LPRN, KC_RIGHT, KC_LEFT, KC_RPRN, KC_RBRC, KC_MINS, KC_SLSH, ____,
        ____,       ____,    ____,    KC_DOWN, KC_UP,   ____,     ____,    ____,    ____,    ____,    ____,    ____,
        ____,       ____,    ____,    ____,    ____,    ____,     ____,    ____,    ____,    ____,    ____,    ____
    ),
    /* WASD LAYER */
    [2] = LAYOUT(
        KC_PSCR, KC_1,  KC_2,    KC_3,   KC_4,  KC_5,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_TAB,  KC_T,  KC_Q,    KC_W,   KC_E,  KC_R,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, DF(0),
        KC_BSPC, KC_G,  KC_A,    KC_S,   KC_D,  KC_F,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LSFT, KC_B,  KC_Z,    KC_X,   KC_C,  KC_V,      KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
        KC_LCTL, KC_NO, KC_LALT, KC_ESC, KC_NO, KC_SPC,    KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO
    )
};

#if defined(SPLIT_TRANSACTION_IDS_USER)

#define MAX_SCROLL_INTERVAL_MS_MASTER 50
#define MIN_SCROLL_INTERVAL_MS_MASTER 5
#define MAX_SCROLL_INTERVAL_MS_SLAVE 200
#define MIN_SCROLL_INTERVAL_MS_SLAVE 20

#define PIN_ANALOG_BUTTON B7
#define PIN_ANALOG_X B5
#define PIN_ANALOG_Y B6

#define OUTER_DEADZONE 50
#define DEADZONE 50

typedef struct _analog_bounds {
    int16_t min_y;
    int16_t rst_y;
    int16_t max_y;
} analog_bounds;

typedef struct _analog_stick_pins {
    int16_t x;
    int16_t y;
} analog_stick_pins;

// Initailize limits for calibration.
//
// These values are initialized slightly closer to `rst_y` than any observed
// values. This way the first scroll is only slightly inaccurate, while still
// having access to the full range of motion.
static analog_bounds analog_bounds_master = { 300, 0, 700 };
static analog_bounds analog_bounds_slave = { 300, 0, 700 };

static uint16_t last_scroll = 0;

static bool is_master = false;

// Read analog stick pin values from slave.
analog_stick_pins stick_pins_slave(void) {
    // Read pins from slave through RPC.
    analog_stick_pins pins = { 0, 0 };
    bool success = transaction_rpc_recv(KEYBOARD_SYNC_A, sizeof(pins), &pins);

    // Report as zero position on failure.
    if (!success) {
        analog_stick_pins fallback_pins = { 0, analog_bounds_slave.rst_y };
        return fallback_pins;
    }

    // Ensure slave analog stick deadpoint is initialized.
    if (analog_bounds_slave.rst_y == 0) {
        analog_bounds_slave.rst_y = pins.y;
    }

    // Update limits on success.
    if (success && pins.y != 0) {
        if (pins.y > analog_bounds_slave.max_y) {
            analog_bounds_slave.max_y = pins.y;
        } else if (pins.y < analog_bounds_slave.min_y) {
            analog_bounds_slave.min_y = pins.y;
        }
    }

    return pins;
}

// Read analog stick pin values from slave.
analog_stick_pins stick_pins_master(void) {
    // Read master analog stick pins.
    analog_stick_pins pins = { analogReadPin(PIN_ANALOG_X), analogReadPin(PIN_ANALOG_Y) };

    // Update limits.
    if (pins.y > analog_bounds_master.max_y) {
        analog_bounds_master.max_y = pins.y;
    } else if (pins.y < analog_bounds_master.min_y) {
        analog_bounds_master.min_y = pins.y;
    }

    return pins;
}

// Get analog stick Y position as inclusive range from -100 to 100.
int8_t analog_percentage(bool master) {
    // Initialize pins and variables.
    analog_stick_pins pins;
    analog_bounds bounds;
    if (master) {
        pins = stick_pins_master();
        bounds = analog_bounds_master;
    } else {
        pins = stick_pins_slave();
        bounds = analog_bounds_slave;
    }

    if (pins.y < bounds.rst_y - DEADZONE) {
        // Get distance from deadzone.
        int16_t delta = bounds.rst_y - DEADZONE - pins.y;

        // Convert distance to percentage.
        int16_t max_delta = bounds.rst_y - DEADZONE - OUTER_DEADZONE - bounds.min_y;
        delta = delta > max_delta ? max_delta : delta;
        int16_t percentage = delta * 100 / max_delta;

        return -1 * (int8_t)percentage;
    } else if (pins.y > bounds.rst_y + DEADZONE) {
        // Get distance from deadzone.
        int16_t delta = pins.y - bounds.rst_y - DEADZONE;

        // Convert distance to percentage.
        int16_t max_delta = bounds.max_y - bounds.rst_y - DEADZONE - OUTER_DEADZONE;
        delta = delta > max_delta ? max_delta : delta;
        int16_t percentage = delta * 100 / max_delta;

        return (int8_t)percentage;
    } else {
        return 0;
    }
}

// Get timer interval from analog stick percentage.
uint16_t analog_interval(int8_t percentage, uint16_t min, uint16_t max) {
    // Always treat percentage as positive number.
    if (percentage < 0) {
        percentage *= -1;
    }

    // Calculate offset from minimum interval.
    uint32_t timer_range = (uint32_t)(max - min);
    uint32_t offset = (uint32_t)percentage * timer_range / 100;

    // Calculate interval
    return max - (uint16_t)offset;
}

void housekeeping_task_user(void) {
    // Enter bootloader on joystick press.
    if (!readPin(PIN_ANALOG_BUTTON)) {
        bootloader_jump();
    }

    // Skip analog check for slave or when analog trigger is not yet possible.
    static uint32_t min_interval = (MIN_SCROLL_INTERVAL_MS_MASTER * MIN_SCROLL_INTERVAL_MS_SLAVE)
        / (MIN_SCROLL_INTERVAL_MS_MASTER + MIN_SCROLL_INTERVAL_MS_SLAVE);
    uint16_t elapsed = timer_elapsed(last_scroll);
    if (!is_master || elapsed < (uint16_t)min_interval) {
        return;
    }

    // Get analog stick positions.
    int8_t percentage_master = analog_percentage(true);
    int8_t percentage_slave = analog_percentage(false);

    // Ignore analog sticks if neither is moved.
    if (percentage_master == 0 && percentage_slave == 0) {
        return;
    }

    // Ignore analog sticks if they oppose each other.
    if ((percentage_master > 0 && percentage_slave < 0)
        || (percentage_master < 0 && percentage_slave > 0))
    {
        return;
    }

    // Convert analog stick percentage to event emission interval.
    uint32_t interval_master = (uint32_t)analog_interval(
            percentage_master,
            MIN_SCROLL_INTERVAL_MS_MASTER,
            MAX_SCROLL_INTERVAL_MS_MASTER
    );
    uint32_t interval_slave = (uint32_t)analog_interval(
            percentage_slave,
            MIN_SCROLL_INTERVAL_MS_SLAVE,
            MAX_SCROLL_INTERVAL_MS_SLAVE
    );

    // Calculate combined interval frequency.
    uint32_t interval = (interval_master * interval_slave) / (interval_master + interval_slave);

    // Send buttons if interval duration has elapsed.
    if (elapsed >= interval) {
        last_scroll = timer_read();

        if (percentage_master > 0 || percentage_slave > 0) {
            register_code(KC_UP);
            unregister_code(KC_UP);
        } else {
            register_code(KC_DOWN);
            unregister_code(KC_DOWN);
        }
    }
}

// Send analog stick pin value.
void analog_sync_handler(uint8_t in_buflen, const void* in_data, uint8_t out_buflen, void* out_data) {
    analog_stick_pins *pins = (analog_stick_pins*)out_data;
    pins->x = analogReadPin(PIN_ANALOG_X);
    pins->y = analogReadPin(PIN_ANALOG_Y);
}

void keyboard_post_init_user(void) {
    is_master = is_keyboard_master();

    // Initialize analog stick button.
    setPinInputHigh(PIN_ANALOG_BUTTON);

    // Initialize master analog stick deadpoint.
    if (is_master && analog_bounds_master.rst_y == 0) {
        analog_stick_pins pins_master = stick_pins_master();
        analog_bounds_master.rst_y = pins_master.y;
    }

    // Setup slave/master communication handlers.
    transaction_register_rpc(KEYBOARD_SYNC_A, analog_sync_handler);
}

#endif
