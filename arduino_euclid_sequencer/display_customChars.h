/**
 * custom characters for the LCD display
 */
byte display_charArrowUp[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
byte display_charArrowDown[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte display_charArrowRight[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};
byte display_charArrowLeft[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b01100,
  0b00100,
  0b00000
};
byte display_charStepActive[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte display_charStepActiveCurrent[8] = {
  0b11111,
  0b11111,
  0b11011,
  0b10001,
  0b00000,
  0b10001,
  0b11011,
  0b11111
};
byte display_charStepInactive[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};
byte display_charStepInactiveCurrent[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11011,
  0b10101,
  0b11011,
  0b11111,
  0b11111
};
