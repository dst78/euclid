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

#define SETTING_PARAMETER_NAME_MCHN "Mchn" // MIDI channel for the instrument
#define SETTING_PARAMETER_NAME_MNTE "Mnte" // sequence MIDI note
#define SETTING_PARAMETER_NAME_MGTE "Mgte" // sequence gate length
#define SETTING_PARAMETER_NAME_MVEL "Mvel" // sequence note velocity
#define SETTING_PARAMETER_NAME_SLEN "Slen" // sequence length
#define SETTING_PARAMETER_NAME_SPUL "Spul" // active sequence pulses - this is in the Euclidian Rythm sense
#define SETTING_PARAMETER_NAME_SROT "Srot" // sequence offset - this is in the Euclidian Rythm sense
#define SETTING_PARAMETER_NAME_SDIR "Sdir" // sequence play direction - forward, backward, alternating
#define SETTING_PARAMETER_NAME_SNLN "Snln" // sequence note length - 16th, 8th, 4th
#define SETTING_PARAMETER_NAME_SRDA "Srda" // sequence randomization amount
#define SETTING_PARAMETER_NAME_SRDC "Srdc" // sequence randomization chance

#define SETTING_PARAMETER_VALUE_MNTE_MIN 24 // C1
#define SETTING_PARAMETER_VALUE_MNTE_MAX 96 // C7

#define SETTING_PARAMETER_VALUE_MGTE_MIN  1
#define SETTING_PARAMETER_VALUE_MGTE_MAX 99

#define SETTING_PARAMETER_VALUE_MVEL_MIN   1
#define SETTING_PARAMETER_VALUE_MVEL_MAX 127

#define SETTING_PARAMETER_VALUE_SDIR_MIN 0
#define SETTING_PARAMETER_VALUE_SDIR_MAX 2

#define SETTING_PARAMETER_VALUE_SNLN_MIN 0
#define SETTING_PARAMETER_VALUE_SNLN_MAX 2


uint8_t setting_parameterIndex = 0;
uint8_t setting_parameterNum = 11;
static const char *SETTING_PARAMETER_NAMES[] = {
  SETTING_PARAMETER_NAME_MCHN, // 0 - MIDI channel for the instrument
  SETTING_PARAMETER_NAME_MNTE, // 1  - sequence MIDI note
  SETTING_PARAMETER_NAME_MGTE, // 2  - sequence MIDI gate length
  SETTING_PARAMETER_NAME_MVEL, // 3  - sequence MIDI note velocity
  SETTING_PARAMETER_NAME_SLEN, // 4  - sequence length
  SETTING_PARAMETER_NAME_SPUL, // 5  - active sequence pulses - this is in the Euclidian Rythm sense
  SETTING_PARAMETER_NAME_SROT, // 6  - sequence offset - this is in the Euclidian Rythm sense
  SETTING_PARAMETER_NAME_SDIR, // 7  - sequence play direction - forward, backward, alternating
  SETTING_PARAMETER_NAME_SNLN, // 8  - sequence note length - 16th, 8th, 4th
  SETTING_PARAMETER_NAME_SRDA, // 9  - sequence randomization amount
  SETTING_PARAMETER_NAME_SRDC, // 10 - sequence randomization chance
};

static const char *SETTING_PARAMETER_VALUE_MIDINOTES[73] = {
                                                                             " C1", // MIDI note 24
"C#1", " D1", "D#1", " E1", " F1", "F#1", " G1", "G#1", " A1", "A#1", " B1", " C2", // MIDI notes 25-36
"C#2", " D2", "D#2", " E2", " F2", "F#2", " G2", "G#2", " A2", "A#2", " B2", " C3", // MIDI notes 37-48
"C#3", " D3", "D#3", " E3", " F3", "F#3", " G3", "G#3", " A3", "A#3", " B3", " C4", // MIDI notes 49-60
"C#4", " D4", "D#4", " E4", " F4", "F#4", " G4", "G#4", " A4", "A#4", " B4", " C5", // MIDI notes 61-72
"C#5", " D5", "D#5", " E5", " F5", "F#5", " G5", "G#5", " A5", "A#5", " B5", " C6", // MIDI notes 73-84
"C#6", " D6", "D#6", " E6", " F6", "F#6", " G6", "G#6", " A6", "A#6", " B6", " C7", // MIDI notes 85-96
};
static const char *SETTING_PARAMETER_VALUE_SDIR[3] = {"Fwd", "Bck", "Alt"};
static const char *SETTING_PARAMETER_VALUE_SNLN[3] = {"/16", "1/8", "1/4"};

