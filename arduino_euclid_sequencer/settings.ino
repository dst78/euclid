/**
 * data structures and handler functions for settings
 */

char setting_clocksource = SETTING_CLOCKSOURCE_CV;

#define SETTING_BPM_MIN 30
#define SETTING_BPM_MAX 240
uint8_t setting_bpm = 120;

#define SETTING_SWING_MIN 50
#define SETTING_SWING_MAX 75
int8_t setting_swing = SETTING_SWING_MIN;

uint8_t setting_parameterIndex = 10;
uint8_t setting_parameterNum = 10;
static const char *SETTING_PARAMETER_NAMES[] = {
  "Mchn", // 10 - MIDI channel for the instrument
  "Slen", // 0  - sequence length
  "Spul", // 1  - active sequence pulses - this is in the Euclidian Rythm sense
  "Srot", // 2  - sequence offset - this is in the Euclidian Rythm sense
  "Sdir", // 3  - sequence play direction - forward, backward, alternating
  "Snln", // 4  - sequence note length - 16th, 8th, 4th
  "Sgte", // 5  - sequence gate length
  "Svel", // 6  - sequence note velocity
  "Snte", // 7  - sequence MIDI note
  "Srda", // 8  - sequence randomization amount
  "Srdc", // 9  - sequence randomization chance
};

static const uint8_t SETTING_PARAMETER_VALUE_MIDICHANNELS[] = {1, 2, 3, 4, 5, 6, 7, 8,9, 10, 11, 12, 13, 14, 15, 16};


uint8_t setting_parameterValueMidiChannel = 10;




/**
 * returns a single character indicating the current clock source
 */
char setting_getClocksource() {
  return setting_clocksource;
}

/**
 * toggles between clocksources
 */
void setting_changeClocksource(int32_t delta) {
  if (setting_clocksource == SETTING_CLOCKSOURCE_CV) {
    setting_clocksource = SETTING_CLOCKSOURCE_INTERNAL;
  } else {
    setting_clocksource = SETTING_CLOCKSOURCE_CV;
  }
}

/**
 * returns the current BPM
 */
char* setting_getBPM() {
  char* returnValue = (char *) malloc(3);
  sprintf(returnValue, "%03d", setting_bpm);
  return returnValue;
}

/**
 * changes the BPM up or down
 */
void setting_changeBPM(int32_t delta) {
  setting_bpm = min(SETTING_BPM_MAX, max(SETTING_BPM_MIN, setting_bpm + delta));
}

/**
 * returns the current swing setting
 */
char* setting_getSwing() {
  char * returnValue = (char *) malloc(2);
  sprintf(returnValue, "%02d", setting_swing);
  return returnValue;
}

/**
 * changes the swing setting up or down
 */
void setting_changeSwing(int32_t delta) {
  setting_swing = min(SETTING_SWING_MAX, max(SETTING_SWING_MIN, setting_swing + delta));
}

/**
 * returns the current parameter name
 */
char* setting_getParameter() {
  char* returnValue = (char *) malloc(4);
  strcpy(returnValue, SETTING_PARAMETER_NAMES[setting_parameterIndex]);
  return returnValue;
}

/**
 * changes the current parameter up or down
 */
void setting_changeParameter(int32_t delta) {
  setting_parameterIndex = max(0, min(setting_parameterNum, setting_parameterIndex + delta));
}

/**
 * returns teh current parameter value
 */
char* setting_getParameterValue() {
  char* returnValue = (char *) malloc(3);
  strcpy(returnValue, "val");
  return returnValue;
}

/**
 * changes the current parameter value up or down
 */
void setting_changeParameterValue(int32_t delta) {
  
}
