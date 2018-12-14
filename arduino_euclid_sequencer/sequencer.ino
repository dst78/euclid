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
 * M..V VVVN NNNN NNOO OOOO PPPP PPLL LLLL
 * . - represents an unused bit
 * M - contains the information whether the sequence is muted (HI = muted)
 * L - overall sequence length, 6-bit for up to 32 steps (technically we could store 0-63 but that makes no sense)
 * P - current sequence position, 0-31, 5-bit 
 * N - MIDI note (this is an indirect index. the actual note will be offset by the amount of SETTING_PARAMETER_VALUE_MNTE_MIN)
 * O - MIDI note length: (the numbers reflect the clock incrememnts)
 *     see the CLOCK_INCREMENT_* constants for actual values
 * V - sequence volume (used in audio output mode only). each increment raises the volume by ~6%
 */
uint32_t sequencer_seqInfo[8] = {
//  M....VVVVOOOOONNNNNNNPPPPPLLLLLL
  0b00000111100110010010000000001000,
  0b00000111100110010011000000000000,
  0b00000111100110010101000000000000,
  0b00000111100110010111000000000000,
  0b00000111100110010010100000000000,
  0b00000111100110100101100000000000,
  0b00000111100110011000000000000000,
  0b00000111100110010100100000000000,
};
//                                  M....VVVVOOOOONNNNNNNPPPPPLLLLLL
uint32_t sequencer_maskInfoMute = 0b10000000000000000000000000000000;
uint32_t sequencer_maskInfoLen  = 0b00000000000000000000000000111111;
uint32_t sequencer_maskInfoPos  = 0b00000000000000000000011111000000;
uint32_t sequencer_maskInfoMnte = 0b00000000000000111111100000000000;
uint32_t sequencer_maskInfoSnln = 0b00000000011111000000000000000000;
uint32_t sequencer_maskInfoSvol = 0b00000111100000000000000000000000;

/**
 * returns a sequence as 32 bit number
 */
uint32_t sequencer_getSeq(uint8_t seqId) {
  return sequencer_seqData[seqId];
}

/**
 * stores a complete sequence
 */
void sequencer_setSeq(uint8_t seqId, uint32_t seq) {
  sequencer_seqData[seqId] = seq;
}

/**
 * returns true if the sequence hits a pulse at the given position, false otherwise.
 */
boolean sequencer_getStep(uint8_t seqId, uint8_t seqPos) {
  return (sequencer_seqData[seqId] & (1 << (31 - seqPos)));
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
boolean sequencer_isMuted(uint8_t seqId) {
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
 * increases the position of the sequencer by one (wrapping around)
 * 
 * returns the new position
 */
uint8_t sequencer_incPos(uint8_t seqId) {
  uint8_t seqPos = sequencer_getPos(seqId) + 1;
  
  if (seqPos == sequencer_getLen(seqId)) {
    seqPos = 0;
  }
  
  sequencer_setPos(seqId, seqPos);

  return seqPos;
}

/**
 * takes a time increment from the clock and
 * advances every sequence if needed. also triggers MIDI notes / samples.
 */
void sequencer_handlePositionIncrement(uint8_t increment) {
  uint8_t snln; // sequence note length
  uint8_t seqPos, oldPos; 
  
  for (uint8_t seqId = 0; seqId < 8; seqId++) {

    if (!sequencer_isMuted(seqId)
        && sequencer_getLen(seqId) > 0) {
      snln = sequencer_getNoteLen(seqId);
    
      if (increment == snln) {
        oldPos = sequencer_getPos(seqId);
        seqPos = sequencer_incPos(seqId);

        if (oldPos != seqPos) {
          // on alternating direction the first and last step is triggered twice so the cursor doesn't always move
          //display_updateSequenceCurrentPosition(seqId, oldPos, seqPos);
        }
        
        if (sequencer_getStep(seqId, seqPos)) {
          midi_sendNoteOn(seqId);
        }
        
      } else if (increment >= snln / 2) {
        seqPos = sequencer_getPos(seqId);
  
        if (sequencer_getStep(seqId, seqPos)) {
          // switch note off again after half the interval
          // @todo replace this with proper gate calculation
          midi_sendNoteOff(seqId);
        }
      }
    }
  }
}

/**
 * updates the sequence offset
 * only the lowest 6 bits will be taken into account
 */
void sequencer_applyOffset(uint8_t seqId, int8_t seqOff) {
  uint8_t seqLen    = sequencer_getLen(seqId);
  // rotating bitshift sequence 
  sequencer_seqData[seqId] = sequencer_rotate(sequencer_seqData[seqId], seqLen, seqOff);
}

/**
 * rotates the given pattern, taking into account the actual length
 */
uint32_t sequencer_rotate(uint32_t pattern, uint8_t len, int8_t rotation) {
  if (rotation > 0) {
    pattern = ((pattern >> rotation) | (pattern << (len - rotation))) & (0xFFFFFFFF << (32 - len));
    
  } else if (rotation < 0) {
    rotation *= -1;
    pattern = ((pattern << rotation) | (pattern >> (len - rotation))) & (0xFFFFFFFF << (32 - len));
  }

  return pattern;
}

/**
 * returns the indirect MIDI note index
 */
uint8_t sequencer_getMidiNote(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoMnte) >> 11;
}

/**
 * updates the indirect MIDI note index
 */
void sequencer_setMidiNote(uint8_t seqId, uint8_t mnte) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoMnte) | ((mnte << 11) & sequencer_maskInfoMnte);
}

/**
 * returns the MIDI note len for the given sequence
 */
uint8_t sequencer_getNoteLen(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoSnln) >> 18;
}

/**
 * updates the sequence with the given MIDI note length
 */
void sequencer_setNoteLen(uint8_t seqId, uint8_t clockIntervals) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoSnln) | ((clockIntervals << 18) & sequencer_maskInfoSnln);
}

/**
 * returns the raw volume information (a 16th of 100% with each interval)
 */
uint8_t sequencer_getVolume(uint8_t seqId) {
  return (sequencer_seqInfo[seqId] & sequencer_maskInfoSvol) >> 23;
}

/**
 * sets the sequence volume. only the lowest four bits will be used
 */
void sequencer_setVolume(uint8_t seqId, uint8_t vol) {
  sequencer_seqInfo[seqId] = (sequencer_seqInfo[seqId] & ~sequencer_maskInfoSvol) | ((vol << 23) & sequencer_maskInfoSvol);
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

/**
 * returns the binary length of a number by counting bitwise
 * 
 * this is faster than calculating (uint8_t) log2(bnry) + 1
 */
uint8_t sequencer_findLength(uint32_t bnry) {
  // no number can have a length of zero - single 0 has a length of one, but no 1s for the sytem to count
  uint8_t length = 1;
  
  for (uint8_t q = 32; q >= 0; q--) {  
    if (bitRead(bnry, q) == 1) {
      length = q + 1;
      break;
    }
  }
  
  return length;
}


#if DEBUG_SEQUENCER
void sequence_debug(uint32_t seq) {
  for (uint8_t i = 0; i < 32; i++) {
    if ((seq & (1 << (31-i))) == (uint32_t) (1 << (31-i))) {
      // active step
      Serial.print("X");
      
    } else {
      // inactive step
      Serial.print("_");
    }
  }

  Serial.println();
}
#endif
