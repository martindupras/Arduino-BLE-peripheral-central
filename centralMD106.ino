// ---------------------------------------------------------------------------
// centralMD104.ino 2022 10 03
// MD: I'm trying to read accel data on one Arduino Nano 33 BLe (peripheral)
// and read it on another Arduino nano 33 BLE  connected with USB (serial),
// sending that data to a PD patch.
//
// THIS SEEMS TO BE WORKING! :)
// ---------------------------------------------------------------------------

#include <ArduinoBLE.h>

#define BLE_UUID_IMU_SERVICE              "1101"
#define BLE_UUID_ACC_CHAR                 "2101"
#define BLE_UUID_GYRO_CHAR                "2102"
#define BLE_UUID_MAG_CHAR                 "2103"


union sensor_data {
  struct __attribute__((packed)) {
    float values[3]; // float array for data (it holds 3)
    bool updated = false;
  };
  uint8_t bytes[3 * sizeof(float)]; // size as byte array 
};

union sensor_data accData;
union sensor_data gyroData;
union sensor_data magData;

void setup() {

  Serial.begin(115200);
  while (!Serial);

  // begin BLE initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
}

void loop()
{
  connectToPeripheral();
}

void connectToPeripheral()
{
  BLEDevice peripheral;

  do
  {
    Serial.println("scanning for service");
    // start scanning for peripherals
    BLE.scanForUuid(BLE_UUID_IMU_SERVICE); 
    peripheral = BLE.available();
    delay(3);

  } while (!peripheral);

  if (peripheral)
  {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found  ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // stop scanning
    BLE.stopScan();

    getDataPeripheral(peripheral);

  }
}

void getDataPeripheral(BLEDevice peripheral)
{
  // connect to the peripheral
  //Serial.println("Connecting ...");

  if (peripheral.connect())
  {
    //Serial.println("Connected");
  } else {
    //Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  //Serial.println("Discovering attributes ...");
  if (peripheral.discoverAttributes()) {
    //Serial.println("Attributes discovered");
  } else
  {
    //Serial.println("Attribute discovery failed!");
    peripheral.disconnect();
    return;
  }

  //BLECharacteristic gestureCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
  BLECharacteristic accCharacteristic = peripheral.characteristic(BLE_UUID_ACC_CHAR);
  BLECharacteristic gyroCharacteristic = peripheral.characteristic(BLE_UUID_GYRO_CHAR);
  BLECharacteristic magCharacteristic = peripheral.characteristic(BLE_UUID_MAG_CHAR);

  // subscribe to acceleration characteristic
  //Serial.println("Subscribing to simple key characteristic ...");
  if (!accCharacteristic) {
    //Serial.println("no gesture characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!accCharacteristic.canSubscribe()) {
    //Serial.println("gesture characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!accCharacteristic.subscribe()) {
    //Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to acceleration");
  }

    // subscribe to gyroscope characteristic
  //Serial.println("Subscribing to simple key characteristic ...");
  if (!gyroCharacteristic) {
    //Serial.println("no gesture characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!gyroCharacteristic.canSubscribe()) {
    //Serial.println("gesture characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!gyroCharacteristic.subscribe()) {
    //Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to gyroscope");
  }


  // subscribe to magneto characteristic
  //Serial.println("Subscribing to simple key characteristic ...");
  if (!magCharacteristic) {
    //Serial.println("no gesture characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!magCharacteristic.canSubscribe()) {
    //Serial.println("gesture characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!magCharacteristic.subscribe()) {
    //Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to magneto");
  }


  while (peripheral.connected()) {

    // while the peripheral is connected

    // check if the value of the gesture characteristic has been updated

    //Serial.println("inside peripheral.connected loop");
    
    //if (accelerationCharacteristic.valueUpdated()) {

    accCharacteristic.readValue(accData.bytes, sizeof accData.bytes);
    Serial.print("acc ");
    Serial.print(accData.values[0]);
    Serial.print(" ");
    Serial.print(accData.values[1]);
    Serial.print(" ");
    Serial.println(accData.values[2]);
    


    gyroCharacteristic.readValue(gyroData.bytes, sizeof gyroData.bytes);
    Serial.print("gyro ");
    Serial.print(gyroData.values[0]);
    Serial.print(" ");
    Serial.print(gyroData.values[1]);
    Serial.print(" ");
    Serial.println(gyroData.values[2]);


    magCharacteristic.readValue(magData.bytes, sizeof magData.bytes);
    Serial.print("mag ");
    Serial.print(magData.values[0]);
    Serial.print(" ");
    Serial.print(magData.values[1]);
    Serial.print(" ");
    Serial.println(magData.values[2]);
/*
      if (oldCommand == command)
        continue;

      oldCommand = command;

      //Serial.print("command:");
      //Serial.println(command);
      //doMovement(command);
    }
    */

    //MD TODO: do I need this?
    //wait
    //delay(50);
  }
  //Serial.println("Peripheral disconnected!");
}

// void doMovement(byte command)
// {
//   // MD removed
// }