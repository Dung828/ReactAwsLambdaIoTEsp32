// 7/09/2020
#include "secrets.h"
#include <WiFiClientSecure.h>

#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "time.h"
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 36000;
const int   daylightOffset_sec = 3600;


const int inled = 2; // internal led gpio-2
const int DELAY_MSEC = 60000; // 60 seconds
const int NUM_BLINKS = 10;
const int TIME_PUB_MSG =  60000; // 15 minutes = 900000
const int TIME_BLINK_LED = 10000;
const int JBUFF = 256;
const int MAX_JBUFF = 512;

int k = 0; // loop count
long pM1 = 0;
long pM2 = 0;
long cM = 0;
int lostConnection = 0;

// Wifi credentials
const char* WIFI_SSID = "xxxx";
const char* WIFI_PASSWORD = "xxxxx";

#define THINGN "nodemcu32s"
//const char* deviceName = THINGN;



// The MQTT topics that this device should publish/subscribe

//#define P_TOPIC  THINGN "/pub"
//#define S_TOPIC THINGN "/sub"

#define S_UPD_TOPIC  "$aws/things/" THINGN "/shadow/update"
#define S_UPD_ACC_TOPIC "$aws/things/" THINGN "/shadow/update/accepted"
#define S_UPD_REJ_TOPIC "$aws/things/" THINGN "/shadow/update/rejected"
//#define S_STATE_DOC_LED_ON  """{"state" : {"desired" : {"LED" : "ON"}}}"""
//#define S_STATE_DOC_LED_OFF """{"state" : {"desired" : {"LED" : "OFF"}}}"""
// The MQTTT endpoint for the device (unique for each AWS account but shared amongst devices within the account)
#define IOT_ENDPOINT "xxxxxxxxxxxxxxxxxxxxxx"

//-----------------------------------------------------------------------------
WiFiClientSecure net = WiFiClientSecure();
MQTTClient client = MQTTClient(MAX_JBUFF);
//-------------------------------------------------------------------------------


void BlinkLed() {
  if (digitalRead(inled) == LOW) {
    digitalWrite(inled, HIGH);
  }
  else {
    digitalWrite(inled, LOW);
  }
}

void wifiSetup() {

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.mode(WIFI_STA); //WiFi mode station connect to wifi router only
  Serial.println("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

void connectAWS()
{

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.begin(IOT_ENDPOINT, 8883, net);

  // Create a message handler
  client.onMessage(messageHandler);

  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGN)) {
    Serial.print(".");
    delay(100);
  }

  if (!client.connected()) {
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  // client.subscribe(S_TOPIC);

  client.subscribe(S_UPD_TOPIC);
  client.subscribe(S_UPD_ACC_TOPIC);
  client.subscribe(S_UPD_REJ_TOPIC);


  Serial.println("AWS IoT Connected!");
}

void sendJsonToAWS(int cnt)
{
  char jsonBuffer[JBUFF];
  StaticJsonDocument<MAX_JBUFF> jsonDoc;
  JsonObject stateObj = jsonDoc.createNestedObject("state");
  JsonObject reportedObj = stateObj.createNestedObject("reported");

  reportedObj["count"] = cnt;
  reportedObj["lostConnection"] = lostConnection;
  reportedObj["localTime"] = String(GetTime());
  reportedObj["time"] = millis();
  reportedObj["wifi_strength"] = WiFi.RSSI();
  reportedObj["device"] = "nodemcu32s";


  // Create a nested object "location"
  //JsonObject locationObj = reportedObj.createNestedObject("location");
  //locationObj["name"] = "Garden";

  // Publish the message to AWS
  serializeJson(jsonDoc, jsonBuffer); // serialize before sending to AWS UTF-8 encoding

  client.publish(S_UPD_TOPIC, jsonBuffer);
  Serial.println("*************************************************************");
  Serial.print("sendJsonToAWS " + String(cnt) + " > "); Serial.println(S_UPD_TOPIC);
  Serial.println(jsonBuffer);
  Serial.println("*************************************************************");
  //  client.publish(P_TOPIC, jsonBuffer);
}

void messageHandler(String &topic, String &payload) {
  Serial.println("--------------------------------------------------");
  Serial.println("millisTime " + String(millis()) + " incoming " + topic);
  Serial.println(payload);
  if (topic == S_UPD_TOPIC) {
    Serial.print(S_UPD_TOPIC); Serial.println(" - blink led 10 times");
    for (int n = 0; n < 10; n++) {
      BlinkLed();
      delay(200);
    }
  }
  else if (topic == S_UPD_ACC_TOPIC) {
    Serial.print(S_UPD_ACC_TOPIC); Serial.println(" - blink led 20 times");
    for (int m = 0; m < 20; m++) {
      BlinkLed();
      delay(100);
    }
  }
  else if (topic == S_UPD_REJ_TOPIC) {
    Serial.print(S_UPD_REJ_TOPIC); Serial.println(" - blink led 40 times");
    for (int m = 0; m < 40; m++) {
      BlinkLed();
      delay(50);
    }
  }
  else {
    Serial.print(topic); Serial.println(" Undefined TOPIC - blink led 80 times");
    for (int m = 0; m < 80; m++) {
      BlinkLed();
      delay(25);
    }
  }

  Serial.println("--------------------------------------------------");
  //  StaticJsonDocument<200> doc;
  //  deserializeJson(doc, payload);
  //  const char* message = doc["message"];

}

String GetTime() {
  struct tm timeinfo;
  char timeStringBuff[50];
  getLocalTime(&timeinfo);
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d/%m/%Y %H:%M:%S", &timeinfo);
  return timeStringBuff;
}

void setup() {
  Serial.begin(9600);
  pinMode(inled, OUTPUT);

  Serial.println("wifiSetup----------------------");
  wifiSetup();

  Serial.println("configTime---------------------");
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(GetTime());

  Serial.println("connectAWS----------------------");
  connectAWS();

  Serial.println("First sendJsonToAWS----------------------");
  sendJsonToAWS(k);
  Serial.println(GetTime());
}


void loop() {

  //  Serial.println("loop " + String(k) + " __ millisTime " + String(millis()));

  cM = millis();
  if (cM - pM1 > TIME_PUB_MSG) {
    pM1 = cM;  k++;
    //****************
    sendJsonToAWS(k);
    Serial.println(GetTime());
  }
  else if (cM - pM2 > TIME_BLINK_LED) {
    pM2 = cM;
    for (int j = 0; j < 5; j++) {
      BlinkLed();
      delay(100);
    }
    if (!client.connected()) {
      lostConnection++;
      Serial.println("!!!!!!!!!!!!!!!! AWS IoT Timeout!");
      Serial.println("wifiSetup.............");
      wifiSetup();
      Serial.println("connectAWS.............");
      connectAWS();
    }
    
  }


  client.loop();
}
