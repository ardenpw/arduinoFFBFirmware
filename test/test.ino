#include <Arduino.h>
#include <HID.h>
#include <math.h>

#include "FFBDescriptor.h"
#include "test.h"

void setup(){
  static HIDSubDescriptor node(_testDescriptor, sizeof(_testDescriptor));
  HID().AppendDescriptor(&node);
}

void loop(){
  
}
 
/*
#define HID_REPORT_DESCRIPTOR_TYPE 0x22

A class that inherits from PluggableUSBModule
Implementation of required virtual methods:

setup()
getInterface()
getDescriptor()

USB descriptor definitions
Endpoint management
*/
/*
// try and make a gamepad with buttons and axis

typedef struct{
  uint8_t reportID;
  uint8_t buttons;
  int16_t xAxis;
  int16_t yAxis;
} HIDBallsREPORT_t;

class MyHIDDevice : public PluggableUSBModule{
  public:
    MyHIDDevice(uint8_t* epType = nullptr) : PluggableUSBModule(1, 1, epType ? epType : defaultEpType) {
      PluggableUSB().plug(this);
    }

    bool setup(USBSetup& setup){
      return false;
    }

    int getInterface(uint8_t* interfaceCount){
      *interfaceCount += 1;
      uint8_t descriptorData[] = {
        0x05,        // bLength
        0x01,        // bDescriptorType (Device)
        0x09, 0x05,  // bcdUSB 5.09
        0xA1,        // bDeviceClass 

        0x01,        // bLength
        0xA1,        // bLength
        0x00,        // bDescriptorType (Undefined)
        0x85, 0x01, 0x05, 0x09, 0x19, 0x01, 0x29, 0x03, 0x15, 0x00, 0x25, 0x01, 0x95, 0x03, 0x75, 0x01, 0x81, 0x02, 0x95, 0x01, 0x75, 0x05, 0x81, 0x03, 0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x02, 0x81, 0x06, 0xC0, 0xC0, 
        // 50 bytes
      };
      return USB_SendControl(0, descriptorData, sizeof(descriptorData));
    }

    int getDescriptor(USBSetup& setup){
      if (setup.wValueH == HID_REPORT_DESCRIPTOR_TYPE) {
        return USB_SendControl(TRANSFER_PGM, _hidReportDescriptor, sizeof(_hidReportDescriptor));
      }
      return 0;
    }

    uint8_t getShortName(char* name) {
      strcpy(name, "MyHID");
      return 5;
    }
    
    void sendReport(uint8_t* data, int length) {
      USB_Send(pluggedEndpoint, data, min(length, 16));
    }
    
  protected:
    uint8_t defaultEpType[1] = {EP_TYPE_INTERRUPT_IN};
};

MyHIDDevice aaaa;

void setup(){
  Serial.begin(9600);
}

void loop(){
  uint8_t data[16] = {0};
  // Fill data
  aaaa.sendReport(data, 16);
  delay(1000);
}
*/

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

