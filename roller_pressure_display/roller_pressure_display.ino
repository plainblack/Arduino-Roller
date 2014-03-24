#include <SoftwareSerial.h>
#include <Encoder.h>

// rotary encoder
Encoder myEnc(5, 6); // pins


// 7 segment display
const int softwareTx = 8; // pin
const int softwareRx = 7;
SoftwareSerial s7s(softwareRx, softwareTx);
long pressure = -999;  // the initialized position of the roller
char tempString[10];  // Will be used with sprintf to create strings


void setup() {
  Serial.begin(9600);
  Serial.println("Roller Pressure Guage Initializing");
  s7s.begin(9600);
  setBrightness(255);  // High brightness

  // initialize
  clearDisplay();  // Clears display, resets cursor
  s7s.print("lets");  // Displays "lEtS roll"
  delay(2000);
  s7s.print("roll");  
  delay(2000);

  // Clear the display before jumping into loop
  clearDisplay();  
  Serial.println("Roller Pressure Guage Initialized");
}


void loop() {
  long newPosition = myEnc.read();
  newPosition = newPosition / 1024.0; // encoder has a 1024 resolution, so we divide to slow it down because we don't need that level of detail
  if (newPosition != pressure) {
    pressure = newPosition;
    // Magical sprintf creates a string for us to send to the s7s.
    //  The %4d option creates a 4-digit integer.
    sprintf(tempString, "%4d", pressure);
  
    // This will output the tempString to the S7S
    s7s.print(tempString);
    setDecimals(0b00000100);  // Sets digit 3 decimal on
  }
}


// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplay()
{
  s7s.write(0x76);  // Clear display command
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightness(byte value)
{
  s7s.write(0x7A);  // Set brightness command byte
  s7s.write(value);  // brightness data byte
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimals(byte decimals)
{
  s7s.write(0x77);
  s7s.write(decimals);
}
