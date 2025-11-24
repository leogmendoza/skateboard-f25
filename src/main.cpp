#include "ble_handler.h"
#include <Arduino.h>
#include <NimBLEDevice.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  initBLE();
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long lastTime = 0;
  unsigned long curtime = millis();

  if (curtime - lastTime > 1000){
    lastTime = curtime;

    notifyBLEData();
    //hi
  }
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}