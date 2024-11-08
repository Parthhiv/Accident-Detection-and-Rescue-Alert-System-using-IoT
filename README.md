
# Accident Detection and Rescue Alert System using IoT

![Setup](./Assets/Setup.jpeg)

## Description

This repository contains the program and the project files that can be used to deploy the Accident Detection and Rescue Alert System using IoT. It uses a GISMO-VI ESP-32 microcontroller paired with the MPU6050 accelerometer to detect a crash. The system utilizes Firebase to store crash-related data and emergency contacts. An Android app, designed using Kodular, sends crash alerts and allows users to update their details. Additionally, Twilio is integrated to send SMS notifications upon detecting an accident.

![Architecture Diagram](./Assets/arch.png)

## Developer Usage

### Phase 1: Downloading Arduino IDE

1. Go to [this link](https://www.arduino.cc/en/software).
2. Download **Windows 10 and newer (64 bits)**.
3. Install the IDE using the setup file.

### Phase 2: Setting up the IDE

1. Go to `File` ➔ `Preferences`.
2. Enter [this URL](https://dl.espressif.com/dl/package_esp32_index.json) into the _Additional Boards Manager URLs:_ dialog box.
3. Go to `Tools` ➔ `Boards` ➔ `Boards Manager`.
4. Type **ESP32** and install _esp32_ by _Espressif Systems_.

_Note: Install the required libraries as mentioned in `GISMO6-Docs/Introduction.pptx`._

### Phase 3: Programming ESP32 with Arduino

1. Connect the ESP32 module along with the necessary sensors for the program.
2. Go to `Tools` ➔ `Boards` and select _ESP32 Dev Module_.
3. If the port is not set, go to `Tools` ➔ `Port` ➔ `COMX`.

_Note: Set the Serial Monitor to 115200 baud for serial messages._

![Arduino Setup](./Assets/arduino5804.jpg)

### Phase 4: Importing `.aia` file in Kodular

1. Open [Kodular Creator](https://creator.kodular.io).
2. Click on **Import Project** and upload the `.aia` file if it is not already imported.

![Kodular Application Structure](./Assets/kodular.jpeg)

### Phase 5: Connecting Kodular to Firebase

1. Copy the Firebase URL into **Firebase URL** (including `https://`) in the Firebase component.
2. Copy the Firebase secret into **Firebase Token** in the Firebase component.

![SMS Example](./Assets/sms.jpeg)
![WiFi Test Example](./Assets/wifitest.jpeg)

_Note: After installing libraries for Firebase and OLED Display, delete the folder `C:\Users\%USERNAME%\AppData\Local\Arduino15\libraries`._

## Phase 6: Setting Up Twilio for SMS Notifications

Twilio is used in this project to send SMS alerts when an accident is detected.

### Steps to Set Up Twilio

1. **Create a Twilio Account**:
   - Go to [Twilio](https://www.twilio.com/try-twilio) and sign up for a free account.
   - After signing up, get your **Account SID**, **Auth Token**, and **Twilio Phone Number**.

2. **Install the Twilio Arduino Library**:
   - In Arduino IDE, go to `Tools` ➔ `Manage Libraries`.
   - Search for **Twilio** and install the library.

3. **Configure the Code**:
   - Replace your **SSID**, **Password**, **Account SID**, **Auth Token**, and phone numbers in the code.

### Example Code for Twilio SMS

```cpp
#include "twilio.hpp"
#include <WiFi.h>

static const char *ssid = "Your_WiFi_SSID";
static const char *password = "Your_WiFi_Password";
static const char *account_sid = "Your_Account_SID";
static const char *auth_token = "Your_Auth_Token";
static const char *from_number = "Your_Twilio_Number";
static const char *to_number = "Receiver_Phone_Number";
static const char *message = "Accident Detected! Please check.";

Twilio *twilio;

void setup() {
  Serial.begin(115200);
  Serial.print("Connecting to WiFi...");
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected!");

  twilio = new Twilio(account_sid, auth_token);
  delay(1000);
  
  String response;
  bool success = twilio->send_message(to_number, from_number, message, response);

  if (success) {
    Serial.println("Message sent successfully!");
  } else {
    Serial.println("Failed to send message:");
    Serial.println(response);
  }
}

void loop() {
}
```

## Conclusion

This project integrates an IoT-based accident detection system with Firebase for real-time data storage and Twilio for sending SMS alerts, providing an effective solution for quick emergency responses.
