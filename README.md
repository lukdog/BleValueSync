# BLE Arduino Library

This Arduino library simplifies the use of Bluetooth Low Energy (BLE) for creating multiple peripheral devices and a single aggregator device. It provides classes to manage BLE characteristics and synchronize values between peripherals and the aggregator.

## Classes

### `BleSyncValue`
Represents a BLE characteristic that can be synchronized.

#### Constructors
- `BleSyncValue()`
- `BleSyncValue(const char *uuid, unsigned int property)`

#### Methods
- `void setValue(int32_t newValue)`: Sets a new value for the characteristic.
- `int32_t getValue()`: Retrieves the current value of the characteristic.

### `BleSync`
Handles a collection of `BleSyncValue` objects and manages BLE initialization and synchronization.

#### Constructor
- `BleSync(const char *name, const char *uuid, uint8_t valuesQuantity)`

#### Methods
- `bool addValue(BleSyncValue *v)`: Adds a `BleSyncValue` to the collection.
- `bool initBLE()`: Initializes the BLE service and starts advertising.
- `void sync(unsigned long waitTime)`: Synchronizes the BLE values.

### `BleReaderValue`
Represents a BLE characteristic that can be read from a peripheral device.

#### Constructor
- `BleReaderValue(const char* uuid, bool resetAfterRead = false)`

#### Methods
- `bool syncValue(BLEDevice peripheral)`: Synchronizes the value from the peripheral.
- `int32_t getValue()`: Retrieves the current value.

### `BleReader`
Manages a collection of `BleReaderValue` objects and handles BLE scanning and synchronization.

#### Constructor
- `BleReader(const char* readerName, const char* readerUUID, uint8_t valueQuantity)`

#### Methods
- `bool addValue(BleReaderValue *v)`: Adds a `BleReaderValue` to the collection.
- `bool syncAll(long syncTime)`: Synchronizes all values from the peripheral devices.

## Examples

### Peripheral Device Sketch

```cpp
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
```

### Aggregator Device Sketch

```cpp
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
```

In these examples, the peripheral device creates two BLE characteristics (temperature and humidity) and advertises them. The aggregator device scans for the peripheral, reads the values of these characteristics, and prints them to the serial monitor.