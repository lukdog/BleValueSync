#include <ArduinoBLE.h>
#include <BleValueSync.h>

// Define BLE service and characteristic UUIDs
const char *serviceUUID = "12345678-1234-5678-1234-56789abcdef0";
const char *charUUID1 = "12345678-1234-5678-1234-56789abcdef1";
const char *charUUID2 = "12345678-1234-5678-1234-56789abcdef2";

// Create BleSyncValue instances
BleSyncValue value1(charUUID1, BLERead | BLEWrite);
BleSyncValue value2(charUUID2, BLERead | BLEWrite);

// Create BleSync instance
BleSync bleSync("MyBLEDevice", serviceUUID, 2);

void setup() {
  // Add values to the BLE service
  bleSync.addValue(&value1);
  bleSync.addValue(&value2);

  // Initialize BLE
  if (!bleSync.initBLE()) {
    Serial.println("Failed to initialize BLE");
    while (1);
  }

  Serial.println("BLE initialized");
}

void loop() {
  // Sync BLE for 1000 milliseconds
  bleSync.sync(1000);

  // Update the values
  value1.setValue(random(0, 100));
  value2.setValue(random(0, 100));
  
  delay(1000);
}
