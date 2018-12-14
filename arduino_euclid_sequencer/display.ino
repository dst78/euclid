/*
 * Handles the 20x4 LCD display
 * 
 * Contains information about the menu structure in the top-row
 * as well as functions to generate the visual representation of the 
 * visible part of a sequence in a single row based on the uint32_t data
 * structure.
 */

#include <LiquidCrystal.h>
#include "display_customChars.h"
#include "constants.h"

#define LCD_PIN_RS 12
#define LCD_PIN_EN 11
#define LCD_PIN_D4 5
#define LCD_PIN_D5 4
#define LCD_PIN_D6 3
#define LCD_PIN_D7 2
// Initialize the library with the interface pins
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

// splashscreen duration in ms
#define DISPLAY_SPLASHSCREEN_DURATION 500

// position of the visible cursor when a menu segment is active
#define DISPLAY_MENU_CURSORPOS_OUTPUT 1
#define DISPLAY_MENU_CURSORPOS_CLOCK 3
#define DISPLAY_MENU_CURSORPOS_BPM 5
#define DISPLAY_MENU_CURSORPOS_SWING 9
#define DISPLAY_MENU_CURSORPOS_PARAMETER_NAME 12
#define DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE 17

// position where the menu segement begins (irrespective of where the visible cursor will be)
#define DISPLAY_MENU_POS_SEQUENCE 0
#define DISPLAY_MENU_POS_OUTPUT 1
#define DISPLAY_MENU_POS_CLOCK 3
#define DISPLAY_MENU_POS_BPM 4
#define DISPLAY_MENU_POS_SWING 8
#define DISPLAY_MENU_POS_PARAMETER_NAME 12
#define DISPLAY_MENU_POS_PARAMETER_VALUE 17

// Current cursor position
uint8_t display_cursorX, display_cursorY;
// Stored cursor position in the menu row - used in display_moveCursorToMenu() 
// these are display (!) positions. Y will always be 0, X is locked to certain steps
uint8_t display_menuPosX = DISPLAY_MENU_CURSORPOS_CLOCK;
uint8_t display_menuPosY = 0;
// Stored cursor position in the sequence section - used in display_moveCursorToSeq()
// these are display (!) positions, not sequence offsets. rows are 1-3, cols are 2-17
uint8_t display_seqPosX = 2; 
uint8_t display_seqPosY = 1;
// The instrument has 8 sequencers but the display only has 3 rows to display them, this variable defines the offset.
uint8_t display_seqRowOffset = 0;
// For long sequences the display row needs to be scrolled horizontally. This stores that offset.
uint8_t display_seqColOffset[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Indicators for every sequence: (K)ick, (S)nare, closed (h)ihat, open (H)ihat, (R)imshot, (C)lave, high (T)om, low (t)om
char display_seqIndicator[8] = {
  DISPLAY_SEQINDICATOR_KICK, 
  DISPLAY_SEQINDICATOR_SNARE, 
  DISPLAY_SEQINDICATOR_HIHAT_CLOSED, 
  DISPLAY_SEQINDICATOR_HIHAT_OPEN, 
  DISPLAY_SEQINDICATOR_RIMSHOT, 
  DISPLAY_SEQINDICATOR_CLAVE, 
  DISPLAY_SEQINDICATOR_TOM_HI, 
  DISPLAY_SEQINDICATOR_TOM_LO
};
// cached value
uint8_t display_seqId;

/**
 * call this in setup()
 */
void display_init() {
  // store custom chars
  lcd.createChar(0, display_charArrowUp);
  lcd.createChar(1, display_charArrowRight);
  lcd.createChar(2, display_charArrowDown);
  lcd.createChar(3, display_charArrowLeft);
  lcd.createChar(4, display_charStepActive);
  lcd.createChar(5, display_charStepActiveCurrent);
  lcd.createChar(6, display_charStepInactive);
  lcd.createChar(7, display_charStepInactiveCurrent);
  // set up the LCD's number of rows and columns: 
  lcd.begin(20, 4);

  display_splashScreen();
  delay(DISPLAY_SPLASHSCREEN_DURATION);
  display_gui();
}

/**
 * call this after splashScreen()
 */
void display_gui() {
  lcd.clear();  
  lcd.cursor();

  display_updateMenu();
  display_updateSequenceRow(0);
  display_updateSequenceRow(1);
  display_updateSequenceRow(2);
  display_moveCursorToMenu();
}

void display_splashScreen() {
  lcd.clear();
  lcd.noCursor();
  
  lcd.setCursor(3, 0);
  lcd.print("Anoikis Nomads");
  lcd.setCursor(1, 1);
  lcd.print("Euclidian Sequencer");
  lcd.setCursor(6, 3);
  lcd.print("v ");
  lcd.print(VERSION);
}

void display_updateMenu() {
  display_updateMenuPartSequence(true);  
  display_updateMenuPartOutput(false); // lcd.write(setting_getOutput());
  display_updateMenuPartClocksource(true); // lcd.write(clocksource);
  display_updateMenuPartBPM(false);
  display_updateMenuPartSwing(true);
  
  display_updateMenuPartParameterName(true); 
  display_updateMenuPartParameterValue(true);
  
  lcd.setCursor(display_cursorX, display_cursorY);
}

/**
 * updates the sequence indicator in the menu
 */
void display_updateMenuPartSequence(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_SEQUENCE, 0);
  }
  
  lcd.write(display_seqIndicator[display_getSeqId()]);
}

