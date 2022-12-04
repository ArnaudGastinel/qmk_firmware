/* Copyright 2022 Arnaud Gastinel
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

#include QMK_KEYBOARD_H
#include "print.h"
#include "secret.h"
// #include "sendstring_us_international.h"

enum planck_layers {
  QWERTY_LAYER,
  COLEMAK_LAYER,
  DVORAK_LAYER,
  LOWER_LAYER,
  RAISE_LAYER,
  NAV_LAYER,
  FN_LAYER,
  ADJUST_LAYER
};

enum planck_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
};

typedef enum {
  TD_NONE,
  TD_UNKNOWN,
  TD_SINGLE_TAP,
  TD_SINGLE_HOLD,
  TD_DOUBLE_TAP,
  TD_DOUBLE_HOLD,
  TD_DOUBLE_SINGLE_TAP, // Send two single taps
  TD_TRIPLE_TAP,
  TD_TRIPLE_HOLD
} td_state_t;

typedef struct {
  bool is_press_action;
  td_state_t state;
} td_tap_t;

// Tap dance enums
enum {
  TD_ESC_GRV = 0,
  // TD_X_CUT,
  // TD_C_COPY,
  // TD_V_PASTE,
  // TD_Z_UNDO,
  TD_SFT_L,
  TD_SFT_R
};

td_state_t cur_dance(qk_tap_dance_state_t *state);

// void z_on_each_tap(qk_tap_dance_state_t *state, void *user_data);
// void z_finished(qk_tap_dance_state_t *state, void *user_data);
// void z_reset(qk_tap_dance_state_t *state, void *user_data);

// void x_on_each_tap(qk_tap_dance_state_t *state, void *user_data);
// void x_finished(qk_tap_dance_state_t *state, void *user_data);
// void x_reset(qk_tap_dance_state_t *state, void *user_data);

// void c_on_each_tap(qk_tap_dance_state_t *state, void *user_data);
// void c_finished(qk_tap_dance_state_t *state, void *user_data);
// void c_reset(qk_tap_dance_state_t *state, void *user_data);

// void v_on_each_tap(qk_tap_dance_state_t *state, void *user_data);
// void v_finished(qk_tap_dance_state_t *state, void *user_data);
// void v_reset(qk_tap_dance_state_t *state, void *user_data);

void sft_on_each_tap(qk_tap_dance_state_t *state, void *user_data);
void sft_l_finished(qk_tap_dance_state_t *state, void *user_data);
void sft_l_reset(qk_tap_dance_state_t *state, void *user_data);
void sft_r_finished(qk_tap_dance_state_t *state, void *user_data);
void sft_r_reset(qk_tap_dance_state_t *state, void *user_data);

void error_management_reset(void);

// Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {
  // Tap once for Esc, twice for Grave
  [TD_ESC_GRV]  = ACTION_TAP_DANCE_DOUBLE(KC_ESC, KC_GRV),
  // [TD_Z_UNDO] = ACTION_TAP_DANCE_FN_ADVANCED(z_on_each_tap, z_finished, z_reset),
  // [TD_X_CUT] = ACTION_TAP_DANCE_FN_ADVANCED(x_on_each_tap, x_finished, x_reset),
  // [TD_C_COPY] = ACTION_TAP_DANCE_FN_ADVANCED(c_on_each_tap, c_finished, c_reset),
  // [TD_V_PASTE] = ACTION_TAP_DANCE_FN_ADVANCED(v_on_each_tap, v_finished, v_reset),
  [TD_SFT_L] = ACTION_TAP_DANCE_FN_ADVANCED(sft_on_each_tap, sft_l_finished, sft_l_reset),
  [TD_SFT_R] = ACTION_TAP_DANCE_FN_ADVANCED(sft_on_each_tap, sft_r_finished, sft_r_reset)
};

#define LOWER TT(LOWER_LAYER)
#define RAISE TT(RAISE_LAYER)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   *              ,-----------------------------------------------------------------------------------.
   *              | Tab  |   Q  |   W  |   E  |   R  |   T  |   Y  |   U  |   I  |   O  |   P  | Bksp |
   *              |------+------+------+------+------+------+------+------+------+------+------+------|
   * Tap for   -- | Esc  |   A  |   S  |   D  |   F  |   G  |   H  |   J  |   K  |   L  |   ;  |  '   |
   * Ctrl         |------+------+------+------+------+------+------+------+------+------+------+------|
   * Tap for ( -- | Shift|   Z  |   X  |   C  |   V  |   B  |   N  |   M  |   ,  |   .  |   /  |Shift | -- Tap for )
   *              |------+------+------+------+------+------+------+------+------+------+------+------|
   * Tap for [ ─┐ | Ctrl | FN   | Alt  | GUI  |Lower |    Space    |Raise | Menu | CMD  |FN    |Enter | -- Tap for ]
   *            │  `-----------------------------------------------------------------------------------'
   *            └-----------┘                                                      |
   *                                                                       Tap for Ctrl
   */
  [QWERTY_LAYER] = LAYOUT_planck_grid(
      TD(TD_ESC_GRV),   KC_Q,     KC_W,     KC_E,    KC_R,   KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,     KC_P,     KC_BSPC,
      KC_TAB,           KC_A,     KC_S,     KC_D,    KC_F,   KC_G,    KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN,  KC_QUOT,
      TD(TD_SFT_L),     KC_Z,     KC_X,     KC_C,    KC_V,   KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLSH,  TD(TD_SFT_R),
      KC_LCTL,          GUI_L,    KC_LGUI,  KC_LALT, LOWER,  NAV_SPC, NAV_SPC, RAISE,   KC_APP,  KC_RALT,  GUI_R,    CTL_ENT
  ),

