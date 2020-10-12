# ESP-IDF BLE HRM (Heart Rate Monitor) Service for ESP32
This is an attempt to create BLE HRM Service using BLE Arduino library but used as a component of ESP-IDF without taking full Arduino dependency.

# How does it work?
Heart Rate Monitor service is a standard published by Bluetooth standards which means that the service GUID and characteristics are all pre-defined. This also makes it easy to test the service using any HRM App on smartphone.

### HRM Service and Measurements
- Heart Rate Service - 0x180D
- Heart Rate Measurements Characteristics (BPM) - 0x2A37
- Body Sensor Location - 0x2A38

For more details check official website links [BLE Services](https://www.bluetooth.com/specifications/gatt/services/) / 
[BLE characteristics](https://www.bluetooth.com/specifications/gatt/characteristics/) / [BLE Descriptors](https://www.bluetooth.com/specifications/gatt/descriptors/)

### How to test the BLE HRM Service?
Install any HRM BLE App and search and connect to your ESP32 device.
- nRFToolbox App (To see how HRM service works)
- nRFConnect App (To learn internals of the BLE service)