/**
 * updates the output type indicator part in the menu
 */
void display_updateMenuPartOutput(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_OUTPUT, 0);
  }
  lcd.write(setting_getOutput());
}

/**
 * updates the clocksource indicator part of the menu
 */
void display_updateMenuPartClocksource(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_CLOCK, 0);
  }
  
  lcd.write(setting_getClocksource());
}

/**
 * updates the BPM indicator part of the menu
 */
void display_updateMenuPartBPM(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_BPM, 0);
  }
  
  if (setting_getClocksource() == SETTING_CLOCKSOURCE_INTERNAL) {
    lcd.write(setting_getBPM());
  } else {
    lcd.write("   ");
  }  
}

/**
 * updates the swing setting part of the menu
 */
void display_updateMenuPartSwing(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_SWING, 0);
  }
  
  lcd.write("s");
  lcd.write(setting_getSwing());
}

/**
 * updates the parameter name display part of the menu
 */
void display_updateMenuPartParameterName(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_PARAMETER_NAME, 0);
  }
  
  lcd.write(setting_getParameterName());
}

/**
 * updates the parameter value display part of the menu
 */
void display_updateMenuPartParameterValue(bool reposition) {
  if (reposition) {
    lcd.setCursor(DISPLAY_MENU_POS_PARAMETER_VALUE, 0);
  }
  
  if (display_menuPosX != DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE
      || display_cursorY != 0) {
    // show stored value when not editing the parameter value
    lcd.write(setting_getParameterValue());
  } else {
    // show temporary value when editing the parameter value
    lcd.write(setting_getParameterTmpValue());
  }
}

/**
 * calculates the sequenceId underneath the display cursor
 */
void display_calculateSeqId() { 
  display_seqId = display_seqRowOffset + display_seqPosY - 1;
}

/**
 * returns the cached id of the sequence underneath the display cursor
 */
uint8_t display_getSeqId() {
  return display_seqId;
}

/**
 * calculates the sequence step underneath the display cursor
 */
uint8_t display_calculateSeqStep() {
  return display_seqColOffset[display_getSeqId()] + display_seqPosX - 2;
}

/**
 * updates the complete display row for a sequence.
 * 
 * row indicates the physical sequence row on the display (0-2), NOT the seqId!
 */
void display_updateSequenceRow(uint32_t row) {
  uint8_t i;
  uint8_t seqId  = display_seqRowOffset + row;
  uint8_t endPos = display_seqColOffset[seqId] + 16;
  uint32_t seq   = sequencer_getSeq(seqId);
  uint8_t seqPos = sequencer_getPos(seqId);
  uint8_t seqLen = sequencer_getLen(seqId);

  lcd.setCursor(0, row+1);

  // sequence indicator
  lcd.print(display_seqIndicator[seqId]);

  // pipe if at the beginning of the sequence, arrowLeft otherwise
  if (display_seqColOffset[seqId] == 0) {
    lcd.print("|");
  } else {
    lcd.write((uint8_t) 3);
  }

  // display the sequence itself
  for (i = display_seqColOffset[seqId]; i < endPos; i++) {  
    if (i < seqLen) {
      if ((seq & (1 << (31-i))) == (uint32_t) (1 << (31-i))) {
        if (i == seqPos) {
          // current step, active
          lcd.write((uint8_t) 5);
        } else {
          // active step
          lcd.write((uint8_t) 4);
        }
        
      } else {
        if (i == seqPos) {
          // current step, inactive
          lcd.write((uint8_t) 7);
        } else {
          // inactive step
          lcd.write((uint8_t) 6);
        }
      }
      
    } else {
      // behind sequence, clear character
      lcd.print(" ");
    }
  }

  // pipe if sequence doesn't extend visible space, arrowRight otherwise
  if (i >= sequencer_getLen(seqId)) {
    lcd.print("|");
  } else {
    lcd.write((uint8_t) 1);
  }

  // up/down arrow if applicable
  if (row == 0 && display_seqRowOffset > 0) {
    lcd.write((uint8_t) 0);
  } else if (row == 2 && display_seqRowOffset < 5) {
    lcd.write((uint8_t) 2);
  } else {
    lcd.print(" ");
  }
}