// TD(TD_Z_UNDO),    TD(TD_X_CUT),    TD(TD_C_COPY),    TD(TD_V_PASTE), Removed for combos

  /* Colemak
  * ,-----------------------------------------------------------------------------------.
  * | Tab  |   Q  |   W  |   F  |   P  |   G  |   J  |   L  |   U  |   Y  |   ;  | Bksp |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Esc  |   A  |   R  |   S  |   T  |   D  |   H  |   N  |   E  |   I  |   O  |Enter |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Shift|   Z  |   X  |   C  |   V  |   B  |   K  |   M  |   ,  |   .  |   /  |Shift |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Ctrl |  FN  | Alt  | GUI  |Lower |    Space    |Raise | Menu | CMD  |FN    |Enter | -- Tap for ]
  * `-----------------------------------------------------------------------------------'
  */
  [COLEMAK_LAYER] = LAYOUT_planck_grid(
      _______, KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,    KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, _______,
      _______, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,    KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    _______,
      _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
  ),

  /* Dvorak
  * ,-----------------------------------------------------------------------------------.
  * | Tab  |   '  |   ,  |   .  |   P  |   Y  |   F  |   G  |   C  |   R  |   L  | Bksp |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Esc  |   A  |   O  |   E  |   U  |   I  |   D  |   H  |   T  |   N  |   S  |  /   |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Shift|   ;  |   Q  |   J  |   K  |   X  |   B  |   M  |   W  |   V  |   Z  |Enter |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * | Brite| Ctrl | Alt  | GUI  |Lower |    Space    |Raise | Left | Down |  Up  |Right |
  * `-----------------------------------------------------------------------------------'
  */
  [DVORAK_LAYER] = LAYOUT_planck_grid(
      KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y,    KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC,
      KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I,    KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH,
      KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X,    KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT,
      KC_LEAD, KC_LCTL, KC_LALT, KC_LGUI, LOWER,   KC_SPC,  KC_SPC,  RAISE,   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
  ),

  /* Numeric layer
    *   ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    *   │  `  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │  8  │  9  │  0  │     │
    *   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    *   │     │     │  ↑  │     │     │  *  │  +  │  4  │  5  │  6  │  *  │     │
    *   ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    *   │     │  ←  │  ↓  │  →  │     │  /  │  -  │  1  │  2  │  3  │  /  │     │
    *   ├─────┼─────┼─────┼─────╆━━━━━╅─────┴─────┼─────┼─────┼─────┼─────┼─────┤
    *   │     │     │     │     ┃     ┃           │     │  0  │  .  │  =  │     │
    *   └─────┴─────┴─────┴─────┺━━━━━┹───────────┴─────┴─────┴─────┴─────┴─────┘
    */
  [LOWER_LAYER] = LAYOUT_planck_grid(
    KC_GRV,  KC_1,     KC_2,    KC_3,     KC_4,    KC_5,     KC_6,     KC_7,    KC_8,  KC_9,   KC_0,        _______,
    _______, XXXXXXX,  KC_UP,   XXXXXXX,  XXXXXXX, KC_ASTR,  KC_PLUS,  KC_4,    KC_5,  KC_6,   KC_ASTR,     _______,
    _______, KC_LEFT,  KC_DOWN, KC_RGHT,  XXXXXXX, KC_SLASH, KC_MINUS, KC_1,    KC_2,  KC_3,   KC_SLASH,    _______,
    _______, _______,  _______, _______,  _______, _______,  _______,  _______, KC_0,  KC_DOT, KC_EQUAL,    _______
  ),

  /* Symbol layer
    *  ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
    *  │     │     │     │     │     │     │     │     │     │     │     │     │
    *  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
    *  │     │  !  │  @  │  #  │  $  │  %  │  ^  │  &  │  *  │  {  │  }  │     │ \
    *  ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤  |-- Mostly shifted version
    *  │     │  _  │  ~  │  |  │ INS │     │  {  │  }  │     │     │     │     │ /    of lower layer
    *  ├─────┼─────┼─────┼─────┼─────┼─────┴─────╆━━━━━╅─────┼─────┼─────┼─────┤
    *  │     │     │     │     │     │  Delete   ┃     ┃     │     │     │     │
    *  └─────┴─────┴─────┴─────┴─────┴───────────┺━━━━━┹─────┴─────┴─────┴─────┘
    */
  [RAISE_LAYER] = LAYOUT_planck_grid(
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
    _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_LCBR, KC_RCBR, KC_BSLS,
    _______, KC_UNDS, KC_TILD, KC_PIPE, KC_INS , _______, KC_LCBR, KC_RCBR, _______, _______,  _______, _______,
    _______, _______, _______, _______, _______, KC_DEL,  KC_DEL,  _______, _______, _______, _______, _______
  ),

  /* Directional navigation layer
  *
  *     ASD gaming -----/`````````\                               /``````````````````````\----- Vim-style arrow keys
  *                 ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
  *                 │     │Home │  ↑  │ End │     │     │     │     │     │     │     │     │     │
  *                 ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
  *                 │     │  ←  │  ↓  │  →  │     │     │     │  ←  │  ↓  │  ↑  │  →  │ PgUp│
  *                 ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
  *                 │     │     │     │     │     │     │     │     │     │     │     │ PgDn│
  *                 ├─────┼─────┼─────┼─────┼─────╆━━━━━━━━━━━╅─────┼─────┼─────┼─────┼─────┤
  *                 │     │     │     │     │     ┃           ┃     │     │     │     │     │
  *                 └─────┴─────┴─────┴─────┴─────┺━━━━━━━━━━━┹─────┴─────┴─────┴─────┴─────┘
  */
  [NAV_LAYER] = LAYOUT_planck_grid(
    XXXXXXX, KC_HOME, KC_UP,   KC_END,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_PGUP,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_PGDN,
    _______, _______, _______, _______, XXXXXXX, NAV_SPC, NAV_SPC, XXXXXXX, _______, _______, _______, _______
  ),

  /* GUI (window management/mouse/media controls) layer
  *
  *
  *                 ┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
  *                 │     │ F1  │ F2  │ F3  │ F4  │ F5  │ F6  │ F7  │ F8  │ F9  │ F10 │     │
  *                 ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
  *                 │     │ F11 │ F12 │     │     │     │     │     │     │     │     │     │
  *                 ├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
  *                 │     │     │     │     │     │     │     │     │     │     │     │     │
  *                 ├─────╆━━━━━╅─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────╆━━━━━╅─────┤
  *                 │ Prev┃     ┃Next │Play │Brig-│Sleep│Wake │Brig+│Mute │Vol- ┃     ┃ Vol+│
  *                 └─────┺━━━━━┹─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┺━━━━━┹─────┘
  *                    \______ Media _____/   \___ Screen/sleep __/   \_____ Volume _____/
  */
  [FN_LAYER] = LAYOUT_planck_grid(
    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,    KC_F10, _______,
    _______, KC_F11,  KC_F12,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, _______,
    _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX, _______,
    KC_MPRV, _______, KC_MNXT, KC_MPLY, KC_BRID, KC_SLEP, KC_SLEP, KC_BRIU, KC_MUTE, KC_VOLD,  _______, KC_VOLU
  ),

  /* Adjust (Lower + Raise)
  *                      v------------------------RGB CONTROL--------------------v
  * ,-----------------------------------------------------------------------------------.
  * | Reset|QWERTY|      | RGB  |RGBMOD| HUE+ | HUE- | SAT+ | SAT- |BRGTH+|BRGTH-|  Del |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * |      |      |MUSmod|Aud on|Audoff|AGnorm|AGswap|Qwerty|      |      |      |      |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * |      |Voice-|Voice+|Mus on|Musoff|MIDIon|MIDIof|      |      |      |      |      |
  * |------+------+------+------+------+------+------+------+------+------+------+------|
  * |      |      |      |      |      |             |      |      |      |      | Clic |
  * `-----------------------------------------------------------------------------------'
  */
  [ADJUST_LAYER] = LAYOUT_planck_grid(
      RESET,   QWERTY,   XXXXXXX, RGB_TOG, RGB_MOD, RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD,  RGB_VAI, RGB_VAD, KC_DEL ,
      _______, XXXXXXX,  MU_MOD,  AU_ON,   AU_OFF,  AG_NORM, AG_SWAP, QWERTY,  XXXXXXX,  XXXXXXX, XXXXXXX,  _______,
      _______, MUV_DE,   MUV_IN,  MU_ON,   MU_OFF,  MI_ON,   MI_OFF,  XXXXXXX, XXXXXXX, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______, _______, CK_TOGG
  )

};

