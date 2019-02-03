/**
 * Contains the handler routines for the thumbstick.
 * 
 * Assumes a 2-axis thumbstick with analog x/y output and a single pushbutton. 
 */

#define THUMBSTICK_PIN_X   A8 // analog pin
#define THUMBSTICK_PIN_Y   A9 // analog pin
#define THUMBSTICK_PIN_BTN 9  // digital pin w/ interrupt support

static const uint32_t THUMBSTICK_REFRESH_INTERVAL = 250; // ms
static uint32_t thumbstick_lastRefresh;

// tweak these to the values of your thumbstick
static const uint16_t thumbstick_centerX = 820;  // analog value when the stick is x-axis centered
static const uint16_t thumbstick_centerY = 820;  // analog value when the stick is y-axis centered
static const uint16_t thumbstick_limitL  = 1020; // value must be above this value to count as left command
static const uint16_t thumbstick_limitR  = 600;  // value must be below this value to count as right command
static const uint16_t thumbstick_limitU  = 1020; // value must be above this value to count as up command
static const uint16_t thumbstick_limitD  = 600;  // value must be below this value to count as down command

/**
 * call in setup()
 */
void thumbstick_init() {
  thumbstick_lastRefresh = 0;
  
  pinMode(THUMBSTICK_PIN_BTN, INPUT);
  attachInterrupt(digitalPinToInterrupt(THUMBSTICK_PIN_BTN), thumbstick_btnHandler, FALLING);
}

/**
 * call in loop()
 */
void thumbstick_update() {
  uint32_t now = millis();
  int16_t thumbX, thumbY;
  #if DEBUG_THUMBSTICK
  boolean dbg = false;
  #endif

  if (now - thumbstick_lastRefresh >= THUMBSTICK_REFRESH_INTERVAL) {
    thumbstick_lastRefresh = now;
      
    thumbX = analogRead(THUMBSTICK_PIN_X);
    thumbY = analogRead(THUMBSTICK_PIN_Y);

    if (thumbX > thumbstick_limitL) {
      display_moveCursorLeft();
      #if DEBUG_THUMBSTICK
      dbg = true;
      #endif
      
    } else if (thumbX < thumbstick_limitR) {
      display_moveCursorRight();
      #if DEBUG_THUMBSTICK
      dbg = true;
      #endif
      
    } else if (thumbY > thumbstick_limitU) {
      display_moveCursorUp();
      #if DEBUG_THUMBSTICK
      dbg = true;
      #endif
      
    } else if (thumbY < thumbstick_limitD) {
      display_moveCursorDown();
      #if DEBUG_THUMBSTICK
      dbg = true;
      #endif
    }

    #if DEBUG_THUMBSTICK
    if (dbg) {
      Serial.print("thumbstick ");
      Serial.print(thumbX);
      Serial.print("/");
      Serial.println(thumbY);
    }
    #endif
  }
}

/**
 * interrupt handler routine for the thumbstick pushbutton
 */
void thumbstick_btnHandler() {
  display_toggleEditMode();
  
  #if DEBUG_THUMBSTICK
  Serial.println("thumbstick pushbutton was pressed");
  #endif
}
