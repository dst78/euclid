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

#define LCD_PIN_RS 12
#define LCD_PIN_EN 11
#define LCD_PIN_D4 5
#define LCD_PIN_D5 4
#define LCD_PIN_D6 3
#define LCD_PIN_D7 2
// Initialize the library with the interface pins
LiquidCrystal lcd(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

// Current cursor position
uint8_t display_cursorX, display_cursorY;
// Stored cursor position in the menu row - used in display_moveCursorToMenu() 
// these are display (!) positions. Y will always be 0, X is locked to certain steps
uint8_t display_menuPosX = 2;
uint8_t display_menuPosY = 0;
// Stored cursor position in the sequence section - used in display_moveCursorToSeq()
// these are display (!) positions, not sequence offsets. rows are 1-3, cols are 2-17
uint8_t display_seqPosX = 3; 
uint8_t display_seqPosY = 1;
// The instrument has 8 sequencers but the display only has 3 rows to display them, this variable defines the offset.
uint8_t display_seqRowOffset = 0;
// For long sequences the display row needs to be scrolled horizontally. This stores that offset.
uint8_t display_seqColOffset[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
// Indicators for every sequence: (K)ick, (S)nare, closed (H)ihat, open (h)ihat, (R)imshot, (C)lave, high (T)om, low (t)om
char display_seqIndicator[8] = {'K', 'S', 'h', 'H', 'R', 'C', 'T', 't'};

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
  lcd.createChar(5, display_charStepInactive);
  lcd.createChar(6, display_charStepCurrent);
  // set up the LCD's number of rows and columns: 
  lcd.begin(20, 4);
  lcd.clear();  
  lcd.cursor();
  //lcd.blink();

  display_updateMenu();
  display_updateSequenceRow(0);
  display_updateSequenceRow(1);
  display_updateSequenceRow(2);
  display_moveCursorToMenu();
}

void display_updateMenu() {
  lcd.setCursor(0, 0);
  lcd.write((uint8_t)1);
  lcd.write(" C120/50   mode val");
}

/**
 * calculates the sequenceId underneath the display cursor
 */
uint8_t display_calculateSeqId() { 
  return display_seqRowOffset + display_seqPosY - 1;
}

/**
 * calculates the sequence step underneath the display cursor
 */
uint8_t display_calculateSeqStep() {
  return display_seqColOffset[display_calculateSeqId()] + display_seqPosX - 2;
}

/**
 * updates the complete display row for a sequence.
 * 
 * row indicates the physical sequence row on the display (0-2), NOT the seqId!
 */
void display_updateSequenceRow(uint32_t row) {
  uint8_t i, iRel;
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
  iRel = 0;
  
  for (i = display_seqColOffset[seqId]; i < endPos; i++) {  
    if (iRel < seqLen) {
      if (iRel == seqPos) {
        // current position
        lcd.write((uint8_t) 6);
        
      } else if ((seq & (1 << iRel)) == (uint32_t) (1 << iRel)) {
        // active step
        lcd.write((uint8_t) 4);
        
      } else {
        // inactive step
        lcd.write((uint8_t) 5);
      }
      
    } else {
      // behind sequence, clear character
      lcd.print(" ");
    }
    iRel++;
  }

  // pipe if sequence doesn't extend visible space, arrowRight otherwise
  if (iRel >= sequencer_getLen(seqId)) {
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
  uint8_t seqId   = display_calculateSeqId();
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
  
  for (int i = 0; i < 32; i++) {
    if ((seq & (1 << (31-i))) == (uint32_t) (1 << (31-i))) {
      // active step
      Serial.print("X");
      
    } else {
      // inactive step
      Serial.print("_");
    }
  }
  Serial.println("");
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
    if (seqStep == seqPos) {
      // current position
      lcd.write((uint8_t) 6);
      
    } else if ((seq & (1 << (31-seqStep))) == (uint32_t) (1 << (31-seqStep))) {
      // active step
      lcd.write((uint8_t) 4);
      
    } else {
      // inactive step
      lcd.write((uint8_t) 5);
    }

    // put the cursor back on the same step
    lcd.setCursor(display_cursorX, display_cursorY);
  }
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
    lcd.setCursor(0, 0);
    lcd.write((uint8_t) 1);
    display_moveCursorToMenu();
  }
}

/**
 * returns 'm' when in menu mode, 's' if in sequence editing mode
 */
char display_getEditMode() {
  return (display_cursorY == 0) ? 'm' : 's';
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
  lcd.setCursor(display_cursorX, display_cursorY);
}

/**
 * moves the cursor up one notch
 * 
 * what this implies differs in sequence editing and menu mode
 */
