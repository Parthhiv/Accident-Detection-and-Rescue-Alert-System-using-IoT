#include "Arduino.h"
#include "Credentials.h"
#include "Wire.h"
MPU6050 mpu;
float baseline[3];
float motion_threshold = 0.9;
const float DEFAULT_MOTION_THRESHOLD = 0.7;

void WiFiInit();
void FirebaseInit();
void FirebaseRead();

String position;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  mpu.initialize();
  pinMode(34,INPUT);
  calibrate();
  WiFiInit();
  FirebaseInit();
}

void loop()
{
  FirebaseRead();
  mpu_record();
  delay(1000); //  default 2000, current 1000
}

void mpu_read(float *posx, float *posy, float *posz)
{
  int16_t _ax, _ay, _az, _gx, _gy, _gz;
  mpu.getMotion6(&_ax, &_ay, &_az, &_gx, &_gy, &_gz);
  *posx = (_ax - 2050) / (16384.0);
  *posy = (_ay - 77) / (16384.0);
  *posz = (_az - 1947) / (16384.0);
}

void calibrate()
{
  float posx, posy, posz;
  for (int i = 0; i < 2; i++)
  {
    mpu_read(&posx, &posy, &posz);
    delay(500);
  }
  baseline[0] = posx;
  baseline[1] = posy;
  baseline[2] = posz;
}

void mpu_record()
{
  float posx, posy, posz;
  float aax, aay, aaz;
  float velx, vely, velz;
  String position;
  mpu_read(&posx, &posy, &posz);
  int val = digitalRead(34);
  velx = posx - baseline[0];
  vely = posy - baseline[1];
  velz = posz - baseline[2];
  Serial.print(velx);
  Serial.print(" ");
  Serial.print(vely);
  Serial.print(" ");
  Serial.println(velz);
  aax = fabs(velx);
  aay = fabs(vely);
  aaz = fabs(velz);
  Serial.print(aax);
  Serial.print(" ");
  Serial.print(aay);
  Serial.print(" ");
  Serial.println(aaz);
  position = "Safe";
  bool f = true;
  if (aax > motion_threshold || aaz > motion_threshold || aay > motion_threshold)
  {
    if (velz < 0){
      f = false;
    }
  }
  if(val==1){
    f = false;
  }
  if(!f){
    position = "Accident";  
    Firebase.setString(firebaseData, "Accident_Detection/IOTLAB/Fall_Detector/Position", position);
  }
  Serial.println(position);
  baseline[0] = posx;
  baseline[1] = posy;
  baseline[2] = posz;
}
void WiFiInit()
{
  pinMode(2, OUTPUT);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to Wi-Fi");
  int counter = 0, COUNTER_LIMIT = 30;
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    digitalWrite(2, !digitalRead(2));
    delay(300);
    counter++;
    if (counter >= COUNTER_LIMIT)
    {
      counter = 0;
      ESP.restart();
    }
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

void FirebaseInit()
{
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void FirebaseRead()
{
  String fdCloudFull;
  String fdCloud;
  if (Firebase.getString(firebaseData, "Accident_Detection/IOTLAB/Fall_Detector/Limit", fdCloudFull))
  {
    if (fdCloudFull.length() == 0)
      motion_threshold = DEFAULT_MOTION_THRESHOLD;
    else
    {
      fdCloud = fdCloudFull.substring(2, fdCloudFull.length() - 2);
      motion_threshold = fdCloud.toFloat();
    }
    Serial.println(motion_threshold);
  }
}
