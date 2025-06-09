#include <HID.h>
#include <math.h>

static const uint8_t _testDescriptor[] PROGMEM = {
  0x05, 0x01,        // USAGE_PAGE (Generic Desktop)
  0x09, 0x04,        // USAGE (Mouse)
  0xA1, 0x01,        // COLLECTION (Application)
    0x09, 0x01,      //   USAGE (Pointer)
    0xA1, 0x00,      //   COLLECTION (Physical)

      // 3 Buttons
      0x05, 0x09,    //     USAGE_PAGE (Button)
      0x19, 0x01,    //     USAGE_MINIMUM (Button 1)
      0x29, 0x08,    //     USAGE_MAXIMUM (Button 8)
      0x15, 0x00,    //     LOGICAL_MINIMUM (0)
      0x25, 0x01,    //     LOGICAL_MAXIMUM (1)
      0x95, 0x08,    //     REPORT_COUNT (8)
      0x75, 0x01,    //     REPORT_SIZE (1)
      0x81, 0x02,    //     INPUT (Data,Var,Abs)

      // X and Y movement
      0x05, 0x01,    //     USAGE_PAGE (Generic Desktop)
      0x09, 0x30,    //     USAGE (X)
      0x09, 0x31,    //     USAGE (Y)
      0x15, 0x81,    //     LOGICAL_MINIMUM (-127)
      0x25, 0x7F,    //     LOGICAL_MAXIMUM (127)
      0x75, 0x08,    //     REPORT_SIZE (8)
      0x95, 0x02,    //     REPORT_COUNT (2)
      0x81, 0x06,    //     INPUT (Data,Var,Rel)

    0xC0,            //   END_COLLECTION
  0xC0               // END_COLLECTION
};

static HIDSubDescriptor node(_testDescriptor, sizeof(_testDescriptor));

typedef struct {
  uint8_t reportId;
  uint8_t buttons;  
  int8_t  x;
  int8_t  y;
} report;

void setup() {
  HID().AppendDescriptor(&node);
  
}

double asdf = 0.0;

void loop() {
  double sinValue = sin(asdf);
  asdf += 0.05;  
  
  
  report rpt;
  rpt.reportId = 1;
  rpt.buttons = 1;
  rpt.x = (int8_t)(sinValue * 127);
  rpt.y = 0;
 
  HID().SendReport(2, &rpt, sizeof(rpt));
}
