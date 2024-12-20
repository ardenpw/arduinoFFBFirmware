/*
  Modified by Matthew Heironimus to support HID Report Descriptors to be in 
  standard RAM in addition to program memory (PROGMEM).

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

  Permission to use, copy, modify, and/or distribute this software for
  any purpose with or without fee is hereby granted, provided that the
  above copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
  WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR
  BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
  OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
  WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
  SOFTWARE.
 */

#ifndef DYNAMIC_HID_h
#define DYNAMIC_HID_h

#include <stdint.h>
#include <Arduino.h>

#ifdef _VARIANT_ARDUINO_DUE_X_
  #define USB_EP_SIZE 64
  #define TRANSFER_PGM 0x80
  #include "USB/PluggableUSB.h"
  #define USB_Available USBD_Available
  #define USB_Recv USBD_Recv
  #define USB_RecvControl USBD_RecvControl
  #define _delay_us delayMicroseconds
#else
  #include "PluggableUSB.h"
#endif

#include "PIDReportHandler.h"

#if defined(USBCON)

#define _USING_DYNAMIC_HID

#define DYNAMIC_HID_GET_REPORT        0x01
#define DYNAMIC_HID_GET_IDLE          0x02
#define DYNAMIC_HID_GET_PROTOCOL      0x03
#define DYNAMIC_HID_SET_REPORT        0x09
#define DYNAMIC_HID_SET_IDLE          0x0A
#define DYNAMIC_HID_SET_PROTOCOL      0x0B

#define DYNAMIC_HID_DESCRIPTOR_TYPE         0x21
#define DYNAMIC_HID_REPORT_DESCRIPTOR_TYPE      0x22
#define DYNAMIC_HID_PHYSICAL_DESCRIPTOR_TYPE    0x23

#define DYNAMIC_HID_SUBCLASS_NONE 0
#define DYNAMIC_HID_SUBCLASS_BOOT_INTERFACE 1

#define DYNAMIC_HID_PROTOCOL_NONE 0
#define DYNAMIC_HID_PROTOCOL_KEYBOARD 1
#define DYNAMIC_HID_PROTOCOL_MOUSE 2

#define DYNAMIC_HID_BOOT_PROTOCOL	0
#define DYNAMIC_HID_REPORT_PROTOCOL	1

#define DYNAMIC_HID_REPORT_TYPE_INPUT   1
#define DYNAMIC_HID_REPORT_TYPE_OUTPUT  2
#define DYNAMIC_HID_REPORT_TYPE_FEATURE 3

#define PID_ENPOINT_COUNT 2

#define PID_ENDPOINT_IN	 (pluggedEndpoint)
#define PID_ENDPOINT_OUT (pluggedEndpoint+1)

extern TEffectState g_EffectStates[MAX_EFFECTS];

typedef struct
{
  uint8_t len;      // 9
  uint8_t dtype;    // 0x21
  uint8_t addr;
  uint8_t versionL; // 0x101
  uint8_t versionH; // 0x101
  uint8_t country;
  uint8_t desctype; // 0x22 report
  uint8_t descLenL;
  uint8_t descLenH;
} DYNAMIC_HIDDescDescriptor;

typedef struct 
{
  InterfaceDescriptor hid;
  DYNAMIC_HIDDescDescriptor   desc;
  EndpointDescriptor  in;
  EndpointDescriptor  out;
} DYNAMIC_HIDDescriptor;

class DynamicHIDSubDescriptor {
public:
  DynamicHIDSubDescriptor *next = NULL;
  DynamicHIDSubDescriptor(const void *d, const uint16_t l, const void* pid_d, const uint16_t pid_l, const bool ipm = true) : data(d), length(l),pid_data(pid_d), pid_length(pid_l), inProgMem(ipm) { }

  const void* data;
  const void* pid_data;
  const uint16_t length;
  const uint16_t pid_length;
  const bool inProgMem;
};

class DynamicHID_ : public PluggableUSBModule
{
public:
  DynamicHID_(void);
  int begin(void);
  bool usb_Available();
  int SendReport(uint8_t id, const void* data, int len);
  int RecvData(byte* data);
  void RecvfromUsb();
  void AppendDescriptor(DynamicHIDSubDescriptor* node);
  void DisableFFB(TEffectState* effectStates);
  PIDReportHandler pidReportHandler;

protected:
  int getInterface(uint8_t* interfaceCount);
  int getDescriptor(USBSetup& setup);
  bool GetReport(USBSetup& setup);
  bool SetReport(USBSetup& setup);
  bool setup(USBSetup& setup);
  uint8_t getShortName(char* name);

private:
  #if defined(__AVR_ATmega32U4__)
    uint8_t epType[2];
  #elif defined(__SAM3X8E__)
    uint32_t epType[2];
  #endif
  uint8_t out_ffbdata[64];
  DynamicHIDSubDescriptor* rootNode;
  uint16_t descriptorSize;

  uint8_t protocol;
  uint8_t idle;
};

DynamicHID_& DynamicHID();

#define D_HIDREPORT(length) { 9, 0x21, 0x11, 0x01, 0, 1, 0x22, lowByte(length), highByte(length) }

#endif // USBCON
#endif // DYNAMIC_HID_h
