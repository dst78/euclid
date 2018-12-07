# Euclidian Rythm Drum Sequencer
drum sequencer based on a Teensy 3.6

**At this time the whole project is in a very bare bones state probably isn't even working. If you're looking for a functional module then I'm afraid you'll have to come back at a later time.**

## Features
- selectable MIDI-out channel
- clock: internal or volca-style pulse from a 3.5mm jack
- globally adjustable swing

## Details on sequences
- freely selectable sequence length
  - maximum length limited only by hardware
- active steps 
  - defined by euclidian rythm
  - setting a new euclidian rythm clears "overlap" if previous sequence was longer
  - (DONE) individual steps can be toggled as well (traditional sequencing)
- sequence offset can be set
- whole sequence can be muted without clearing
- sequence step direction: forward, backward, alternating
- sequence speed: 1/16, 1/8, 1/4, triplets on/off
- selectable gate length per step
- selectable velocity per step
- selectable MIDI note per sequence
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
- pattern editing, live editing modes
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
      - moves the cursor one step backward/forward in the current sequence row
      - scrolls the visible section of long sequences if cursor is in leftmost/rightmost position
    - pushbutton
      - moves to setting editing
  - setting editing:
    - left/right
      - moves cursor between clock source, BPM, swing, mode, mode value
    - up/down
      - moves between sub-menu options if available
    - pushbutton
      - moves to grid editing

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