uint8_t setting_parameterValue_Mchn    = 10;
uint8_t setting_parameterValue_tmp     = setting_parameterValue_Mchn; // first parameter to show is Mchn so we're copying that value
uint8_t setting_parameterValue_Mnte[8] = {36, 38, 42, 46, 37, 75, 48, 41};
uint8_t setting_parameterValue_Mgte[8] = {50, 50, 50, 50, 50, 50, 50, 50};
uint8_t setting_parameterValue_Mvel[8] = {100, 100, 100, 100, 100, 100, 100, 100};
uint8_t setting_parameterValue_Slen[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Spul[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Srot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Sdir[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Snln[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Srda[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Srdc[8] = {0, 0, 0, 0, 0, 0, 0, 0};




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

  // set temporary parameter value to actual value of new parameter to be safe
  uint8_t seqId = display_calculateSeqId();

  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument
    setting_parameterValue_tmp = setting_parameterValue_Mchn;
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    setting_parameterValue_tmp = setting_parameterValue_Mnte[seqId] - SETTING_PARAMETER_VALUE_MNTE_MIN;
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    setting_parameterValue_tmp = setting_parameterValue_Mgte[seqId];
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    setting_parameterValue_tmp = setting_parameterValue_Mvel[seqId];
  } else if (setting_parameterIndex == 4) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    setting_parameterValue_tmp = setting_parameterValue_Slen[seqId];
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = setting_parameterValue_Spul[seqId];
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SROT - sequence offset - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = setting_parameterValue_Srot[seqId];
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    setting_parameterValue_tmp = setting_parameterValue_Sdir[seqId];
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length - 16th, 8th, 4th
    setting_parameterValue_tmp = setting_parameterValue_Snln[seqId];
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    setting_parameterValue_tmp = setting_parameterValue_Srda[seqId];
  } else if (setting_parameterIndex == 10) {  
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    setting_parameterValue_tmp = setting_parameterValue_Srdc[seqId];
  } 
}

/**
 * returns the current parameter value
 */
char* setting_getParameterValue() {
  char* returnValue = (char *) malloc(3);
  uint8_t seqId = display_calculateSeqId();

  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument
    sprintf(returnValue, "%3d", setting_parameterValue_Mchn);
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    strcpy(returnValue, SETTING_PARAMETER_VALUE_MIDINOTES[setting_parameterValue_Mnte[seqId] - SETTING_PARAMETER_VALUE_MNTE_MIN]);
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    sprintf(returnValue, "%3d", setting_parameterValue_Mgte[seqId]);
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    sprintf(returnValue, "%3d", setting_parameterValue_Mvel[seqId]);
  } else if (setting_parameterIndex == 4) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    sprintf(returnValue, "%3d", setting_parameterValue_Slen[seqId]);
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    sprintf(returnValue, "%3d", setting_parameterValue_Spul[seqId]);
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SROT - sequence offset - this is in the Euclidian Rythm sense
    sprintf(returnValue, "%3d", setting_parameterValue_Srot[seqId]);
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SDIR[setting_parameterValue_Sdir[seqId]]);
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length - 16th, 8th, 4th
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SNLN[setting_parameterValue_Snln[seqId]]);
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    sprintf(returnValue, "%3d", setting_parameterValue_Srda[seqId]);
  } else if (setting_parameterIndex == 10) {  
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    sprintf(returnValue, "%3d", setting_parameterValue_Srdc[seqId]);  
  }
 
  return returnValue;
}

/**
 * returns the temporary parameter value
 * 
 * be aware that there is only a single tmp value, which is updated whenever a new parameter
 * is selected or when a parameter value is being changed. 
 * calling this function instead of setting_getParameterValue() will yield unexpected results
 * 99% of the time.
 */
char* setting_getParameterTmpValue() {
  char* returnValue = (char *) malloc(3);
  
  if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    strcpy(returnValue, SETTING_PARAMETER_VALUE_MIDINOTES[setting_parameterValue_tmp]);
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SDIR[setting_parameterValue_tmp]);
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length - 16th, 8th, 4th
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SNLN[setting_parameterValue_tmp]);
  } else {
    // numerical parameter value
    sprintf(returnValue, "%3d", setting_parameterValue_tmp);
  }
  
  return returnValue;
}

/**
 * changes the current parameter value up or down
 */
void setting_changeParameterValue(int32_t delta) {
  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument  
    setting_parameterValue_tmp = min(16, max(1, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_MNTE_MAX - SETTING_PARAMETER_VALUE_MNTE_MIN, max(0, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_MGTE_MAX, max(SETTING_PARAMETER_VALUE_MGTE_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_MVEL_MAX, max(SETTING_PARAMETER_VALUE_MVEL_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 4) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    setting_parameterValue_tmp = min(32, max(0, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = 0;
    
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SROT - sequence offset - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = 0;
  
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_SDIR_MAX, max(SETTING_PARAMETER_VALUE_SDIR_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length - 16th, 8th, 4th
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_SNLN_MAX, max(SETTING_PARAMETER_VALUE_SNLN_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    setting_parameterValue_tmp = min(100, max(0, setting_parameterValue_tmp + delta));  
  } else if (setting_parameterIndex == 10) {
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    setting_parameterValue_tmp = min(100, max(0, setting_parameterValue_tmp + delta));   
  }
}
