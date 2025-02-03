#include <Arduino.h>
#include <HID.h>

// Custom HID Report Descriptor (e.g., 4 buttons, 2 axes)
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

  // PADDING (4 unused bits to fill a full byte)
  0x75, 0x01,        // REPORT_SIZE (1 bit)
  0x95, 0x04,        // REPORT_COUNT (4 bits)
  0x81, 0x03,        // INPUT (Const,Var,Abs)

  // X AXIS (8 bits)
  0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
  0x09, 0x30,        // USAGE (X)
  0x15, 0x00,        // LOGICAL_MINIMUM (0)
  0x26, 0xFF, 0x00,  // LOGICAL_MAXIMUM (255)
  0x75, 0x08,        // REPORT_SIZE (8 bits)
  0x95, 0x01,        // REPORT_COUNT (1 field)
  0x81, 0x02,        // INPUT (Data,Var,Abs)

  // Y AXIS (8 bits)
  0x09, 0x31,        // USAGE (Y)
  0x15, 0x00,        // LOGICAL_MINIMUM (0)
  0x26, 0xFF, 0x00,  // LOGICAL_MAXIMUM (255)
  0x75, 0x08,        // REPORT_SIZE (8 bits)
  0x95, 0x01,        // REPORT_COUNT (1 field)
  0x81, 0x02,        // INPUT (Data,Var,Abs)

  // END COLLECTION
  0xC0,
  // END COLLECTION
  0xC0
};

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