enum combo_events {
  LEADER_KEY,
  E_ACUTE,
  E_GRAVE,
  E_TREMA,
  A_GRAVE,
  A_CIRCUM,
  I_TREMA,
  U_GRAVE,
  U_CIRCUM,
  HOME,
  END,
  // SFTZ_DASH,
  MIN,
  ALT_MIN,
  PLUS,
  EQUAL,
  UNDERSCORE,
  CEDILLE,
  GREATER_THAN,
  LESSER_THAN,
  NOT_EQUAL,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!
// Leader key through a combo
const uint16_t PROGMEM lead_key_combo[] = {KC_Q, KC_W, COMBO_END};
// Accents
const uint16_t PROGMEM e_acute_combo[] = {KC_W, KC_E, COMBO_END};
const uint16_t PROGMEM e_grave_combo[] = {KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM e_trema_combo[] = {KC_W, KC_E, KC_R, COMBO_END};
const uint16_t PROGMEM a_grave_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM a_circum_combo[] = {KC_TAB, KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM i_trema_combo[] = {KC_U, KC_I, KC_O, COMBO_END};
const uint16_t PROGMEM u_grave_combo[] = {KC_U, KC_I, COMBO_END};
const uint16_t PROGMEM u_circum_combo[] = {KC_Y, KC_U, KC_I, COMBO_END};

const uint16_t PROGMEM cedille_combo[] = {KC_X, KC_C, COMBO_END};
// const uint16_t PROGMEM shift_z_dash_combo[] = {KC_LSFT, KC_Z, COMBO_END};
// Symbols
const uint16_t PROGMEM underscore_combo[] = {KC_Z, KC_X, COMBO_END};
const uint16_t PROGMEM minus_combo[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM alt_minus_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM plus_combo[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM equal_combo[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM greater_than_combo[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM lesser_than_combo[] = {KC_DOT, KC_SLASH, COMBO_END};
const uint16_t PROGMEM not_equal_combo[] = {KC_COMM, KC_DOT, COMBO_END};

// Navigation
const uint16_t PROGMEM home_combo[] = {KC_A, KC_S, COMBO_END};
const uint16_t PROGMEM end_combo[] = {KC_D, KC_F, COMBO_END};


combo_t key_combos[] = {
    //
  [LEADER_KEY] = COMBO(lead_key_combo, KC_LEAD),
  /////////
  [HOME] = COMBO(home_combo, KC_HOME),
  [END] = COMBO(end_combo, KC_END),
  // [SFTZ_DASH] = COMBO(shift_z_dash_combo, KC_MINS),
  [MIN] = COMBO(minus_combo, KC_MINUS),
  [ALT_MIN] = COMBO(alt_minus_combo, KC_MINUS),
  [PLUS] = COMBO(plus_combo, KC_PLUS),
  [EQUAL] = COMBO(equal_combo, KC_EQUAL),
  [UNDERSCORE] = COMBO(underscore_combo, KC_UNDS),
  [GREATER_THAN] = COMBO(greater_than_combo, S(KC_COMM)),
  [LESSER_THAN] = COMBO(lesser_than_combo, S(KC_DOT)),
  ////////
  [E_ACUTE] = COMBO_ACTION(e_acute_combo),
  [E_GRAVE] = COMBO_ACTION(e_grave_combo),
  [E_TREMA] = COMBO_ACTION(e_trema_combo),
  [A_GRAVE] = COMBO_ACTION(a_grave_combo),
  [A_CIRCUM] = COMBO_ACTION(a_circum_combo),
  [I_TREMA] = COMBO_ACTION(i_trema_combo),
  [U_GRAVE] = COMBO_ACTION(u_grave_combo),
  [U_CIRCUM] = COMBO_ACTION(u_circum_combo),
  [NOT_EQUAL] = COMBO_ACTION(not_equal_combo),
  //
  [CEDILLE] = COMBO_ACTION(cedille_combo)


};
/* COMBO_ACTION(x) is same as COMBO(x, KC_NO) */

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case E_ACUTE:
      if (pressed) {
        SEND_STRING(SS_TAP(X_QUOT) SS_TAP(X_E));
      }
      break;
    case E_GRAVE:
      if (pressed) {
        SEND_STRING(SS_TAP(X_GRV) SS_TAP(X_E));
      }
      break;
    case E_TREMA:
      if (pressed) {
        SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_QUOT) SS_UP(X_LSFT) SS_TAP(X_E));
      }
      break;
    case A_GRAVE:
      if (pressed) {
        SEND_STRING(SS_TAP(X_GRV) SS_TAP(X_A));
      }
      break;
    case A_CIRCUM:
      if (pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_6)) SS_TAP(X_A));
      }
      break;
    case I_TREMA:
      if (pressed) {
        SEND_STRING(SS_DOWN(X_LSFT) SS_TAP(X_QUOT) SS_UP(X_LSFT) SS_TAP(X_I));
      }
      break;
    case U_GRAVE:
      if (pressed) {
        SEND_STRING(SS_TAP(X_GRV) SS_TAP(X_U));
      }
      break;
    case U_CIRCUM:
      if (pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_6)) SS_TAP(X_A));
      }
      break;
    case CEDILLE:
      if (pressed) {
        SEND_STRING(SS_DOWN(X_RALT) SS_TAP(X_COMM) SS_UP(X_RALT));
      }
      break;
    case NOT_EQUAL:
      if (pressed) {
        SEND_STRING(SS_LSFT(SS_TAP(X_1)) SS_TAP(X_EQL) );
      }
      break;
    // case BSPC_LSFT_CLEAR:
    //   if (pressed) {
    //     tap_code16(KC_END);
    //     tap_code16(S(KC_HOME));
    //     tap_code16(KC_BSPC);
    //   }
    //   break;
  }
}