/**
 * re-draws the sequence step underneath the cursor
 */
void display_updateSequenceStep() {
  uint8_t seqId   = display_getSeqId();
  uint8_t seqStep = display_calculateSeqStep();

  display_updateSequenceStep(seqId, seqStep);

  #if DEBUG_DISPLAY
  Serial.println("--------------------");
  Serial.print("updated sequence ");
  Serial.print(seqId);
  Serial.print(" step ");
  Serial.println(seqStep);
  Serial.println("new sequence: ");
  uint32_t seq = sequencer_getSeq(seqId);
  uint16_t seqLen = sequencer_getLen(seqId);
  for (int i = 0; i < seqLen; i++) {
    Serial.print(".");
  }
  Serial.println("");

    #if DEBUG_SEQUENCER
    sequence_debug(seq);
    #endif
  #endif  
}

/**
 * re-draws the sequence step as given by absolute coordinates
 */
void display_updateSequenceStep(uint8_t seqId, uint8_t seqStep) {
  // translate sequence and sequence step to display coordinates
  uint8_t displayRow = seqId - display_seqRowOffset + 1;
  uint8_t displayCol = seqStep - display_seqColOffset[seqId] + 2;
  uint32_t seq   = sequencer_getSeq(seqId);
  uint8_t seqPos = sequencer_getPos(seqId);
  
  // only update the LCD when coordinates are visible
  if (displayRow >= 1 && displayRow < 4
    && displayCol >= 2 && displayCol < 18) {
      
    #if DEBUG_DISPLAY
    Serial.println("character is visible");
    #endif
      
    lcd.setCursor(displayCol, displayRow);
    
    if ((seq & (1 << (31-seqStep))) == (uint32_t) (1 << (31-seqStep))) {
      if (seqStep == seqPos) {
        // current step, active
        lcd.write((uint8_t) 5);
      } else {
        // active step
        lcd.write((uint8_t) 4);
      }
      
    } else {
      if (seqStep == seqPos) {
        // current step, inactive
        lcd.write((uint8_t) 7);
      } else {
        // inactive step
        lcd.write((uint8_t) 6);
        
      }
    }

    // put the cursor back on the same step
    lcd.setCursor(display_cursorX, display_cursorY);
  }
}

/**
 * if visible, moves the current position cursor of a sequence
 */
void display_updateSequenceCurrentPosition(uint8_t seqId, uint8_t oldPos, uint8_t newPos) {
  #if DEBUG_SEQUENCER
  Serial.print("updating moving position for sequence "); Serial.println(seqId);
  #endif
  display_updateSequenceStep(seqId, oldPos);
  display_updateSequenceStep(seqId, newPos);
}

/**
 * toggles between sequence editing mode and menu mode
 */
void display_toggleEditMode() {
  if (display_cursorY == 0) {
    lcd.setCursor(0, 0);
    lcd.write((uint8_t) 2);
    display_moveCursorToSeq();
    
  } else {
    display_moveCursorToMenu();
  }
}

/**
 * returns 'm' when in menu mode, 's' if in sequence editing mode
 */
char display_getEditMode() {
  return (display_cursorY == 0) ? DISPLAY_EDITMODE_MENU : DISPLAY_EDITMODE_GRID;
}

/**
 * enters menu mode
 */
void display_moveCursorToMenu() {
  display_cursorX = display_menuPosX;
  display_cursorY = display_menuPosY;
  lcd.setCursor(display_cursorX, display_cursorY);
}

/**
 * enters sequence editing mode
 */
void display_moveCursorToSeq() {
  display_cursorX = display_seqPosX;
  display_cursorY = display_seqPosY;
  
  display_updateMenuPartSequence(true);
  display_updateMenuPartParameterValue(true);
  
  lcd.setCursor(display_cursorX, display_cursorY);
}

