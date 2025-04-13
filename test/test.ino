#include <Arduino.h>
#include <PluggableUSB.h>

#include "HIDDescriptor.h"

// try and make a gamepad with buttons and axis

typedef struct{
  uint8_t reportID;
  uint8_t buttons;
  int16_t xAxis;
  int16_t yAxis;
} HIDBallsREPORT_t;

int main(){
  for(;;);
}


/*
const int BUTTON_PIN = 2; // Declare the button pin
const int X_PIN = A0;     // Declare the X-axis pin
const int Y_PIN = A1;     // Declare the Y-axis pin

void setup() {
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

struct JoystickReport {
  uint8_t buttons; // 4 buttons (bits 0-3)
  uint8_t x;       // X-axis (0-255)
  uint8_t y;       // Y-axis (0-255)
};

void loop() {
  JoystickReport report = {
    .buttons = (digitalRead(BUTTON_PIN) == LOW) ? 0b0001 : 0b0000, // Read the button state
    .x = analogRead(X_PIN) >> 2, // Convert 10-bit ADC to 8-bit
    .y = analogRead(Y_PIN) >> 2
  };

  HID().SendReport(1, &report, sizeof(report));
  delay(10);
}
*/

