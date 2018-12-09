/**
 * the euclidian rythm generator
 * 
 * The implementation is roughly based on the "Simplest Euclidean Rythm algorithm" from Computer Music Design:
 * http://www.computermusicdesign.com/simplest-euclidean-rhythm-algorithm-explained/
 * 
 * The implementation of rotation etc was adapted to suit the environment.
 */

uint32_t euclid(uint8_t steps, uint8_t pulses, uint8_t offsets) {
  uint32_t pattern = 0;
  uint8_t bucket  = 0;

  for (uint8_t i = 0; i < steps; i++) {
    bucket += pulses;

    if (bucket >= steps) {
      bucket -= steps;
      // pulse
      pattern |= (1 << (31-i));
      
    } else if (bucket < steps) {
      // no pulse
      pattern &= ~(1 << (31-i));
    }
  }

  // rotate so that highest bit is a pulse
  uint8_t euclidianWidth = sequencer_findLength(pattern);
  int8_t rotation = -32 + euclidianWidth + offsets;

  #if DEBUG_SEQUENCER
  Serial.println("----------------------------");
  Serial.print("pattern length: "); Serial.println(steps);
  Serial.print("pattern pulses: "); Serial.println(pulses);
  Serial.print("pattern offset: "); Serial.println(offsets);
  Serial.print("orig. pattern : "); sequence_debug(pattern);
  Serial.print("euclid width  : "); Serial.println(euclidianWidth);
  Serial.print("rotation      : "); Serial.println(rotation);
  #endif
  
  if (rotation != 0) {
    pattern = sequencer_rotate(pattern, steps, rotation);
  }

  #if DEBUG_SEQUENCER
  if (rotation != 0) {
    Serial.print("rot. pattern  : "); sequence_debug(pattern);
  }
  #endif

  return pattern;
}
