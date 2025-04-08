// CustomHID.h
#pragma once
#include <HID.h>

// Custom HID Descriptor for FFB
static const uint8_t _hidReportDescriptor[] PROGMEM = {
  // USAGE_PAGE (Generic Desktop)
  0x05, 0x01,
  // USAGE (Steering Wheel)
  0x09, 0x04,
  // COLLECTION (Application)
  0xA1, 0x01,

  // --- Input Report (Device → PC) ---
  // Steering Axis (16-bit)
  0x09, 0x30,        // USAGE (X)
  0x15, 0x00,        // LOGICAL_MIN (0)
  0x26, 0xFF, 0xFF,  // LOGICAL_MAX (65535)
  0x75, 0x10,        // REPORT_SIZE (16 bits)
  0x95, 0x01,        // REPORT_COUNT (1)
  0x81, 0x02,        // INPUT (Data,Var,Abs)

  // --- Output Report (PC → Device) ---
  // Force Feedback (PID Page)
  0x05, 0x0F,        // USAGE_PAGE (PID)
  0x09, 0x20,        // USAGE (Set Envelope)
  0x15, 0x00,        // LOGICAL_MIN (0)
  0x26, 0xFF, 0xFF,  // LOGICAL_MAX (65535)
  0x75, 0x10,        // REPORT_SIZE (16 bits)
  0x95, 0x01,        // REPORT_COUNT (1)
  0x91, 0x02,        // OUTPUT (Data,Var,Abs)

  // END_COLLECTION
  0xC0
};

// Override the default HID descriptor
extern const void* _hidReport;
extern const uint16_t _hidReportSize;
const void* _hidReport = _hidReportDescriptor;
const uint16_t _hidReportSize = sizeof(_hidReportDescriptor);