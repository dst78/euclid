
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

// include the library code:
#include <LiquidCrystal.h>
#include <Encoder.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
         // LCD pins: RS  RW  EN  D4 D5 D6 D7

Encoder rotary(6, 7);
long encoderPos = 0;
const int rotaryButton = 8;
int posX = 2;
int posY = 1;

int thumbstickX = 9;
int thumbstickY = 8;
int thumbstickButton = 9;

byte arrowUp[8] = {
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b00100,
  0b00100,
  0b00000
};
byte arrowDown[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte arrowRight[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};
byte arrowLeft[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01100,
  0b11111,
  0b01100,
  0b00100,
  0b00000
};
byte stepActive[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
byte stepInactive[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b00100,
  0b00000,
  0b00000
};
byte stepCurrent[8] = {
  0b00000,
  0b00100,
  0b00100,
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b00100
};

const int ledPin = 13;
int i = 0;
  
void setup() {
  Serial.begin(9600);
  Serial.println("Encoder Test:");
  
  pinMode(ledPin, OUTPUT);
  pinMode(rotaryButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(rotaryButton), rotaryButtonHandler, FALLING);

  pinMode(thumbstickButton, INPUT);
  attachInterrupt(digitalPinToInterrupt(thumbstickButton), thumbstickButtonHandler, FALLING);

  // store custom chars
  lcd.createChar(0, arrowUp);
  lcd.createChar(1, arrowRight);
  lcd.createChar(2, arrowDown);
  lcd.createChar(3, arrowLeft);
  lcd.createChar(4, stepActive);
  lcd.createChar(5, stepInactive);
  lcd.createChar(6, stepCurrent);
  // set up the LCD's number of rows and columns: 
  lcd.begin(20, 4);
  lcd.clear();  
  lcd.blink();

  lcd.setCursor(0, 0);
  lcd.write((uint8_t)1);
  lcd.write(" C120/50   mode val");

  lcd.setCursor(0, 1);
  lcd.write("3|"); lcd.write((uint8_t)5); lcd.write((uint8_t)6); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write("|"); lcd.write((uint8_t)0);

  lcd.setCursor(0, 2);
  lcd.write("4"); lcd.write((uint8_t)3); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)6); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)4); lcd.write((uint8_t)1);

  lcd.setCursor(0, 3);
  lcd.write("5|"); lcd.write((uint8_t)5); lcd.write((uint8_t)6); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write((uint8_t)5); lcd.write("|          "); lcd.write((uint8_t)2);

  lcd.setCursor(posX, posY);
}

void loop() {
  long newEncoderPos;
  long thumbX, thumbY;
  newEncoderPos = rotary.read();

  if (newEncoderPos <= -2) {
    posX = max(2, posX-1);
    encoderPos = newEncoderPos;
    lcd.setCursor(posX, posY);
    rotary.write(0);
    Serial.print("moved left, new pos: ");
    Serial.println(encoderPos);
  }
  if (newEncoderPos >= 2) {
    posX = min(17, posX+1);
    encoderPos = newEncoderPos;
    lcd.setCursor(posX, posY);
    rotary.write(0);
    Serial.print("moved right, new pos: ");
    Serial.println(encoderPos);
  }

  thumbX = analogRead(thumbstickX);
  thumbY = analogRead(thumbstickY);
  
  /*
  Serial.print("thumbstick ");
  Serial.print(thumbX);
  Serial.print("/");
  Serial.println(thumbY);
  */
}

void rotaryButtonHandler() {
  Serial.println("rotary pushbutton was pressed");
}

void thumbstickButtonHandler() {
  Serial.println("thumbstick pushbutton was pressed");
}