void display_moveCursorUp() {
  uint8_t newOffset = display_seqRowOffset;

  if (display_cursorY == 0) {
    // menu row
    
  } else if (display_cursorY > 1) {
    // sequencer rows
    display_seqPosY = max(1, display_seqPosY - 1);
    display_cursorY = display_seqPosY;
    lcd.setCursor(display_cursorX, display_cursorY);
    
  } else if (display_cursorY == 1) {
    // seqRowOffset is zero-based, display_seqPosY starts at 1 so there is no "-1" calculation
    newOffset = max(0, display_seqRowOffset - 1);
      
    if (display_seqRowOffset != newOffset) {
      display_seqRowOffset = newOffset;
      display_updateSequenceRow(0);
      display_updateSequenceRow(1);
      display_updateSequenceRow(2);    
      
      lcd.setCursor(display_cursorX, display_cursorY);
    }
  }

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * moves the cursor down one notch
 * 
 * what this implies differs in sequence editing and menu mode
 */
void display_moveCursorDown() {
  uint8_t newOffset = display_seqRowOffset;
  
  if (display_cursorY == 0) {
    // menu row
  } else if (display_cursorY < 3) {
    // sequencer rows
    display_seqPosY = min(3, display_seqPosY + 1);
    display_cursorY = display_seqPosY;
    lcd.setCursor(display_cursorX, display_cursorY);
    
  } else if (display_cursorY == 3) {
    newOffset = min(5, display_seqRowOffset+1);
    
    if (display_seqRowOffset != newOffset) {
      display_seqRowOffset = newOffset;
      display_updateSequenceRow(0);
      display_updateSequenceRow(1);
      display_updateSequenceRow(2);

      lcd.setCursor(display_cursorX, display_cursorY);
    }
  }

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * handles encoder rotation. negative values for delta indicate counterclockwise
 * movement, positive values indicate clockwise movement.
 */
void display_encoderMove(int32_t delta) {
  uint8_t seqId = display_calculateSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  
  if (display_cursorY == 0) {
    // menu row
  } else {
    // sequence rows
    display_seqPosX = max(2, min(min(1+seqLen, 17), display_seqPosX+delta));
    display_cursorX = display_seqPosX;
  }
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * menu row jumps the cursor between predefined positions
 * sequence rows go step by step but not all the way left/right
 */
void display_moveCursorLeft() {
  if (display_cursorY == 0) {
    // menu row
    if (display_menuPosX == 2) {
      // currently at clock source, do nothing
      // @todo add horizontal scrolling
    } else if (display_menuPosX == 4) {
      // currently at BPM
      display_menuPosX = 2;
    } else if (display_menuPosX == 7) {
      // currently at swing value
      display_menuPosX = 4;
    } else if (display_menuPosX == 13) {
      // currently at mode
      display_menuPosX = 7;
    } else if (display_menuPosX == 18) {
      // currently at mode value
      display_menuPosX = 13;
    }
    display_cursorX = display_menuPosX;
    
  } else {
    display_seqPosX = max(2, display_seqPosX-1);
    display_cursorX = display_seqPosX;
  }
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

/**
 * menu row jumps the cursor between predefined positions
 * sequence rows go step by step but not all the way left/right
 */
void display_moveCursorRight() {
  uint8_t seqId = display_calculateSeqId();
  uint8_t seqLen = sequencer_getLen(seqId);
  
  if (display_cursorY == 0) {
    // menu row
    if (display_menuPosX == 2) {
      // currently at clock source
      display_menuPosX = 4;
    } else if (display_menuPosX == 4) {
      // currently at BPM
      display_menuPosX = 7;
    } else if (display_menuPosX == 7) {
      // currently at swing value
      display_menuPosX = 13;
    } else if (display_menuPosX == 13) {
      // currently at mode
      display_menuPosX = 18;
    } else if (display_menuPosX == 18) {
      // currently at mode value, do nothing
      // @todo add horizontal scrolling
    }
    display_cursorX = display_menuPosX;
    
  } else {
    display_seqPosX = min(min(1+seqLen, 17), display_seqPosX+1);
    display_cursorX = display_seqPosX;
  }
  
  lcd.setCursor(display_cursorX, display_cursorY);

  #if DEBUG_DISPLAY
  display_positionDebug();
  #endif
}

#if DEBUG_DISPLAY
void display_positionDebug() {
  Serial.println("--------------");
  Serial.println("new positions:");
  Serial.print("cursor: "); Serial.print(display_cursorX); Serial.print(" / "); Serial.println(display_cursorY);
  Serial.print("menu  : "); Serial.print(display_menuPosX); Serial.print(" / "); Serial.println(display_menuPosY);
  Serial.print("grid  : "); Serial.print(display_seqPosX); Serial.print(" / "); Serial.println(display_seqPosY);
  Serial.print("seq   : "); Serial.print(display_calculateSeqId()); Serial.print(" "); Serial.println(display_calculateSeqStep());
}
#endif