// Audio part

#ifdef AUDIO_ENABLE
  float qwerty_song[][2]     = SONG(QWERTY_SOUND);
  float rick[][2] = SONG(RICK_ROLL);
  float leader_start_song[][2] = SONG(COIN_SOUND);
  float mario_mushroom_song[][2] = SONG(MARIO_MUSHROOM);
  float disney_song[][2] = SONG(DISNEY_SONG);
  float mario_theme_song[][2] = SONG(MARIO_THEME);
  float imperial_march_song[][2] = SONG(IMPERIAL_MARCH);
  float mario_game_over_song[][2] = SONG(MARIO_GAMEOVER);
#endif

// Initialize variable holding the binary
// representation of active modifiers.
uint8_t mod_state;
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (!process_caps_word(keycode, record)) { return false; }

  error_management_reset();
  
  // Store the current modifier state in the variable for later reference
  mod_state = get_mods();

  switch (keycode) {
    // Handle Shift + BSPC as Delete /////////////////////
    // https://docs.qmk.fm/#/feature_advanced_keycodes?id=shift-backspace-for-delete
    case KC_BSPC:
    {
    // Initialize a boolean variable that keeps track
    // of the delete key status: registered or not?
    static bool delkey_registered;
    if (record->event.pressed) {
      // Detect the activation of either shift keys
      if (mod_state & MOD_MASK_SHIFT) {
        // First temporarily canceling both shifts so that
        // shift isn't applied to the KC_DEL keycode
        del_mods(MOD_MASK_SHIFT);
        register_code(KC_DEL);
        // Update the boolean variable to reflect the status of KC_DEL
        delkey_registered = true;
        // Reapplying modifier state so that the held shift key(s)
        // still work even after having tapped the Backspace/Delete key.
        set_mods(mod_state);
        return false;
      }
    } else { // on release of KC_BSPC
      // In case KC_DEL is still being sent even after the release of KC_BSPC
      if (delkey_registered) {
        unregister_code(KC_DEL);
        delkey_registered = false;
        return false;
      }
    }
    // Let QMK process the KC_BSPC keycode as usual outside of shift
    return true;
    }
    // END Handle Shift + BSPC //////////////////////////
    // Hold X C V for Cut, Copy, Paste
    // case KC_X:
    //   if (!record->tap.count && record->event.pressed) {
    //     tap_code16(C(KC_X)); // Intercept hold function to send Ctrl-X
    //     return false;
    //   }
    //   return true;
    // case LT(0, KC_C):
    //   if (!record->tap.count && record->event.pressed) {
    //     tap_code16(C(KC_C)); // Intercept hold function to send Ctrl-C
    //     return false;
    //   }
    //   return true;
    // case LT(0, KC_V):
    //   if (!record->tap.count && record->event.pressed) {
    //     tap_code16(C(KC_V)); // Intercept hold function to send Ctrl-V
    //     return false;
    //   }
    //   return true;
    // END X,C,V hold
    case QWERTY:
      if (record->event.pressed) {
        print("mode just switched to qwerty\n");
        set_single_persistent_default_layer(QWERTY_LAYER);
      }
      return false;
      break;
    case COLEMAK:
      if (record->event.pressed) {
        print("mode just switched to colemak\n");
        set_single_persistent_default_layer(COLEMAK_LAYER);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        print("mode just switched to dvorak\n");
        set_single_persistent_default_layer(DVORAK_LAYER);
      }
      return false;
      break;
    case RESET:
        rgblight_setrgb(RGB_PINK);
        rgblight_setrgb_at(RGB_CYAN, BOTTOM_RIGHT_LED);
        rgblight_setrgb_at(RGB_CYAN, BOTTOM_LEFT_LED);
  }
  return true;
}

