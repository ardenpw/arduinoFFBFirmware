#include <Arduino.h>
#include <HID.h>
#include <Wire.h>
#include <AS5600.h>

// Updated HID Report Descriptor: 4 buttons, 2 axes (signed 8-bit values)
static const uint8_t _hidReportDescriptor[] PROGMEM = {
  // USAGE_PAGE (Generic Desktop)
  0x05, 0x01,
  // USAGE (Joystick)
  0x09, 0x04,
  // COLLECTION (Application)
  0xA1, 0x01,
    // COLLECTION (Physical)
    0xA1, 0x00,

      // BUTTONS (4 buttons, 1 bit each)
      0x05, 0x09,        // USAGE_PAGE (Button)
      0x19, 0x01,        // USAGE_MINIMUM (Button 1)
      0x29, 0x04,        // USAGE_MAXIMUM (Button 4)
      0x15, 0x00,        // LOGICAL_MINIMUM (0)
      0x25, 0x01,        // LOGICAL_MAXIMUM (1)
      0x75, 0x01,        // REPORT_SIZE (1 bit per button)
      0x95, 0x04,        // REPORT_COUNT (4 buttons)
      0x81, 0x02,        // INPUT (Data,Var,Abs)

      // PADDING (4 bits)
      0x75, 0x01,        // REPORT_SIZE (1 bit)
      0x95, 0x04,        // REPORT_COUNT (4 bits)
      0x81, 0x03,        // INPUT (Const,Var,Abs)

      // X AXIS (8 bits, signed)
      0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
      0x09, 0x30,        // USAGE (X)
      0x15, 0x80,        // LOGICAL_MINIMUM (-128)
      0x25, 0x7F,        // LOGICAL_MAXIMUM (127)
      0x75, 0x08,        // REPORT_SIZE (8 bits)
      0x95, 0x01,        // REPORT_COUNT (1 field)
      0x81, 0x02,        // INPUT (Data,Var,Abs)

      // Y AXIS (8 bits, signed)
      0x09, 0x31,        // USAGE (Y)
      0x15, 0x80,        // LOGICAL_MINIMUM (-128)
      0x25, 0x7F,        // LOGICAL_MAXIMUM (127)
      0x75, 0x08,        // REPORT_SIZE (8 bits)
      0x95, 0x01,        // REPORT_COUNT (1 field)
      0x81, 0x02,        // INPUT (Data,Var,Abs)

    // END COLLECTION (Physical)
    0xC0,
  // END COLLECTION (Application)
  0xC0
};

AS5600 as5600;  // AS5600 sensor object
const int BUTTON_PIN = 2; // Button pin
const int Y_PIN = A1;     // Y-axis analog pin

void setup() {
  Wire.begin();  // Start I2C
  if (!as5600.begin()) {
    while (true) { /* Handle connection failure */ }
  }
  as5600.setDirection(AS5600_CLOCK_WISE);  // Optional: Set rotation direction
  
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  HID().AppendDescriptor(&node);
}

// Update the report structure to use signed 8-bit values for axes
struct JoystickReport {
  uint8_t buttons; // 4 buttons (bits 0-3)
  int8_t x;        // X-axis (-128 to 127)
  int8_t y;        // Y-axis (-128 to 127)
};

void loop() {
  int rawAngle = as5600.getCumulativePosition(); 
  int8_t xAxis = map(rawAngle, 0, 4095, -128, 127);
  int8_t yAxis = (analogRead(Y_PIN) >> 2) - 128; 
  
  // Debug output
  Serial.print("Raw Angle: ");
  Serial.print(rawAngle);
  Serial.print(" -> Mapped x: ");
  Serial.println(xAxis);
  
  JoystickReport report = {
    .buttons = (digitalRead(BUTTON_PIN) == LOW) ? 0b0001 : 0b0000,
    .x = xAxis,
    .y = yAxis
  };

  HID().SendReport(0, &report, sizeof(report));
  delay(10);
}

