#include <ArduinoBLE.h>
#include "BleValueSync.h"

BleSyncValue temperatureValue("19B10000-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);
BleSyncValue humidityValue("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify);

BleSync peripheralDevice("PeripheralDevice", "19B10000-E8F2-537E-4F6C-D104768A1214", 2);

void setup() {
  Serial.begin(9600);
  
  peripheralDevice.addValue(&temperatureValue);
  peripheralDevice.addValue(&humidityValue);
  
  if (!peripheralDevice.initBLE()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }

  Serial.println("BLE initialized");
}

void loop() {
  int32_t temperature = random(20, 30);
  int32_t humidity = random(40, 60);

  temperatureValue.setValue(temperature);
  humidityValue.setValue(humidity);

  peripheralDevice.sync(5000);  // Sync for 5 seconds
}