#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_LSPO:
    case KC_RSPC:
      return TAPPING_TERM - 50;
    case NAV_SPC:
      return TAPPING_TERM + 150;
    default:
      return TAPPING_TERM;
  }
}
#endif

// TODO https://docs.qmk.fm/#/feature_tap_dance?id=example-1
// TODO https://docs.qmk.fm/#/tap_hold?id=retro-tapping

// #ifdef RGB_MATRIX_ENABLE
// void rgb_matrix_indicators_user(void) {
//   switch (biton32(layer_state)) {
//     case LOWER_LAYER:
//       rgb_matrix_set_color(40, 0xFF, 0xFF, 0xFF); // LOWER
//       break;
//     case RAISE_LAYER:
//       rgb_matrix_set_color(44, 0xFF, 0xFF, 0xFF); // RAISE
//       break;
//     case NAV_LAYER:
//       rgb_matrix_set_color(43, 0xFF, 0xFF, 0xFF); // NAV_BSP
//       break;
//     case FN_LAYER:
//       rgb_matrix_set_color(36, 0xFF, 0xFF, 0xFF); // GUI_L
//       rgb_matrix_set_color(48, 0xFF, 0xFF, 0xFF); // GUI_R
//       break;
//     case STENO_LAYER:
//       rgb_matrix_set_color(36, 0xFF, 0x30, 0x00); // STN_EXIT

//       // Mask out everything but alphabetic steno keys.
//       rgb_matrix_set_color(0, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(1, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(2, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(3, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(4, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(5, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(6, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(7, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(8, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(9, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(10, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(11, 0x00, 0x00, 0x00);

//       rgb_matrix_set_color(12, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(17, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(18, 0x00, 0x00, 0x00);

//       rgb_matrix_set_color(24, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(29, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(30, 0x00, 0x00, 0x00);

