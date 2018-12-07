/**
 * Contains the data structures and functions to create and manipulate sequences.
 * 
 * The instrument contains a total of 8 sequences, with a maximum step length of 32.
 * 
 * Each sequence is represented as a single uint32_t bit-field where every bit represents
 * a single step with HI being active steps. 
 * 
 * In addition to that the following additional information is stored. This is stored
 * as a single uint16_t bit-field: .....MPPPPPLLLLL
 * - . - represents an unused bit
 * - M - contains the information whether the sequence is muted (HI = muted)
 * - PPPPP - current sequence position 
 * - LLLLL - overall sequence length 
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
uint16_t sequencer_seqInfo[8] = {
  0x0015,
  0x0010,
  0x0008,
  0x0000,
  0x0000,
  0x0000,
  0x0000,
  0x0000,
};

uint16_t sequencer_maskInfoMute = 0b0000010000000000;
uint16_t sequencer_maskInfoPos  = 0b0000001111100000;
uint16_t sequencer_maskInfoLen  = 0b0000000000011111;

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
 */
uint16_t sequencer_getLen(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoLen);
}

/**
 * returns the current position in the sequence
 */
uint16_t sequencer_getPos(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoPos) >> 5;
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
