#ifndef BleValueSync_h
#define BleValueSync_h

#include <ArduinoBLE.h>

class BleSyncValue {
public:
  BleSyncValue();
  BleSyncValue(const char *uuid, unsigned int property) {
    _uuid = uuid;
    _characteristic = new BLEIntCharacteristic(uuid, property);
    _characteristic->writeValue(0);
  }

  void setValue(int32_t newValue) {
    if(newValue == getValue()) {return;}
    //value = newValue;
    _characteristic->writeValue(newValue);
  }

  int32_t getValue() {
    int32_t v = 0;
    _characteristic->readValue(v);
    return v;
  }

private:
  const char *_uuid;
  uint32_t value = 0;
  BLEIntCharacteristic *_characteristic;
  
friend class BleSync;
};

class BleSync {
public:
  BleSync(const char *name, const char *uuid, uint8_t valuesQuantity) {
    _name = name;
    _uuid = uuid;
    _values = new BleSyncValue*[valuesQuantity];
    _quantity = valuesQuantity;
  }

  bool addValue(BleSyncValue *v){
    if(_currentQuantity == _quantity ) {
      return false;
    }

    _values[_currentQuantity] = v;
    _currentQuantity++;

    return true;
    
  }

  bool initBLE() {
    _bleService = new BLEService(_uuid);
    
    if(!BLE.begin()) {
      return false;
    }

    BLE.setLocalName(_name);
    BLE.setAdvertisedService(*_bleService);

    for(uint8_t i = 0; i<_currentQuantity; i++){
      _bleService->addCharacteristic(*(_values[i]->_characteristic));
    }

    BLE.addService(*_bleService);
    BLE.advertise();

  }

  void sync(unsigned long waitTime){
    
    long startSync = millis();
    while(millis() - startSync < waitTime){
      BLEDevice central = BLE.central();
      if(central){
        while(central.connected()){
          delay(1);
        }
      }
      delay(100);
    }
  }

protected:
  BLEService *_bleService;
  BleSyncValue **_values;
  const char *_name;
  const char *_uuid;
  uint8_t _quantity = 0;
  uint8_t _currentQuantity = 0;
};


#endif
