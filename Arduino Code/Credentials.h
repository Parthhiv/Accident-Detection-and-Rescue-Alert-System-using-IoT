#include "I2Cdev.h"
#include "MPU6050.h"
#include "FirebaseESP32.h"
#include <Wire.h>
#define FIREBASE_HOST "accident-detection-c3d5d-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "nylidEnD7dLZvM6QWxK2qoH9u2evtlunyXNxKhJF"

FirebaseData firebaseData;
const char* ssid = "abcd";
const char* pass = "12345678";
