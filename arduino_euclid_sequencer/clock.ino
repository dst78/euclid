/**
 * handles both the internal and the CV-driven clock
 * 
 * the internal clock is implemented as an interval timer with an interval of 
 * 12 times the BPM value, in order to play 16th note triplets
 */

#include "constants.h"

IntervalTimer clock_internal;
  
#define CLOCK_LED_PIN 13

// timer interval in ms
float clock_interval;
volatile uint8_t clock_ticks;

/**
 * call this during setup()
 */
void clock_init() {
  pinMode(CLOCK_LED_PIN, OUTPUT);
  digitalWrite(CLOCK_LED_PIN, LOW);
  
  clock_setInterval();
  if (setting_getClocksource() == SETTING_CLOCKSOURCE_INTERNAL) {
    clock_useInternal();
  }
}

/**
 * starts the internal clock
 */
void clock_useInternal() {
  clock_ticks = 0;
  clock_internal.begin(clock_timerCallback, clock_interval);
}

/**
 * stops the internal clock
 */
void clock_useExternal() {
  clock_internal.end();
}

/**
 * sets the interval timer of the internal clock to CLOCK_TIMER_RESOLUTION times the BPM value
 */
void clock_setInterval() {
  clock_setInterval(setting_getRawBPM());
}

/**
 * sets the interval timer of the internal clock to CLOCK_TIMER_RESOLUTION times the BPM value
 */
void clock_setInterval(uint8_t bpm) {
  clock_interval = 60000000 / (bpm * CLOCK_TIMER_RESOLUTION);
  clock_internal.update(clock_interval);

  #if DEBUG_CLOCK
  Serial.print("Setting internal clock to ");
  Serial.print(setting_getRawBPM());
  Serial.print(" BPM == ");
  Serial.print(clock_interval);
  Serial.println(" microseconds");
  #endif  
}

/**
 * callback function for the internal clock
 */
void clock_timerCallback() {
  clock_ticks++;
  
  if (clock_ticks % CLOCK_TIMER_RESOLUTION == 0) {
    // quarter note
    clock_ticks = 0;
    digitalWrite(CLOCK_LED_PIN, HIGH);

    sequencer_handlePositionIncrement(CLOCK_INCREMENT_QUARTERS);
  }

  if (clock_ticks % ((CLOCK_TIMER_RESOLUTION * 2) / 3) == 0) {
    // quarter note triplets
    sequencer_handlePositionIncrement(CLOCK_INCREMENT_QUARTER_TRIPLETS);
  }
  
  if (clock_ticks % (CLOCK_TIMER_RESOLUTION / 2) == 0) {
    // eighth note
    sequencer_handlePositionIncrement(CLOCK_INCREMENT_EIGHTHS);
  }

  if (clock_ticks % (CLOCK_TIMER_RESOLUTION / 3) == 0) {
    // eighth note triplets
    sequencer_handlePositionIncrement(CLOCK_INCREMENT_EIGHTH_TRIPLETS);
  }
  
  if (clock_ticks % (CLOCK_TIMER_RESOLUTION / 4) == 0) {
    // sixteenth note 
    sequencer_handlePositionIncrement(CLOCK_INCREMENT_SIXTEENTHS);
  }

  if (clock_ticks % (CLOCK_TIMER_RESOLUTION / 6) == 0) {
    // sixteenth note triplets
    sequencer_handlePositionIncrement(CLOCK_INCREMENT_SIXTEENTH_TRIPLETS);
  }
  
  if (clock_ticks == 2) {
    digitalWrite(CLOCK_LED_PIN, LOW);
  }
  
}
