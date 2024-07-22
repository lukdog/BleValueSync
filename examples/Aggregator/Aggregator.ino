#include <ArduinoBLE.h>
#include "BleValueSync.h"

BleReaderValue temperatureValue("19B10000-E8F2-537E-4F6C-D104768A1214", false);
BleReaderValue humidityValue("19B10001-E8F2-537E-4F6C-D104768A1214", false);

BleReader peripheral("PeripheralDevice", "19B10000-E8F2-537E-4F6C-D104768A1214", 2);

void setup() {
  Serial.begin(9600);
  
  peripheral.addValue(&temperatureValue);
  peripheral.addValue(&humidityValue);

  BLE.begin();
}

void loop() {
  if (peripheral.syncAll(10000)) {  // Sync for 10 seconds
    Serial.print("Temperature: ");
    Serial.println(temperatureValue.getValue());
    Serial.print("Humidity: ");
    Serial.println(humidityValue.getValue());
  } else {
    Serial.println("Failed to sync with peripherals");
  }

  delay(10000);
}