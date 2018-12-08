/**
 * Contains the data structures and functions to create and manipulate sequences.
 * 
 * The instrument contains a total of 8 sequences, with a maximum step length of 32.
 * 
 * Each sequence is represented as a single uint32_t bit-field where every bit represents
 * a single step with HI being active steps. 
 */

uint32_t sequencer_seqData[8] = {
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
};

// information about mute-state, position and length
/**
 * Additional data related to a sequence. This is a 32-bit field, divided up as follows
 * M... ...N NNNN NNOO OOOO PPPP PPLL LLLL
 * - . - represents an unused bit
 * - M - contains the information whether the sequence is muted (HI = muted)
 * - L - overall sequence length, 6-bit for up to 32 steps (technically we could store 0-63 but that makes no sense)
 * - P - current sequence position, 0-31, 5-bit 
 * - O - play offset, 0-31, 5-bit
 * - N - MIDI note (this is an indirect index. the actual note will be offset by the amount of SETTING_PARAMETER_VALUE_MNTE_MIN)

 */
uint32_t sequencer_seqInfo[8] = {
//  M........NNNNNNNOOOOOPPPPPLLLLLL
  0b00000000001001000000000000001000,
  0b00000000001001100000000000000000,
  0b00000000001010100000000000000000,
  0b00000000001011100000000000000000,
  0b00000000001001010000000000000000,
  0b00000000010010110000000000000000,
  0b00000000001100000000000000000000,
  0b00000000001010010000000000000000,
};
//                                  M........NNNNNNNOOOOOPPPPPLLLLLL
uint32_t sequencer_maskInfoMute = 0b10000000000000000000000000000000;
uint32_t sequencer_maskInfoLen  = 0b00000000000000000000000000111111;
uint32_t sequencer_maskInfoPos  = 0b00000000000000000000011111000000;
uint32_t sequencer_maskInfoOff  = 0b00000000000000001111100000000000;
uint32_t sequencer_maskInfoMNte = 0b00000000011111110000000000000000;

/**
 * returns a sequence as 32 bit number
 */
uint32_t sequencer_getSeq(uint8_t seqId) {
  return sequencer_seqData[seqId];
}

/**
 * toggles the mute status of a sequence.
 */
void sequencer_muteToggleSeq(uint8_t seqId) {
  sequencer_seqInfo[seqId] ^= sequencer_maskInfoMute;
}

/**
 * returns true if the current sequence is muted
 */
boolean sequencer_getMuteStatus(uint8_t seqId) {
  return ((sequencer_seqInfo[seqId] & sequencer_maskInfoMute) == sequencer_maskInfoMute);
}

/**
 * returns the sequence length
 * return value will never be larger than 32
 */
uint8_t sequencer_getLen(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoLen);
}

/**
 * updates the sequence length with the new value
 * only the lowest 6 bits will be taken into account
 */
void sequencer_setLen(uint8_t seqId, uint8_t seqLen) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoLen) | (seqLen & sequencer_maskInfoLen);
}

/**
 * returns the current position in the sequence
 * return value will never be larger than 32
 */
uint8_t sequencer_getPos(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoPos) >> 6;
}

/**
 * updates the current position in the sequence
 * only the lowest 6 bits will be taken into account
 */
void sequencer_setPos(uint8_t seqId, uint8_t seqPos) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoPos) | ((seqPos << 6) & sequencer_maskInfoPos);
}

/**
 * returns the sequence offset
 * return value will never be larger than 32
 */
uint8_t sequencer_getOffset(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoOff) >> 11;
}

/**
 * updates the sequence offset
 * only the lowest 6 bits will be taken into account
 */
void sequencer_setOffset(uint8_t seqId, uint8_t seqOff) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoOff) | ((seqOff << 11) & sequencer_maskInfoOff);
}

/**
 * returns the indirect MIDI note index
 */
uint8_t sequencer_getMidiNote(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoMNte) >> 16;
}

/**
 * updates the indirect MIDI note index
 */
void sequencer_setMidiNote(uint8_t seqId, uint8_t mnte) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoMNte) | ((mnte << 16) & sequencer_maskInfoMNte);
}

/**
 * toggles a single step in a sequence between active and inactive.
 * 
 * returns true if successful
 */
bool sequencer_toggleSeqStep(uint8_t seqId, uint8_t seqStep) {
  bool returnValue = false;
  
  if (seqId >= 0 
      && seqId < 8 
      && seqStep < sequencer_getLen(seqId)) {
        
    sequencer_seqData[seqId] ^= (1 << (31-seqStep));
    returnValue = true;
    
    #if DEBUG_SEQUENCER
    Serial.print("toggling step ");
    Serial.print(seqStep);
    Serial.print(" in sequence ");
    Serial.println(seqId);
    #endif
  }

  return returnValue;
}
