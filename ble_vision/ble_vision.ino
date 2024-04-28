#include <ArduinoBLE.h>

BLEService myService("12345678-1234-1234-1234-123456789012"); // Create a BLE service
BLEStringCharacteristic myCharacteristic("87654321-4321-4321-4321-210987654321", BLERead | BLEWrite, 20); // Create a characteristic

void setup() {
  Serial.begin(9600);
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("Nicla Vision");
  BLE.setAdvertisedService(myService);
  myService.addCharacteristic(myCharacteristic);
  BLE.addService(myService);
  myCharacteristic.writeValue("Hello World"); // Initial value
  BLE.advertise();
  Serial.println("BLE Peripheral Ready");
  Serial.println("Device address: "); //a8:61:0a:1e:64:7d

  Serial.println(BLE.address());
}

void loop() {
  BLEDevice central = BLE.central();
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address()); //c8:89:f3:e7:48:4f
    // Exchange data
    while (central.connected()) {
      if (myCharacteristic.written()) {
        int command = myCharacteristic.value().toInt();
        Serial.println(myCharacteristic.value());
        switch (command) {
          case 1:
            Serial.println("Start data collection");
            break;
          case 2:
            Serial.println("Niknejad is here");
            break;
          default:
            Serial.println("No one is here");
        }
      }
    }
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}