//       rgb_matrix_set_color(37, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(38, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(39, 0x00, 0x00, 0x00);

//       rgb_matrix_set_color(45, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(46, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(47, 0x00, 0x00, 0x00);
//       rgb_matrix_set_color(48, 0x00, 0x00, 0x00);
//       break;
//     case ADJUST_LAYER:
//       rgb_matrix_set_color(40, 0xFF, 0xFF, 0xFF); // LOWER
//       rgb_matrix_set_color(44, 0xFF, 0xFF, 0xFF); // RAISE
//       break;
//     case CAMEL_LAYER:
//     case KEBAB_LAYER:
//     case SNAKE_LAYER:
//       rgb_matrix_set_color(12, 0xFF, 0x30, 0x00); // STCH_EX
//       rgb_matrix_set_color(41, 0x88, 0xFF, 0x00); // "Space bar"
//       break;
//   }

//   // Disable middle LED between keys in grid layout.
//   rgb_matrix_set_color(42, 0x00, 0x00, 0x00);
// }
// #endif

// TAP DANCE
/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'TD_DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
td_state_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return TD_SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) return TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) return TD_DOUBLE_HOLD;
        else return TD_DOUBLE_TAP;
    }

    // Assumes no one is trying to type the same letter three times (at least not quickly).
    // If your tap dance key is 'KC_W', and you want to type "www." quickly - then you will need to add
    // an exception here to return a 'TD_TRIPLE_SINGLE_TAP', and define that enum just like 'TD_DOUBLE_SINGLE_TAP'
    // if (state->count == 3) {
    //     if (state->interrupted || !state->pressed) return TD_TRIPLE_TAP;
    //     else return TD_TRIPLE_HOLD;
    // } else return TD_UNKNOWN;
    return TD_UNKNOWN;
}

void handleDefault(qk_tap_dance_state_t *state, uint16_t keycode) {
    int loop = state->count - 1;
    for (int i = 0; i < loop; i ++) {
      tap_code(keycode);
    }
    // if (state->interrupted || !state->pressed){
    // } else {
    // }
    register_code(keycode);
}

// Create an instance of 'td_tap_t' for the 'x' tap dance.
// static td_tap_t xtap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

// static td_tap_t ctap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

// static td_tap_t vtap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

// static td_tap_t ztap_state = {
//     .is_press_action = true,
//     .state = TD_NONE
// };

static td_tap_t l_sfttap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

static td_tap_t r_sfttap_state = {
    .is_press_action = true,
    .state = TD_NONE
};

// void tap_dance_on_each_step(td_tap_t* keycode_state, qk_tap_dance_state_t *state, uint16_t keycode) {
//   if (state->count > 2) {
//     state->finished = true;
//     keycode_state->state = TD_TRIPLE_TAP;
//     handleDefault(state, keycode);
//   }
// }

// void tap_dance_finished(td_tap_t* keycode_state, qk_tap_dance_state_t *state, uint16_t keycode) {
//   keycode_state->state = cur_dance(state);
//     switch (keycode_state->state) {
//         case TD_SINGLE_TAP: register_code(keycode); break;
//         case TD_DOUBLE_HOLD: register_code(KC_LCTL); tap_code(keycode); break;
//         default:
//           handleDefault(state, keycode);
//     }
// }

// void tap_dance_reset(td_tap_t* keycode_state, qk_tap_dance_state_t *state, uint16_t keycode) {
//   switch (keycode_state->state) {
//         case TD_SINGLE_TAP: unregister_code(keycode); break;
//         case TD_DOUBLE_HOLD: unregister_code(KC_LCTL); break;
//         default:
//           unregister_code(keycode);
//     }
//     keycode_state->state = TD_NONE;
// }

// // Ctrl + x
// void x_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_on_each_step(&xtap_state, state, KC_X);
// }

// void x_finished(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_finished(&xtap_state, state, KC_X);
// }

// void x_reset(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_reset(&xtap_state, state, KC_X);
// }

// // Ctrl + c
// void c_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_on_each_step(&ctap_state, state, KC_C);
// }

// void c_finished(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_finished(&ctap_state, state, KC_C);
// }

// void c_reset(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_reset(&ctap_state, state, KC_C);
// }

// // Ctrl + v
// void v_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_on_each_step(&vtap_state, state, KC_V);
// }

// void v_finished(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_finished(&vtap_state, state, KC_V);
// }

// void v_reset(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_reset(&vtap_state, state, KC_V);
// }

// // Ctrl + z
// void z_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_on_each_step(&ztap_state, state, KC_Z);
// }

// void z_finished(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_finished(&ztap_state, state, KC_Z);
// }

// void z_reset(qk_tap_dance_state_t *state, void *user_data) {
//     tap_dance_reset(&ztap_state, state, KC_Z);
// }
void sft_on_each_tap(qk_tap_dance_state_t *state, void *user_data) {
}