/**
 * moves to the previous sequence row
 */
void display_moveCursorUp() {
  uint8_t newOffset = display_seqRowOffset;

  // sequence row
  if (display_seqPosY > 1) {
    // sequencer rows
    display_seqPosY = max(1, display_seqPosY - 1);
    
    display_calculateSeqId();
    display_updateMenuPartSequence(true);
    display_updateMenuPartParameterValue(true);
      
  } else if (display_seqPosY == 1) {
    newOffset = max(0, display_seqRowOffset - 1);
      
    if (display_seqRowOffset != newOffset) {
      display_seqRowOffset = newOffset;
      
      display_calculateSeqId();
      display_updateMenuPartSequence(true);
      display_updateMenuPartParameterValue(true);
      display_updateSequenceRow(0);
      display_updateSequenceRow(1);
      display_updateSequenceRow(2);
    }
  }

  // make sure cursor isn't further right than sequence length
  uint8_t seqId = display_getSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  display_seqPosX = max(2, min(min(1+seqLen, 17), display_seqPosX));
  display_cursorX = display_seqPosX;
  display_cursorY = display_seqPosY;
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * moves to the next lowest sequence row
 */
void display_moveCursorDown() {
  uint8_t newOffset = display_seqRowOffset;

  // sequence row
  if (display_seqPosY < 3) {
    // sequencer rows
    display_seqPosY = min(3, display_seqPosY + 1);
    
    display_calculateSeqId();
    display_updateMenuPartSequence(true);
    display_updateMenuPartParameterValue(true);
    
  } else if (display_seqPosY == 3) {
    newOffset = min(5, display_seqRowOffset+1);
    
    if (display_seqRowOffset != newOffset) {
      display_seqRowOffset = newOffset;
      
      display_calculateSeqId();
      display_updateMenuPartSequence(true);
      display_updateMenuPartParameterValue(true);
      display_updateSequenceRow(0);
      display_updateSequenceRow(1);
      display_updateSequenceRow(2);
    }
  }

  // make sure cursor isn't further right than sequence length
  display_calculateSeqId();
  uint8_t seqId = display_getSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  display_seqPosX = max(2, min(min(1+seqLen, 17), display_seqPosX));
  display_cursorX = display_seqPosX;
  display_cursorY = display_seqPosY;
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * handles encoder rotation. negative values for delta indicate counterclockwise
 * movement, positive values indicate clockwise movement.
 */
void display_encoderMove(int32_t delta) {
  uint8_t seqId = display_getSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  
  if (display_cursorY == 0) {
    // menu row
    if (display_menuPosX == DISPLAY_MENU_CURSORPOS_OUTPUT) {
      setting_changeOutput(delta);
      display_updateMenuPartOutput(true);
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_CLOCK) {
      setting_changeClocksource(delta);
      display_updateMenuPartClocksource(true);
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_BPM) {
      setting_changeBPM(delta);
      display_updateMenuPartBPM(true);
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_SWING) {
      setting_changeSwing(delta);
      display_updateMenuPartSwing(true);
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_NAME) {
      // parameter
      setting_changeParameter(delta);
      display_updateMenuPartParameterName(true);
      display_updateMenuPartParameterValue(true);
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE) {
      // parameter value
      setting_changeParameterValue(delta);
      display_updateMenuPartParameterValue(true);
    }
    
  } else {
    // sequence row
    if (delta < 0
        && display_seqPosX == 2
        && display_seqColOffset[seqId] > 0) {
      // can scroll left
      display_seqColOffset[seqId]--;
      display_updateSequenceRow(display_cursorY-1);
      
    } else if (delta < 0 
        && display_seqPosX > 2) {
      // can move left
      display_seqPosX = max(2, display_seqPosX-1);
      display_cursorX = display_seqPosX;
      
    } else if (delta > 0
        && display_seqPosX == 17 
        && seqLen - display_seqColOffset[seqId] > 16) {
      // can scroll right
      display_seqColOffset[seqId]++;
      display_updateSequenceRow(display_seqPosY-1);      
      
    } else if (delta > 0
        && display_seqPosX < 17) {
      // can move right
      display_seqPosX = min(min(1+seqLen, 17), display_seqPosX+1);
      display_cursorX = display_seqPosX;
    }
  }
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * handles the encoder button press
 */
void display_encoderButtonPress() {
  if (display_cursorY == 0) {
    if (display_menuPosX == DISPLAY_MENU_CURSORPOS_OUTPUT) {
      // output was changed, hop to input position
      display_menuPosX = DISPLAY_MENU_CURSORPOS_CLOCK;
      display_cursorX = display_menuPosX;
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_CLOCK
        && setting_getClocksource() == SETTING_CLOCKSOURCE_INTERNAL) {
      // clock source was changed to internal, hop to BPM position
      display_menuPosX = DISPLAY_MENU_CURSORPOS_BPM;
      display_cursorX = display_menuPosX;
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_NAME) {
      // menu parameter name was selected, move cursor to parameter value
      display_menuPosX = DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE;
      display_cursorX = display_menuPosX;
      
    } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE) {
      // menu parameter value needs to be stored
      setting_persistParameterValue(); 
      // update sequence row only when needed
      uint8_t parameterIndex = setting_getParameterIndex();
      if (parameterIndex >= 5 
          && parameterIndex <= 7) {
        display_updateSequenceRow(display_seqPosY-1);
      }
    }  
    
    lcd.setCursor(display_cursorX, display_cursorY);
  }
}

/**
 * jumps the cursor between predefined positions in the menu row
 */
void display_moveCursorLeft() {  
  // menu row
  display_cursorY = display_menuPosY;

  if (display_menuPosX == DISPLAY_MENU_CURSORPOS_OUTPUT) {
    // currently at output, wrap around
    display_menuPosX = DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_CLOCK) {
    // currently at clock source
    display_menuPosX = DISPLAY_MENU_CURSORPOS_OUTPUT;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_BPM) {
    // currently at BPM
    display_menuPosX = DISPLAY_MENU_CURSORPOS_CLOCK;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_SWING) {
    // currently at swing value
    if (setting_getClocksource() == SETTING_CLOCKSOURCE_CV) {
      display_menuPosX = DISPLAY_MENU_CURSORPOS_CLOCK;
    } else {
      display_menuPosX = DISPLAY_MENU_CURSORPOS_BPM;
    }
    
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_NAME) {
    // currently at parameter
    display_menuPosX = DISPLAY_MENU_CURSORPOS_SWING;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE) {
    // currently at parameter value
    display_menuPosX = DISPLAY_MENU_CURSORPOS_PARAMETER_NAME;
  }
  
  display_cursorX = display_menuPosX;
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * jumps the cursor between predefined positions in the menu row
 */
void display_moveCursorRight() {  
  // menu row
  display_cursorY = display_menuPosY;

  if (display_menuPosX == DISPLAY_MENU_CURSORPOS_OUTPUT) {
    // currently at output
    display_menuPosX = DISPLAY_MENU_CURSORPOS_CLOCK;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_CLOCK) {
    // currently at clock source
    if (setting_getClocksource() == SETTING_CLOCKSOURCE_CV) {
      display_menuPosX = DISPLAY_MENU_CURSORPOS_SWING; 
    } else {
      display_menuPosX = DISPLAY_MENU_CURSORPOS_BPM;
    }
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_BPM) {
    // currently at BPM
    display_menuPosX = DISPLAY_MENU_CURSORPOS_SWING;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_SWING) {
    // currently at swing value
    display_menuPosX = DISPLAY_MENU_CURSORPOS_PARAMETER_NAME;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_NAME) {
    // currently at parameter
    display_menuPosX = DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE;
  } else if (display_menuPosX == DISPLAY_MENU_CURSORPOS_PARAMETER_VALUE) {
    // currently at parameter value, wrap around
    display_menuPosX = DISPLAY_MENU_CURSORPOS_OUTPUT;
  }
  
  display_cursorX = display_menuPosX;
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

#if DEBUG_DISPLAY
void display_positionDebug() {
  uint8_t seqId = display_getSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  Serial.println("--------------");
  Serial.print("offsets: "); Serial.print(display_seqColOffset[seqId]); Serial.print(" / "); Serial.println(display_seqRowOffset);
  Serial.println("new positions:");
  Serial.print("cursor: "); Serial.print(display_cursorX); Serial.print(" / "); Serial.println(display_cursorY);
  Serial.print("menu  : "); Serial.print(display_menuPosX); Serial.print(" / "); Serial.println(display_menuPosY);
  Serial.print("grid  : "); Serial.print(display_seqPosX); Serial.print(" / "); Serial.println(display_seqPosY);
  Serial.print("seq   : "); Serial.print(seqId); Serial.print(" pos: "); Serial.print(display_calculateSeqStep()); Serial.print(" len: "); Serial.println(seqLen);
}
#endif
