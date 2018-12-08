
/*
  LiquidCrystalFast Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystalFast
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD WR pin to digital pin 10
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 25 July 2009
 by David A. Mellis
 
 http://www.pjrc.com/teensy/td_libs_LiquidCrystal.html
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */
#define DEBUG true
#define DEBUG_CLOCK true
#define DEBUG_DISPLAY true
#define DEBUG_ENCODER true
#define DEBUG_SEQUENCER true
#define DEBUG_THUMBSTICK true
#define DEBUG_SETTING true

  
void setup() {
  #if DEBUG || DEBUG_DISPLAY || DEBUG_ENCODER || DEBUG_SEQUENCER || DEBUG_THUMBSTICK
  Serial.begin(9600);
  #endif

  clock_init();
  sequencer_init();
  encoder_init();
  thumbstick_init();
  display_init();  
}

void loop() {
  encoder_update();
  thumbstick_update();
}
