/**
 * implementing MIDI-over-USB for now, until the hardware rests in its enclosure
 */

/**
 * call this in setup()
 */
void midi_init() {
  
}

void midi_sendNoteOn(uint8_t seqId) {
  if (setting_getOutput() == SETTING_OUTPUT_MIDI) {
    usbMIDI.sendNoteOn(sequencer_getMidiNote(seqId), setting_getMidiVelocity(seqId), setting_getMidiChannel());
    #if DEBUG_MIDI
    Serial.print("sent note on for sequencer "); Serial.println(seqId);
    #endif
  }
}

void midi_sendNoteOff(uint8_t seqId) {
  if (setting_getOutput() == SETTING_OUTPUT_MIDI) {
    usbMIDI.sendNoteOff(sequencer_getMidiNote(seqId), setting_getMidiVelocity(seqId), setting_getMidiChannel());
    #if DEBUG_MIDI
    Serial.print("sent note off for sequencer "); Serial.println(seqId);
    #endif
  }
}