void sft_l_finished(qk_tap_dance_state_t *state, void *user_data) {
    if (state->count == 1) {
        if (state->pressed) l_sfttap_state.state = TD_SINGLE_HOLD;
        else l_sfttap_state.state = TD_SINGLE_TAP;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) l_sfttap_state.state = TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) l_sfttap_state.state = TD_DOUBLE_HOLD;
        else l_sfttap_state.state = TD_DOUBLE_TAP;
    }

  switch (l_sfttap_state.state) {
      case TD_SINGLE_HOLD: register_code(KC_LSFT); break;
      case TD_SINGLE_TAP: register_code(KC_LSFT); tap_code(KC_9); break;
      case TD_DOUBLE_TAP: SEND_STRING(SS_TAP(X_HOME) SS_LSFT(SS_TAP(X_END))); break;
      case TD_DOUBLE_HOLD: register_code(KC_LSFT); break;
      default:
        register_code(KC_LSFT);
        handleDefault(state, KC_9);
  }
}

void sft_l_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (l_sfttap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case TD_SINGLE_TAP: unregister_code(KC_LSFT); break;
        case TD_DOUBLE_TAP: break;
        case TD_DOUBLE_HOLD: unregister_code(KC_LSFT); break;
        default:
          unregister_code(KC_9);
          unregister_code(KC_LSFT);
    }
    l_sfttap_state.state = TD_NONE;
}

void sft_r_finished(qk_tap_dance_state_t *state, void *user_data) {
  if (state->count == 1) {
        if (!state->interrupted && !state->pressed) r_sfttap_state.state = TD_SINGLE_TAP;
        // Special case here, if the key is interrupted we must consider it as a HOLD
        else r_sfttap_state.state = TD_SINGLE_HOLD;
    } else if (state->count == 2) {
        // TD_DOUBLE_SINGLE_TAP is to distinguish between typing "pepper", and actually wanting a double tap
        // action when hitting 'pp'. Suggested use case for this return value is when you want to send two
        // keystrokes of the key, and not the 'double tap' action/macro.
        if (state->interrupted) r_sfttap_state.state = TD_DOUBLE_SINGLE_TAP;
        else if (state->pressed) r_sfttap_state.state = TD_DOUBLE_HOLD;
        else r_sfttap_state.state = TD_DOUBLE_TAP;
    }

  switch (r_sfttap_state.state) {
      case TD_SINGLE_HOLD: register_code(KC_RSFT); break;
      case TD_SINGLE_TAP: register_code(KC_RSFT); tap_code(KC_0); break;
      case TD_DOUBLE_TAP: SEND_STRING(SS_LCTL(SS_TAP(X_LEFT)) SS_LSFT(SS_LCTL(SS_TAP(X_RIGHT)))); break;
      case TD_DOUBLE_HOLD: register_code(KC_RSFT); break;
      default:
        register_code(KC_RSFT);
        handleDefault(state, KC_0);
  }
}

void sft_r_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (r_sfttap_state.state) {
        case TD_SINGLE_HOLD: unregister_code(KC_RSFT); break;
        case TD_SINGLE_TAP: unregister_code(KC_RSFT); break;
        case TD_DOUBLE_TAP: break;
        case TD_DOUBLE_HOLD: unregister_code(KC_RSFT); break;
        default:
          unregister_code(KC_0);
          unregister_code(KC_RSFT);
    }
    r_sfttap_state.state = TD_NONE;
}

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
    // case TD(TD_V_PASTE):
    // case TD(TD_C_COPY):
    // case TD(TD_X_CUT):
    // case TD(TD_Z_UNDO):
      add_weak_mods(MOD_BIT(KC_LSFT));  // Apply shift to next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_BSPC:
    case KC_MINS:
    case KC_UNDS:
    case KC_DEL:
      return true;

    default:
      return false;  // Deactivate Caps Word.
  }
}

// Leader key
bool did_leader_succeed;
bool leader_led_on = false;
static uint16_t leader_result_timer;

LEADER_EXTERNS();
void leader_start(void) {
#ifdef AUDIO_ENABLE
  //PLAY_SONG(leader_start_song);
#endif
  rgblight_setrgb_at(RGB_BLUE, TOP_LEFT_LED);
  rgblight_setrgb_at(RGB_BLUE, TOP_MIDDLE_LEFT_LED);
  rgblight_setrgb_at(RGB_BLUE, TOP_MIDDLE_RIGHT_LED);
  rgblight_setrgb_at(RGB_BLUE, TOP_RIGHT_LED);

  rgblight_setrgb_at(RGB_OFF, BOTTOM_LEFT_LED);
  rgblight_setrgb_at(RGB_OFF, BOTTOM_RIGHT_LED);
}

void leader_end(void) {
  // Turn off leader start indication
  rgblight_setrgb(RGB_OFF);

  if (did_leader_succeed) {
    rgblight_setrgb_at(RGB_GREEN, BOTTOM_LEFT_LED);
    rgblight_setrgb_at(RGB_GREEN, BOTTOM_RIGHT_LED);
  } else {
    rgblight_setrgb_at(RGB_PINK, BOTTOM_LEFT_LED);
    rgblight_setrgb_at(RGB_PINK, BOTTOM_RIGHT_LED);
  }
  leader_led_on = true;
  leader_result_timer = timer_read();
}

