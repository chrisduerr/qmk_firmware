/* Copyright 2017 Wunder
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "xd75.h"

// Layer shorthand
#define _QW 0
#define _FN 1

// NOOP
#define _____ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* DVORAK
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | PR SCR | 1      | 2      | 3      | 4      | 5      |        | PR UP  |        | 6      | 7      | 8      | 9      | 0      | DEL    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
 * | TAB    | '      | ,      | .      | P      | Y      |        | PG DWN |        | F      | G      | C      | R      | L      | `      |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
 * | BACKSP | A      | O      | E      | U      | I      |        |        |        | D      | H      | T      | N      | S      | ENTER  |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
 * | LSHIFT | ;      | Q      | J      | K      | X      |        | UP     |        | B      | M      | W      | V      | Z      | RSHIFT |
 * |--------+--------+--------+--------+--------+-----------------+--------+--------+--------+--------+-----------------+--------+--------|
 * | LCTRL  | LSUPER | LALT   | ESC    | FN     | SPACE  | LEFT   | DOWN   | RIGHT  | SPACE  | FN     | ESC    | RALT   | RSUPER | RCTRL  |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_QW] = { /* DVORAK */
  { KC_PSCREEN, KC_1,    KC_2,    KC_3,   KC_4,    KC_5,   KC_NO,   KC_PGUP,   KC_NO,   KC_6,   KC_7,    KC_8,   KC_9,    KC_0,    KC_DEL   },
  { KC_TAB,     KC_QUOT, KC_COMM, KC_DOT, KC_P,    KC_Y,   KC_NO,   KC_PGDOWN, KC_NO,   KC_F,   KC_G,    KC_C,   KC_R,    KC_L,    KC_GRAVE },
  { KC_BSPC,    KC_A,    KC_O,    KC_E,   KC_U,    KC_I,   KC_NO,   KC_NO,     KC_NO,   KC_D,   KC_H,    KC_T,   KC_N,    KC_S,    KC_ENT   },
  { KC_LSFT,    KC_SCLN, KC_Q,    KC_J,   KC_K,    KC_X,   KC_NO,   KC_UP,     KC_NO,   KC_B,   KC_M,    KC_W,   KC_V,    KC_Z,    KC_RSFT  },
  { KC_LCTL,    KC_LGUI, KC_LALT, KC_ESC, MO(_FN), KC_SPC, KC_LEFT, KC_DOWN,   KC_RGHT, KC_SPC, MO(_FN), KC_ESC, KC_RALT, KC_RGUI, KC_RCTL  },
 },

/* FUNCTION
 * .--------------------------------------------------------------------------------------------------------------------------------------.
 * | F1     | F2     | F3     | F4     | F5     | F6     |        |        |        | F7     | F8     | F9     | F10    | F11    | F12    |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | 1      | 2      | 3      | 4      | 5      |        |        |        | 6      | 7      | 8      | 9      | 0      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * |        | \      | =      | [      | (      |        |        |        |        |        | )      | ]      | -      | /      |        |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | LSHIFT |        |        |        |        |        |        |        |        |        |        | RGB    | RGM    | RGB    | RSHIFT |
 * |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
 * | RESET  |        |        |        | FN     |        |        |        |        |        | FN     | RGB    | RGB    | RGB    | RGB    |
 * '--------------------------------------------------------------------------------------------------------------------------------------'
 */

 [_FN] = { /* FUNCTION */
  { KC_F1,   KC_F2,   KC_F3,  KC_F4,   KC_F5,   KC_F6, _____, _____, _____, KC_F7, KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12  },
  { _____,   KC_1,    KC_2,   KC_3,    KC_4,    KC_5,  _____, _____, _____, KC_6,  KC_7,    KC_8,    KC_9,    KC_0,    _____   },
  { _____,   KC_BSLS, KC_EQL, KC_LBRC, KC_LPRN, _____, _____, _____, _____, _____, KC_RPRN, KC_RBRC, KC_MINS, KC_SLSH, _____   },
  { KC_LSFT, _____,   _____,  _____,   _____,   _____, _____, _____, _____, _____, _____,   RGB_HUD, RGB_HUI, RGB_MOD, KC_RSFT },
  { RESET,   _____,   _____,  _____,   MO(_FN), _____, _____, _____, _____, _____, MO(_FN), RGB_SAD, RGB_SAI, RGB_VAD, RGB_VAI },
 }
};

const uint16_t PROGMEM fn_actions[] = {

};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
  // MACRODOWN only works in this function
      switch(id) {
        case 0:
          if (record->event.pressed) {
            register_code(KC_RSFT);
            #ifdef BACKLIGHT_ENABLE
              backlight_step();
            #endif
          } else {
            unregister_code(KC_RSFT);
          }
        break;
      }
    return MACRO_NONE;
};
