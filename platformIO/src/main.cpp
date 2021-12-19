/*
  Author: Orekaria
  Thermostast for vehicles with Arduino
*/

#include <Arduino.h>
#include <Shared.h>
#include <Utils.h>
#include <PowerConsumption.h>
#include <CircularBuffer.h>
#include <CountDown.h>
#include <Display.h>
#include <Heater.h>
#include <Voltimeter.h>
#include <Potentiometer.h>
#include <Thermostat.h>

// set the interval at which different code is executed. Notice that time is multiplied depending on SPEED_REDUCED. e.g: SPEED_REDUCED = 0x02 => 16/4 => time multiplied by 4
#define MICRO_BEAT (uint8_t) (4/(uint8_t)(pow(2, (uint8_t)SPEED_FULL))) // in ticks. update display when isDisplayOn
#define MINI_BEAT (uint16_t) (50/(uint8_t)(pow(2, (uint8_t)SPEED_REDUCED))) // in ticks. check button and potentiometer
#define BEAT (uint16_t) (1000/(uint8_t)(pow(2, (uint8_t)SPEED_REDUCED))) // in ticks. get ambient temp. disable display after AUTO_DISPLAY_OFF_TIME
#define LONG_BEAT (uint32_t) (60000/(uint8_t)(pow(2, (uint8_t)SPEED_REDUCED))) // in ticks. set heater ON/OFF

#define AUTO_DISPLAY_OFF_TIME (uint16_t) (2000/(uint8_t)(pow(2, (uint8_t)SPEED_FULL))) // beats

#define LM_35 A4 // if A2 is used for something else, it can cause inestabilities (?)
#define POTENTIOMETER A1
#define VCC A6
#define PUSH_BUTTON A3
#define LONG_PRESS (uint16_t) (2000/(uint8_t)(pow(2, (uint8_t)SPEED_FULL))) // ticks
#define RELE 13

Display display = Display(AUTO_DISPLAY_OFF_TIME / BEAT);
Heater heater = Heater(RELE);
Voltimeter voltimeter = Voltimeter(VCC);
Thermostat thermostat = Thermostat(LM_35);
Potentiometer potentiometer = Potentiometer(POTENTIOMETER);

float ambientTempAvg = -1;
float desiredTemp = 0.0;

#if DEBUG == true
uint16_t beatCount = 0;
#endif

void longbeat() {
#if DEBUG == true
  Utils::log(CARRIAGE_RETURN + "long beat" + CARRIAGE_RETURN);
#endif
  bool isChanged = heater.setOnOff(false, ambientTempAvg, desiredTemp);
  if (isChanged) {
    display.switchOn();
  }
}

void beat(float desiredTemp) {
#if DEBUG == true
  Utils::log(String(millis()) + " - ");
#endif

  // voltage. if the caravan is power on, or 220V is plugged
  if (voltimeter.isVoltageSpike()) {
    heater.lock();
  }

#if DEBUG == true
  beatCount++;
  if (beatCount <= 1) { // skip first random readings in proteus
    return;
  }
  Utils::log(String(beatCount, DEC) + " ");
#endif

  // temperature
  ambientTempAvg = thermostat.readLM35();

#if DEBUG == true
  Utils::log("| tempAvg: " + String(ambientTempAvg, 1));
  // Utils::log(" (" + String(vinCircularBuffer.minSample(), 2));
  // Utils::log(" - " + String(vinCircularBuffer.maxSample(), 2));
  // Utils::log(" = " + String(vinCircularBuffer.maxSample() - vinCircularBuffer.minSample(), 2));
  // Utils::log(")");
  Utils::log(", pot: " + String(desiredTemp, 1));
  Utils::log(", heater: " + String(heater.lastHeaterState() == HIGH ? "ON" : "OFF"));
#endif

  display.heartbeat(heater.lastHeaterState());

#if DEBUG == true
  Utils::log(CARRIAGE_RETURN);
#endif
}

float lastDesiredTemp = -1;
uint8_t infoToShow = 1;

CountDown longPressCountDown = CountDown(LONG_PRESS / MINI_BEAT);
void minibeat(float desiredTemp) {
  // push button. check if it is pushed
  int buttonState = digitalRead(PUSH_BUTTON);
  if (buttonState == HIGH) {
    if (longPressCountDown.isFinished()) {
      heater.lock();
    } else {
      if (heater.isHeaterLocked()) {
        heater.setOnOff(true, ambientTempAvg, desiredTemp);
      }
      infoToShow = 1;
      display.switchOn();
    }
  } else {
    longPressCountDown.reset();
    infoToShow = 2;
  }

  // potentiometer. check if it has been rotated
  if (lastDesiredTemp != desiredTemp) {
    lastDesiredTemp = desiredTemp;
    heater.setOnOff(true, ambientTempAvg, desiredTemp);
    infoToShow = 2;
    display.switchOn();
  }

  if (heater.isHeaterLocked()) {
    display.switchOn();
  }
}

void microbeat() {
  display.updateDisplay();
}

CountDown beatCountdown = CountDown(BEAT / MICRO_BEAT, true);
CountDown minibeatCountdown = CountDown(MINI_BEAT / MICRO_BEAT, true);
CountDown longbeatCountdown = CountDown(LONG_BEAT / MICRO_BEAT, true);
PowerConsumption powerConsumtion = PowerConsumption();

// the loop function runs over and over again forever
void loop() {
  desiredTemp = potentiometer.readPotentiometer();

  if (minibeatCountdown.isFinished()) {
    minibeat(desiredTemp);
  }

  if (beatCountdown.isFinished()) {
    beat(desiredTemp);
  }

  if (longbeatCountdown.isFinished()) {
    longbeat();
  }

  // display
  if (display.isDisplayOn()) {
    powerConsumtion.high();
    switch (infoToShow) {
    case 1: // temp
      display.setContent(heater.isHeaterLocked(), ambientTempAvg, desiredTemp, true);
      break;
    case 2: // pot
      display.setContent(heater.isHeaterLocked(), ambientTempAvg, desiredTemp, false);
      break;
    default:
      break;
    }
  } else {
    // enter low power mode if the display is off
    powerConsumtion.low();
  }

  microbeat();

  delay(MICRO_BEAT);
}

// the setup function runs once when you press reset or power the board
void setup() {
  powerConsumtion.high();

  pinMode(PUSH_BUTTON, INPUT);
}