void matrix_scan_user(void) {
LEADER_DICTIONARY() {
  did_leader_succeed = leading = false;

  // GIT
  SEQ_TWO_KEYS(KC_G, KC_A) {
    SEND_STRING("git add . && git commit");
    did_leader_succeed = true;
  }
  SEQ_TWO_KEYS(KC_G, KC_F) {
    SEND_STRING("git push --force-with-lease");
    did_leader_succeed = true;
  }
  SEQ_THREE_KEYS(KC_G, KC_A, KC_M) {
    SEND_STRING("git add . && git commit --amend");
    did_leader_succeed = true;
  }

  // KUBERNETES
  SEQ_TWO_KEYS(KC_K, KC_P) {
    SEND_STRING("kubectl get pods");
    did_leader_succeed = true;
  }
  SEQ_TWO_KEYS(KC_K, KC_L) {
      SEND_STRING("kubectl logs -f --since=10m");
      did_leader_succeed = true;
  }
  SEQ_TWO_KEYS(KC_K, KC_D) {
      SEND_STRING("kubectl describe ");
      did_leader_succeed = true;
  }
  SEQ_TWO_KEYS(KC_K, KC_R) {
      SEND_STRING("kubectl delete ");
      did_leader_succeed = true;
  }

  // QMK
  SEQ_THREE_KEYS(KC_Q, KC_M, KC_K) {
    SEND_STRING("qmk compile -kb planck/rev6 -km paf");
    did_leader_succeed = true;
  }

  // DO NOT TELL ME WHAT TO DO
  SECRET_COMBO_1 {
    SEND_STRING_SECRET_1;
    did_leader_succeed = true;
  }

  // Songs
  SEQ_TWO_KEYS(KC_R, KC_R) {
    PLAY_SONG(rick);
    did_leader_succeed = true;
  }
  SEQ_FIVE_KEYS(KC_M, KC_A, KC_R, KC_I, KC_O) {
    PLAY_SONG(mario_theme_song);
    did_leader_succeed = true;
  }
  SEQ_FOUR_KEYS(KC_M, KC_U, KC_S, KC_H) {
    PLAY_SONG(mario_mushroom_song);
    did_leader_succeed = true;
  }
  SEQ_THREE_KEYS(KC_D, KC_I, KC_S) {
    PLAY_SONG(disney_song);
    did_leader_succeed = true;
  }
  SEQ_FOUR_KEYS(KC_S, KC_T, KC_A, KC_R) {
    PLAY_SONG(imperial_march_song);
    did_leader_succeed = true;
  }
  
  SEQ_FOUR_KEYS(KC_O, KC_V, KC_E, KC_R) {
    PLAY_SONG(mario_game_over_song);
    did_leader_succeed = true;
  }

  leader_end();
  }
}

// Error management (Led indicator)
void error_management_reset(void) {
  if (leader_led_on && (timer_elapsed(leader_result_timer) > MIN_INDICATION_DURATION)) {
    leader_led_on = false;
    rgblight_setrgb(RGB_OFF);
  }
}

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, LOWER_LAYER, RAISE_LAYER, ADJUST_LAYER);
    
    switch (get_highest_layer(state)) {
    case LOWER_LAYER:
        print("Layer LOWER\n");
        rgblight_setrgb_at(RGB_GREEN, TOP_LEFT_LED);
        rgblight_setrgb_at(RGB_GREEN, TOP_MIDDLE_LEFT_LED);
        rgblight_setrgb_at(RGB_GREEN, TOP_MIDDLE_RIGHT_LED);
        rgblight_setrgb_at(RGB_GREEN, TOP_RIGHT_LED);
        break;
    case RAISE_LAYER:
        print("Layer RAISE\n");
        rgblight_setrgb_at(RGB_MAGENTA, TOP_LEFT_LED);
        rgblight_setrgb_at(RGB_MAGENTA, TOP_MIDDLE_LEFT_LED);
        rgblight_setrgb_at(RGB_MAGENTA, TOP_MIDDLE_RIGHT_LED);
        rgblight_setrgb_at(RGB_MAGENTA, TOP_RIGHT_LED);
        break;
    case ADJUST_LAYER:
        print("Layer ADJUST\n");
        rgblight_setrgb_at(RGB_CYAN, TOP_LEFT_LED);
        rgblight_setrgb_at(RGB_CYAN, TOP_MIDDLE_LEFT_LED);
        rgblight_setrgb_at(RGB_CYAN, TOP_MIDDLE_RIGHT_LED);
        rgblight_setrgb_at(RGB_CYAN, TOP_RIGHT_LED);
        break;
    case FN_LAYER:
        print("Layer FN\n");
        rgblight_setrgb_at(RGB_CORAL, TOP_LEFT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_MIDDLE_LEFT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_MIDDLE_RIGHT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_RIGHT_LED);
        break;
    case NAV_LAYER:
        print("Layer NAV\n");
        rgblight_setrgb_at(RGB_CORAL, TOP_LEFT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_MIDDLE_LEFT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_MIDDLE_RIGHT_LED);
        rgblight_setrgb_at(RGB_CORAL, TOP_RIGHT_LED);
        break;
    default: //  for any other layers, or the default layer
        print("No layer\n");
        rgblight_setrgb (RGB_OFF);
        break;
    }
  return state;
}
