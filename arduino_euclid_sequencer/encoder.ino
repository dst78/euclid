/**
 * Contains all handler routines for the rotary encoder.
 * 
 * The hardware used is an encoder with pushbutton.
 * The model I'm using performs 2 steps per indentation of the knob,
 * if yours is different, change the rotary_stepPerDent value.
 */

#include <Encoder.h>

#define ENCODER_PIN_BTN 8 // digital pin with interrupt support
#define ENCODER_PIN_1   6 // digital pin, ideally with interrupt support
#define ENCODER_PIN_2   7 // digital pin, ideally with interrupt support

Encoder encoder(ENCODER_PIN_1, ENCODER_PIN_2);
uint8_t encoder_stepPerDent = 2;

/**
 * call this in setup();
 */
void encoder_init() {
  pinMode(ENCODER_PIN_BTN, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_BTN), encoder_btnHandler, FALLING);
}

/**
 * call this in loop()
 */
void encoder_update() {
  int32_t newPos = encoder.read() / encoder_stepPerDent;

  if (newPos != 0) {
    display_encoderMove(newPos);
    encoder.write(0);
    
    #if DEBUG_ENCODER
    Serial.print("encoder moved, new pos: ");
    Serial.println(newPos);
    #endif
  }
}

/**
 * interrupt handler routine for the thumbstick pushbutton
 */
void encoder_btnHandler() {
  if (display_getEditMode() == 's') {
    // sequence editing mode
    if (sequencer_toggleSeqStep(display_calculateSeqId(), display_calculateSeqStep())) {
      display_updateSequenceStep();
    }

  } else {
    // menu mode
  }
  
  #if DEBUG_ENCODER
  Serial.println("encoder pushbutton was pressed");
  #endif
}
