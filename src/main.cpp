
#include <WiFi.h>
#include "FirebaseESP32.h"

#define FIREBASE_HOST "egce58-project.firebaseio.com" //Do not include https:// in FIREBASE_HOST
#define FIREBASE_AUTH "SMS40RKC8viNsnAUPSAC9HxUZCTZuVFKxK0PGGap"
#define WIFI_SSID "Che"
#define WIFI_PASSWORD "0987654321"

//Define Firebase Data object
FirebaseData firebaseData;
String path = "/project-ce";

#define FRAME 1000
#define CUTOFF 100
int SENSOR = A0;
int values[FRAME];
int cloneValues[FRAME];
int count;
unsigned long currentTime = 0;
void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  // check is
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop()
{
  int value = analogRead(SENSOR);
  values[count] = value;
  count = count + 1;
  // values = [345, 343, 332, ...., 202]

  for (int i = 0; i < FRAME; i++)
  {
    cloneValues[i] = values[i];
  }
  // CUTOFF
  int sum = 0;
  for (int i = CUTOFF; i < FRAME - CUTOFF; i++)
  {
    sum = sum + cloneValues[i];
  }
  float avg = sum / (FRAME - (CUTOFF * 2));
  float voltage = (avg * 5.0 / 1024.0);

  Serial.println(voltage);
  if (count == FRAME)
  {
    count = 0;
  }
  delay(10);

  if (millis() - currentTime > 2000)
  {
    Firebase.setDouble(firebaseData, path + "/sensors/turbidity", voltage);
    currentTime = millis();
  }
}



int sort_desc(const void *cmp1, const void *cmp2)
{
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  return a < b ? -1 : (a > b ? 1 : 0);
}
