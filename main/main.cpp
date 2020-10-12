/*
	HEART RATE MONITOR BLE Service with BLE Arduino library as ESP-IDF component
	- by Sukesh Ashok Kumar


    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleServer.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    updates by chegewara
*/
#include <stdio.h>
#include <esp_log.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "BLE2902.h"
#include "Task.h"
#include "math.h"

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define byte uint8_t

#define heartRateService BLEUUID((uint16_t)0x180D)

BLECharacteristic heartRateMeasurementCharacteristics(BLEUUID((uint16_t)0x2A37), BLECharacteristic::PROPERTY_NOTIFY);
BLECharacteristic sensorPositionCharacteristic(BLEUUID((uint16_t)0x2A38), BLECharacteristic::PROPERTY_READ);
BLEDescriptor heartRateDescriptor(BLEUUID((uint16_t)0x2901));
BLEDescriptor sensorPositionDescriptor(BLEUUID((uint16_t)0x2901));

byte flags = 0b00111110;
byte bpm;
byte heart[8] = { 0b00001110, 60, 0, 0, 0 , 0, 0, 0};
byte hrmPos[1] = {2};

bool _BLEClientConnected = false;

// Required for using C++ with ESP-IDF for ESP32
extern "C" {
	void app_main(void);
}

// Just a random generator
int randomGen(int min, int max)
{
	int range; 
	range = max-min + 1;
	return rand()%range + min;
}

// Task which generates random numbers between 80-180 and sends it in notification
class bleMessageTask: public Task {
	void run(void *data) {
	
		while(1) {
      heart[1] = (byte)bpm;
      int energyUsed = 3000;
      heart[3] = energyUsed / 256;
      heart[2] = energyUsed - (heart[2] * 256);
      //printf("BPM: %d\n",bpm);

      heartRateMeasurementCharacteristics.setValue(heart, 8);
      heartRateMeasurementCharacteristics.notify();

      sensorPositionCharacteristic.setValue(hrmPos, 1);
      bpm = (byte)randomGen(80,180);  

      delay(200);
		} // While 1
	} // run
}; // bleMessageTask

bleMessageTask *pBpmNotifyTask;

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      printf("Connected!\n");
      pBpmNotifyTask->start();
    };

    void onDisconnect(BLEServer* pServer) {
      pBpmNotifyTask->stop();
      printf("Disconnected!\n");
    }
};

static void run() 
{
  pBpmNotifyTask = new bleMessageTask();
  pBpmNotifyTask->setStackSize(8000);

  printf("Starting BLE setup!\n");

  // BLE Device Name - Look for this name from the App
  BLEDevice::init("HRM SVC");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pHeart = pServer->createService(heartRateService);

  // Create HRM BLE Characteristics
  pHeart->addCharacteristic(&heartRateMeasurementCharacteristics);

  heartRateDescriptor.setValue("Rate from 0 to 200");
  heartRateMeasurementCharacteristics.addDescriptor(&heartRateDescriptor);
  
  pHeart->addCharacteristic(&sensorPositionCharacteristic);
  sensorPositionDescriptor.setValue("Position 0 - 6");
  sensorPositionCharacteristic.addDescriptor(&sensorPositionDescriptor);

  pServer->getAdvertising()->addServiceUUID(heartRateService);

  pHeart->start();

  // Start advertising
  pServer->getAdvertising()->start();

  printf("Connect using a smartphone App to this 'HRM SVC'\n");
  printf("Use nRFToolbox or nRFConnect App which supports Hear Rate Monitor\n");

}

void app_main()
{
  run();
}
