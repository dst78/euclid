# Euclidian Rythm Drum Sequencer
drum sequencer based on a Teensy 3.6

**At this time the whole project is in a very bare bones state probably isn't even working. If you're looking for a functional module then I'm afraid you'll have to come back at a later time.**

## Features
- (DONE) selectable MIDI-out channel
- clock: 
  - (DONE) internal
  - volca-style pulse from a 3.5mm jack
- globally adjustable swing

## Details on sequences
- (DONE) freely selectable sequence length
- active steps 
  - (DONE) defined by euclidian rythm
  - (DONE) setting a new euclidian rythm clears "overlap" if previous sequence was longer
  - (DONE) individual steps can be toggled as well (traditional sequencing)
- (DONE) sequence offset can be set
- (DONE) whole sequence can be muted without clearing
- sequence step direction: forward, backward, alternating
- (DONE) sequence speed: 1/16, 1/16 triplets, 1/8, 1/8 triplets 1/4, 1/4 triplets
- selectable gate length per sequence
- (DONE) selectable velocity per sequence
- (DONE) selectable MIDI note per sequence
- randomization
  - amount of randomization per sequence
  - chance of randomization per sequence
- number of sequences limited only by hardware
  - (DONE) preconfigured indicator for every sequence
  - logical usages:
    - kick            K
    - snare           S
    - closed hihat    h
    - open hihat      H
    - ride cymbal     R
    - crash cymbal    C
    - high tom        T
    - low tom         t
    - percussion(s)   ...
- (DONE) pattern editing, live editing modes
- live triggering (playing a step once without changing the stored pattern)
    
## Details on the display
20x4 LCD display to display things

```
  → C120/50   mode val   // status line: clock source BPM/swing, mode indicator, mode-dependent value
  3|∙X◆∙∙∙◆∙░∙◆∙∙∙◆∙|↑   // 16-step sequence, up-arrow indicates vertical scrolling
  4←∙◆∙X◆∙∙∙◆∙∙◆∙∙∙◆→_   // long sequence, left/right arrows indicate horizontal scrolling
  5|∙X∙∙∙∙|__________↓   // 6-step sequence, down-arrow indicates vertical scrolling
```  
  top-left character displays → when in setting editing, ↓ when in grid editing mode
    
  legend:
  - clock sources: C = CV, I = internal
  - 1-n number of the sequence. alternatively, show sequence indicator instead of index
  - _   empty cell on the display
  - ○   inactive step
  - ●   active step
  - X   current step (when sequencer is running)
  - ░   cursor position, maybe try to create a pencil icon? blinks with what is underneath
  - ↑   display isn't showing the first sequence and can be scrolled up
  - ↓   display isn't showing the last sequence and can be scrolled down
  - ←   sequence row isn't shown from the beginning and can be scrolled left
  - →   sequence row isn't shown until the end and can be scrolled right

## Details on the usability concept
### toggle switch for clock source
  - internal (will make the module send MIDI clock)
  - CV (Korg Volca style pulses)
  - MIDI (disables start/stop/continue buttons and reads those from MIDI along with clock)

### debounced buttons
  - MIDI start / pause
  - MIDI stop
  - track mute

### controller-style two-axis joystick with pushbutton
  - grid editing:
    - up/down 
      - moves the cursor between sequence rows
      - scrolls the sequences in the display if the cursor is in first/last visible row
    - left/right 
      - jumps back to menu row and moves between menu items
    - pushbutton
      - moves to setting editing
  - setting editing:
    - left/right
      - moves cursor between clock source, BPM, swing, mode, mode value
    - up/down
      - makes the previous / next sequence row active for editing
    - pushbutton
      - moves to grid editing without changing the active sequence

### rotary encoder with pushbutton
  - grid editing:
    - rotation
      - moves between sequence steps, same as joystick left/right
    - pushbutton
      - toggles active/inactive step
  - setting editing:
    - rotation
      - moves through values of currently selected option
    - pushbutton
      - registers new value of currently selected option
      
## hardware
- 1x teensy 3.6
- 1x teensy audio shield?
- 2x 3.5mm stereo jack                for clock-in
                                      for audio out (mono'd)
- 1x two-way switch                   on/off switch
- 1x MIDI DIN jack
- 1x thumbstick with pushbutton
- 1x rotary encoder w/ pushbutton
- 1x 20x4 digit LCD display
- 1x eurorack connector port 
- 3x pushbutton                       current track mute
                                      clock start / pause
                                      clock stop
