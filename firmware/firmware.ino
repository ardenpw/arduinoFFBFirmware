#include "Joystick.h"
#include "DigitalWriteFast.h"
#include "AS5600.h"

// Check which board we're using
#if defined(__AVR_ATmega32U4__) // Leonardo
  #define USING_LEONARDO
#elif defined(__SAM3X8E__) // Due
  #define USING_DUE
#else
  #error "This code is designed for Arduino Leonardo or Due only"
#endif

AS5600 as5600; 

#define encoderPinA 2
#define encoderPinB 3

#define motorPinA 7
#define motorPinB 8
#define motorPinPWM 9

#define ENCODER_MAX_VALUE 4095
#define ENCODER_MIN_VALUE -4095

#define MAX_PWM 1023

bool isOutOfRange = false;
int32_t forces[2] = {0};
Gains gains[2];
EffectParams effectparams[2];

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
  8, 0,                  // Button Count, Hat Switch Count
  true, true, false,     // X and Y, but no Z Axis
  true, true, false,     // Rx, Ry, no Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

volatile long value = 0;
int32_t g_force = 0;

int32_t currentPosition = 0;
volatile int8_t oldState = 0;
const int8_t KNOBDIR[] = {
  0, 1, -1, 0,
  -1, 0, 0, 1,
  1, 0, 0, -1,
  0, -1, 1, 0
};

int32_t customEncoderOutput = 0;

void setCustomEncoderOutput(int32_t newValue) {
  customEncoderOutput = constrain(newValue, ENCODER_MIN_VALUE, ENCODER_MAX_VALUE);
}

void setup() {
  Serial.begin(115200);                        
  Joystick.setRyAxisRange(0, 500);
  Joystick.setRxAxisRange(0, 500);
  Joystick.setYAxisRange(0, 500);
  Joystick.setXAxisRange(ENCODER_MIN_VALUE, ENCODER_MAX_VALUE);
  Joystick.setGains(gains);
  Joystick.begin(true);

  Wire.begin();

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.

  pinMode(motorPinA, OUTPUT);
  pinMode(motorPinB, OUTPUT);
  pinMode(motorPinPWM, OUTPUT);

  pinMode(A0, INPUT_PULLUP);
  
  setupTimer();
}

void setupTimer() {
  #ifdef USING_LEONARDO
    cli();
    TCCR3A = 0; //set TCCR1A 0
    TCCR3B = 0; //set TCCR1B 0
    TCNT3  = 0; //counter init
    OCR3A = 399;
    TCCR3B |= (1 << WGM32); //open CTC mode
    TCCR3B |= (1 << CS31); //set CS11 1(8-fold Prescaler)
    TIMSK3 |= (1 << OCIE3A);
    sei();
  #elif defined(USING_DUE)
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk(ID_TC0);
    TC_Configure(TC0, 0, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK4);
    TC0->TC_CHANNEL[0].TC_RC = 41999;
    TC0->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
    NVIC_EnableIRQ(TC0_IRQn);
    TC_Start(TC0, 0);
  #endif
}

#ifdef USING_LEONARDO
ISR(TIMER3_COMPA_vect) {
  Joystick.getUSBPID();
}
#elif defined(USING_DUE)
void TC0_Handler() {
  TC_GetStatus(TC0, 0);
  Joystick.getUSBPID();
}
#endif

void loop() {
  int32_t outputValue = (customEncoderOutput != 0) ? customEncoderOutput : 0;

  if (outputValue > ENCODER_MAX_VALUE) {
    isOutOfRange = true;
    outputValue = ENCODER_MAX_VALUE;
  } else if (outputValue < ENCODER_MIN_VALUE) {
    isOutOfRange = true;
    outputValue = ENCODER_MIN_VALUE;
  } else {
    isOutOfRange = false;
  }
  
  Joystick.setXAxis(outputValue);
  Joystick.setRxAxis(analogRead(A1));
  Joystick.setRyAxis(analogRead(A2));
  Joystick.setYAxis(analogRead(A3));

  effectparams[0].springMaxPosition = ENCODER_MAX_VALUE;
  effectparams[0].springPosition = outputValue;
  effectparams[1].springMaxPosition = 255;
  effectparams[1].springPosition = 0;
  Joystick.setEffectParams(effectparams);
  Joystick.getForce(forces);

  Serial.println(forces[0]);  // Assuming forces[0] is the main force value

  if (!isOutOfRange) {
    if (forces[0] > 0) {
      digitalWrite(motorPinA, HIGH);
      digitalWrite(motorPinB, LOW);
      analogWrite(motorPinPWM, abs(forces[0]));
    } else {
      digitalWrite(motorPinA, LOW);
      digitalWrite(motorPinB, HIGH);
      analogWrite(motorPinPWM, abs(forces[0]));
    }
  } else {
    if (value < 0) {
      digitalWrite(motorPinA, LOW);
      digitalWrite(motorPinB, HIGH);
    } else {
      digitalWrite(motorPinA, HIGH);
      digitalWrite(motorPinB, LOW);
    }
    analogWrite(motorPinPWM, MAX_PWM);
  }
  
  setCustomEncoderOutput(as5600.getCumulativePosition());
  //Serial.println(as5600.getCumulativePosition());
}