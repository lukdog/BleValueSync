# BleValueSync Library Documentation

The `BleValueSync` library simplifies the configuration and communication using Bluetooth Low Energy (BLE) peripherals for synchronizing numeric values. This library leverages the `ArduinoBLE` library to facilitate BLE operations.

## Overview

`BleValueSync` provides an easy-to-use interface for setting up BLE peripherals and syncing numeric values. It consists of two main classes:

- `BleSyncValue`: Represents a BLE characteristic that holds a numeric value.
- `BleSync`: Manages BLE services and characteristics, and handles BLE communication.

## Installation

To use the `BleValueSync` library, include the header file in your project:

```cpp
#include "BleValueSync.h"
```

Make sure you have the `ArduinoBLE` library installed, as `BleValueSync` depends on it.

## Classes

### BleSyncValue

The `BleSyncValue` class represents a BLE characteristic that holds a numeric value.

#### Constructor

```cpp
BleSyncValue();
BleSyncValue(const char *uuid, unsigned int property);
```

- `uuid`: The UUID of the BLE characteristic.
- `property`: The BLE property (e.g., `BLERead | BLEWrite`).

#### Methods

- `void setValue(uint32_t newValue)`: Sets the value of the characteristic and updates the BLE characteristic if the value has changed.

### BleSync

The `BleSync` class manages BLE services and characteristics and handles BLE communication.

#### Constructor

```cpp
BleSync(const char *name, const char *uuid, uint8_t valuesQuantity);
```

- `name`: The local name of the BLE device.
- `uuid`: The UUID of the BLE service.
- `valuesQuantity`: The number of `BleSyncValue` instances that can be managed.

#### Methods

- `bool addValue(BleSyncValue *v)`: Adds a `BleSyncValue` instance to the BLE service.
- `bool initBLE()`: Initializes the BLE device and starts advertising.
- `void sync(unsigned long waitTime)`: Synchronizes the BLE device for the specified duration in milliseconds.

## Usage

Here is an example of how to use the `BleValueSync` library:

```cpp
#include <ArduinoBLE.h>
#include "BleValueSync.h"

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
```

In this example:

1. Two `BleSyncValue` instances are created with their respective UUIDs and properties.
2. A `BleSync` instance is created to manage the BLE service and characteristics.
3. The `BleSync` instance is initialized and starts advertising the BLE service.
4. In the `loop` function, the BLE device is synchronized, and the values of the characteristics are updated periodically.


---

This documentation provides a comprehensive guide to using the `BleValueSync` library for BLE communication in Arduino projects.