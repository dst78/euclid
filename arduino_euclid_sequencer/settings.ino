/**
 * data structures and handler functions for settings
 */

#define SETTING_OUTPUT_MIN 0
#define SETTING_OUTPUT_MAX 2
static const char SETTING_OUTPUT_OPTIONS[3] = {SETTING_OUTPUT_MIDI, SETTING_OUTPUT_AUDIO, SETTING_OUTPUT_OFF};
uint8_t setting_output = SETTING_OUTPUT_MIN;

char setting_clocksource = SETTING_CLOCKSOURCE_INTERNAL;

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
#define SETTING_PARAMETER_NAME_SVOL "Svol" // sequence volume (when using audio out)
#define SETTING_PARAMETER_NAME_SLEN "Slen" // sequence length
#define SETTING_PARAMETER_NAME_SPUL "Spul" // active sequence pulses - this is in the Euclidian Rythm sense
#define SETTING_PARAMETER_NAME_SOFF "Soff" // sequence offset - this is in the Euclidian Rythm sense
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
#define SETTING_PARAMETER_VALUE_SNLN_MAX 5

#define SETTING_PARAMETER_VALUE_SVOL_MIN 0
#define SETTING_PARAMETER_VALUE_SVOL_MAX 15


uint8_t setting_parameterIndex = 0;
uint8_t setting_parameterNum = 11;
static const char *SETTING_PARAMETER_NAMES[] = {
  SETTING_PARAMETER_NAME_MCHN, // 0  - MIDI channel for the instrument
  SETTING_PARAMETER_NAME_MNTE, // 1  - sequence MIDI note
  SETTING_PARAMETER_NAME_MGTE, // 2  - sequence MIDI gate length
  SETTING_PARAMETER_NAME_MVEL, // 3  - sequence MIDI note velocity
  SETTING_PARAMETER_NAME_SVOL, // 4  - sequence volume
  SETTING_PARAMETER_NAME_SLEN, // 5  - sequence length
  SETTING_PARAMETER_NAME_SPUL, // 6  - active sequence pulses - this is in the Euclidian Rythm sense
  SETTING_PARAMETER_NAME_SOFF, // 7  - sequence offset - this is in the Euclidian Rythm sense
  SETTING_PARAMETER_NAME_SDIR, // 8  - sequence play direction - forward, backward, alternating
  SETTING_PARAMETER_NAME_SNLN, // 9  - sequence note length - 16th, 8th, 4th
  SETTING_PARAMETER_NAME_SRDA, // 10 - sequence randomization amount
  SETTING_PARAMETER_NAME_SRDC  // 11 - sequence randomization chance
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

/**
 *       24    1 /  4th
 *       16    1 /  4th triplets
 *       12    1 /  8th
 *        8    1 /  8th triplets
 *        6    1 / 16th
 *        4    1 / 16th triplets
 */
static const char *SETTING_PARAMETER_VALUE_SNLN[6]        = {"16t", "16 ", " 8t", " 8 ", " 4t", " 4 "};
static const uint8_t SETTING_PARAMETER_VALUE_SNLN_VAL[6]  = {
  CLOCK_INCREMENT_SIXTEENTH_TRIPLETS, 
  CLOCK_INCREMENT_SIXTEENTHS, 
  CLOCK_INCREMENT_EIGHTH_TRIPLETS,
  CLOCK_INCREMENT_EIGHTHS,
  CLOCK_INCREMENT_QUARTER_TRIPLETS,
  CLOCK_INCREMENT_QUARTERS
};
// inverse lookup, values of 255 should never be hit. those with values 0-1 indicate the index in SETTING_PARAMETER_VALUE_SNLN_VAL
static const uint8_t SETTING_PARAMETER_VALUE_SNLN_INV[25] = {255, 255, 255, 255, 0, 255, 1, 255, 2, 255, 255, 255, 3, 255, 255, 255, 4, 255, 255, 255, 255, 255, 255, 255, 5};

uint8_t setting_parameterValue_Mchn    = 10;
int8_t setting_parameterValue_tmp     = setting_parameterValue_Mchn; // first parameter to show is Mchn so we're copying that value
uint8_t setting_parameterValue_Mgte[8] = {50, 50, 50, 50, 50, 50, 50, 50};
uint8_t setting_parameterValue_Mvel[8] = {100, 100, 100, 100, 100, 100, 100, 100};
uint8_t setting_parameterValue_Spul[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Snln[8] = {1, 1, 1, 1, 1, 1, 1, 1}; // this is not the value but the array index in SETTING_PARAMETER_VALUE_SNLN
uint8_t setting_parameterValue_Sdir[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Srda[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t setting_parameterValue_Srdc[8] = {0, 0, 0, 0, 0, 0, 0, 0};




/**
 * returns a single character indicating the current clock source
 */
char setting_getClocksource() {
  return setting_clocksource;
}

/**
 * toggles between clocksources, change is immediate
 */
void setting_changeClocksource(int32_t delta) {
  if (setting_clocksource == SETTING_CLOCKSOURCE_CV) {
    setting_clocksource = SETTING_CLOCKSOURCE_INTERNAL;
    clock_useInternal();
  } else {
    setting_clocksource = SETTING_CLOCKSOURCE_CV;
    clock_useExternal();
  }
}

/**
 * returns the active output type
 */
char setting_getOutput() {
  return SETTING_OUTPUT_OPTIONS[setting_output];
}

/**
 * changes the output type
 */
void setting_changeOutput(int32_t delta) {
  if (setting_output == SETTING_OUTPUT_MIN) {
    // MIDI
    if (delta < 0) {
      setting_output = SETTING_OUTPUT_MAX;
    } else if (delta > 0) {
      setting_output++;
    }
    
  } else if (setting_output == SETTING_OUTPUT_MAX) {
    // no output
    if (delta < 0) {
      setting_output--;
    } else if (delta > 0) {
      setting_output = SETTING_OUTPUT_MIN;
    }
    
  } else {
    if (delta < 0) {
      setting_output--;
    } else if (delta > 0) {
      setting_output++;
    } 
  }
}

/**
 * returns the BPM as a number
 */
uint8_t setting_getRawBPM() {
  return setting_bpm;
}

/**
 * returns the current BPM as array of char
 */
char* setting_getBPM() {
  char* returnValue = (char *) malloc(3);
  sprintf(returnValue, "%03d", setting_bpm);
  return returnValue;
}

/**
 * changes the BPM up or down, change is immediate
 * @todo must update the timer interrupt for internal clock
 */
void setting_changeBPM(int32_t delta) {
  setting_bpm = min(SETTING_BPM_MAX, max(SETTING_BPM_MIN, setting_bpm + delta));
  clock_setInterval(setting_bpm);
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
 * changes the swing setting up or down, change is immediate
 * @todo swing must be handled
 */
void setting_changeSwing(int32_t delta) {
  setting_swing = min(SETTING_SWING_MAX, max(SETTING_SWING_MIN, setting_swing + delta));
}

/**
 * returns the MIDI channel of the instrument
 */
uint8_t setting_getMidiChannel() {
  return setting_parameterValue_Mchn;
}

/**
 * returns the configured MIDI velocity
 */
uint8_t setting_getMidiVelocity(uint8_t seqId) {
  return setting_parameterValue_Mvel[seqId];
}

/**
 * returns the current parameter name
 */
char* setting_getParameterName() {
  char* returnValue = (char *) malloc(4);
  strcpy(returnValue, SETTING_PARAMETER_NAMES[setting_parameterIndex]);
  return returnValue;
}

/**
 * returns the numerical index of a parameter
 */
uint8_t setting_getParameterIndex() {
  return setting_parameterIndex;
}

/**
 * changes the current parameter up or down
 */
void setting_changeParameter(int32_t delta) {
  setting_parameterIndex = max(0, min(setting_parameterNum, setting_parameterIndex + delta));

  // set temporary parameter value to actual value of new parameter to be safe
  uint8_t seqId = display_getSeqId();

  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument
    setting_parameterValue_tmp = setting_parameterValue_Mchn;
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    setting_parameterValue_tmp = sequencer_getMidiNote(seqId) - SETTING_PARAMETER_VALUE_MNTE_MIN;
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    setting_parameterValue_tmp = setting_parameterValue_Mgte[seqId];
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    setting_parameterValue_tmp = setting_parameterValue_Mvel[seqId];
  } else if (setting_parameterIndex == 4) {  
    // SETTING_PARAMETER_NAME_SVOL - sequence volume
    setting_parameterValue_tmp = sequencer_getVolume(seqId);
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    setting_parameterValue_tmp = sequencer_getLen(seqId);
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = setting_parameterValue_Spul[seqId];
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SOFF - sequence offset
    setting_parameterValue_tmp = 0;
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    setting_parameterValue_tmp = setting_parameterValue_Sdir[seqId];
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length, we're storing an array index here, not the actual value
    //setting_parameterValue_Snln[seqId] = SETTING_PARAMETER_VALUE_SNLN_INV[sequencer_getNoteLen(seqId)];
    setting_parameterValue_tmp = setting_parameterValue_Snln[seqId];
  } else if (setting_parameterIndex == 10) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    setting_parameterValue_tmp = setting_parameterValue_Srda[seqId];
  } else if (setting_parameterIndex == 11) {  
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    setting_parameterValue_tmp = setting_parameterValue_Srdc[seqId];
  } 
}

/**
 * returns the current parameter value for display
 */
char* setting_getParameterValue() {
  char* returnValue = (char *) malloc(3);
  uint8_t seqId = display_getSeqId();

  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument
    sprintf(returnValue, "%3d", setting_parameterValue_Mchn);
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    strcpy(returnValue, SETTING_PARAMETER_VALUE_MIDINOTES[sequencer_getMidiNote(seqId) - SETTING_PARAMETER_VALUE_MNTE_MIN]);
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    sprintf(returnValue, "%3d", setting_parameterValue_Mgte[seqId]);
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    sprintf(returnValue, "%3d", setting_parameterValue_Mvel[seqId]);
  } else if (setting_parameterIndex == 4) {  
    // SETTING_PARAMETER_NAME_SVOL - sequence volume
    sprintf(returnValue, "%3d", 10 + 6 * sequencer_getVolume(seqId));  
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    sprintf(returnValue, "%3d", sequencer_getLen(seqId));
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    sprintf(returnValue, "%3d", setting_parameterValue_Spul[seqId]);
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SOFF - sequence offset
    sprintf(returnValue, "%3d", 0);
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SDIR[setting_parameterValue_Sdir[seqId]]);
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length - 16th, 8th, 4th
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SNLN[setting_parameterValue_Snln[seqId]]);
  } else if (setting_parameterIndex == 10) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    sprintf(returnValue, "%3d", setting_parameterValue_Srda[seqId]);
  } else if (setting_parameterIndex == 11) {  
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
  } else if (setting_parameterIndex == 4) {
    // SETTING_PARAMETER_NAME_SVOL - sequence volume
    sprintf(returnValue, "%3d", 10 + 6 * setting_parameterValue_tmp);
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    strcpy(returnValue, SETTING_PARAMETER_VALUE_SDIR[setting_parameterValue_tmp]);
  } else if (setting_parameterIndex == 9) {
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
  uint8_t seqId = display_getSeqId();
  
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
    // SETTING_PARAMETER_NAME_SVOL - sequence volume
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_SVOL_MAX, max(SETTING_PARAMETER_VALUE_SVOL_MIN, setting_parameterValue_tmp + delta));   
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    setting_parameterValue_tmp = min(32, max(0, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses - this is in the Euclidian Rythm sense
    setting_parameterValue_tmp = min(sequencer_getLen(seqId), max(0, setting_parameterValue_tmp + delta));    
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SOFF - sequence offset - this is in the Euclidian Rythm sense
    uint8_t offsetWidth = sequencer_getLen(seqId) - 1;
    setting_parameterValue_tmp = min(offsetWidth, max(-1 * offsetWidth, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_SDIR_MAX, max(SETTING_PARAMETER_VALUE_SDIR_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length, storing an array index here, not a value
    setting_parameterValue_tmp = min(SETTING_PARAMETER_VALUE_SNLN_MAX, max(SETTING_PARAMETER_VALUE_SNLN_MIN, setting_parameterValue_tmp + delta));
  } else if (setting_parameterIndex == 10) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    setting_parameterValue_tmp = min(100, max(0, setting_parameterValue_tmp + delta));  
  } else if (setting_parameterIndex == 11) {
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    setting_parameterValue_tmp = min(100, max(0, setting_parameterValue_tmp + delta));   
  }
}

/**
 * this persistently stores the parameter value in the tmp variable
 * to the actual storage, making the change effective.
 */
void setting_persistParameterValue() {
  uint8_t seqId = display_getSeqId();
  
  if (setting_parameterIndex == 0) {
    // SETTING_PARAMETER_NAME_MCHN - MIDI channel for the instrument
    setting_parameterValue_Mchn = setting_parameterValue_tmp;
  } else if (setting_parameterIndex == 1) {
    // SETTING_PARAMETER_NAME_MNTE - sequence MIDI note
    sequencer_setMidiNote(seqId, setting_parameterValue_tmp + SETTING_PARAMETER_VALUE_MNTE_MIN);
  } else if (setting_parameterIndex == 2) {
    // SETTING_PARAMETER_NAME_MGTE - sequence MIDI gate length
    setting_parameterValue_Mgte[seqId] = setting_parameterValue_tmp;
  } else if (setting_parameterIndex == 3) {
    // SETTING_PARAMETER_NAME_MVEL - sequence MIDI note velocity
    setting_parameterValue_Mvel[seqId] = setting_parameterValue_tmp;
  } else if (setting_parameterIndex == 4) {
    // SETTING_PARAMETER_NAME_SVOL - sequence volume
    sequencer_setVolume(seqId, setting_parameterValue_tmp);
  } else if (setting_parameterIndex == 5) {
    // SETTING_PARAMETER_NAME_SLEN - sequence length
    sequencer_setLen(seqId, setting_parameterValue_tmp);
  } else if (setting_parameterIndex == 6) {
    // SETTING_PARAMETER_NAME_SPUL - active sequence pulses 
    // persisting this will overwrite the existing pattern with an Euclidian Rythm 
    setting_parameterValue_Spul[seqId] = setting_parameterValue_tmp;
    sequencer_setSeq(seqId, euclid(sequencer_getLen(seqId), setting_parameterValue_Spul[seqId], 0));
  } else if (setting_parameterIndex == 7) {
    // SETTING_PARAMETER_NAME_SOFF - sequence offset
    sequencer_applyOffset(seqId, setting_parameterValue_tmp);
  } else if (setting_parameterIndex == 8) {
    // SETTING_PARAMETER_NAME_SDIR - sequence play direction - forward, backward, alternating
    setting_parameterValue_Sdir[seqId] = setting_parameterValue_tmp;
  } else if (setting_parameterIndex == 9) {
    // SETTING_PARAMETER_NAME_SNLN - sequence note length
    setting_parameterValue_Snln[seqId] = setting_parameterValue_tmp; // storing the array index
    sequencer_setNoteLen(seqId, SETTING_PARAMETER_VALUE_SNLN_VAL[setting_parameterValue_tmp]); // storing the clock intervals
  } else if (setting_parameterIndex == 10) {
    // SETTING_PARAMETER_NAME_SRDA - sequence randomization amount
    setting_parameterValue_Srda[seqId] = setting_parameterValue_tmp;
  } else if (setting_parameterIndex == 11) {
    // SETTING_PARAMETER_NAME_SRDC - sequence randomization chance
    setting_parameterValue_Srdc[seqId] = setting_parameterValue_tmp;
  }
}
