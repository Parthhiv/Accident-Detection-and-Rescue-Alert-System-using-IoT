#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL335.h>

#define PULSE_SENSOR_PIN A0
#define LM35_SENSOR_PIN A1
#define BUZZER_PIN 2

// Replace with your WiFi credentials and ThingSpeak channel details
const char *ssid = "your-SSID";
const char *password = "your-PASSWORD";
const char *thingSpeakApiKey = "your-THING_SPEAK_API_KEY";
const unsigned long postingInterval = 15000; // Posting interval in milliseconds (15 seconds)

// Initialize ADXL335 sensor
Adafruit_ADXL335 accel = Adafruit_ADXL335(3, 4, 5);

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read accelerometer values
  sensors_event_t event;
  accel.getEvent(&event);

  // Read temperature from LM35 sensor
  int lm35Value = analogRead(LM35_SENSOR_PIN);
  float temperature = (lm35Value * 0.48828125);

  // Read pulse sensor data
  int pulseValue = analogRead(PULSE_SENSOR_PIN);

  // Check for elderly activity (adjust threshold as needed)
  if (event.acceleration.x > 2.0 || event.acceleration.y > 2.0 || event.acceleration.z > 2.0) {
    // Elderly person is active
    digitalWrite(BUZZER_PIN, HIGH);
    delay(1000);
    digitalWrite(BUZZER_PIN, LOW);
  }

  // Send data to ThingSpeak
  if (millis() - lastConnectionTime > postingInterval) {
    updateThingSpeak(temperature, pulseValue);
  }
}

void updateThingSpeak(float temperature, int pulseValue) {
  // Create a ThingSpeak update object
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, pulseValue);

  // Write the fields to ThingSpeak
  int httpCode = ThingSpeak.writeFields(CHANNEL_ID, thingSpeakApiKey);

  if (httpCode == 200) {
    Serial.println("Channel update successful");
  } else {
    Serial.println("Failed to update ThingSpeak channel. HTTP error code: " + String(httpCode));
  }

  lastConnectionTime = millis();
}
