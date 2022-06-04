/*  ePaper Display (EPD) by ThatRed based on klenzel's code
 *
 *  Bibliotheken:
 *  
 *  ESP8266:
 *  ESP8266WiFi.h
 *  ESP8266mDNS.h
 *  
 *  ESP32:
 *  WiFi.h
 *  ESPmDNS.h
 *  
 *  Allgemein:
 *  time.h
 *  GxEPD.h
 *  GxEPD.h LilyGo Version: https://github.com/lewisxhe/GxEPD den Ordner GxDEPG0150BN nach GxEPD/src kopieren
 *  GxIO/GxIO_SPI/GxIO_SPI.h
 *  GxIO/GxIO.h
 *  PubSubClient.h
 *  WiFiClient.h
 *  WebOTA.h
 *  
 *  Schriftarten:
 *  Adafruit_GFX.h>
 *  Fonts/Picopixel.h>
 *  Fonts/TomThumb.h>
 *  Fonts/FreeSans9pt7b.h>
 *  Fonts/FreeSans12pt7b.h>
 *  Fonts/FreeSans18pt7b.h>
 *  Fonts/FreeSans24pt7b.h>
 *  Fonts/FreeSansBold9pt7b.h>
 *  Fonts/FreeSansBold12pt7b.h>
 *  Fonts/FreeSansBold18pt7b.h>
 *  Fonts/FreeSansBold24pt7b.h>
 *
 *  WEMOS D1 mini Verkabelung:
 *  
 *  Busy violett D1
 *  RST  weiss   D0
 *  DC   grün    D2
 *  CS   orange  D8
 *  CLK  gelb    D5
 *  DIN  blau    D7
 *  GND  braun   G
 *  VCC  grau    3V3
 *
 *  Board: "LOLIN(WEMOS) D1 R2 & mini"
 *
 *  LILYGO® TTGO MINI ESP32 Verkabelung:
 *  
 *  Busy violett 4
 *  RST  weiss   16
 *  DC   grün    17
 *  CS   orange  5
 *  CLK  gelb    18
 *  DIN  blau    23
 *  GND  braun   GND
 *  VCC  grau    3V3
 *  
 *  Board: "ESP32 DEV Module"
 *  
 *  Richtiges Board und Port unter Werkzeuge auswählen!! 
 *  
 *  Im Boardverwalter für ESP32, Version 2.0.3 installieren.
 *
 *  3D-Dateien: https://www.thingiverse.com/thing:5402812
 *  
 *  Anfang benutzerdefinierte Konfiguration:
 *
 */ 
//
// Wifi Konfiguration
//
const char* ssid = "SSID";
const char* password = "Passwort";
//
// MQTT Konfiguration
//
const char* mqttClient = "ePaperDisplayRaum";
const char* mqtt_server = "IP-Adresse";
const char* mqttUser = "Benutzer";
const char* mqttPassword = "Passwort";
//
// MQTT Topics
//
const char* mqttTopic_CO2 = "myhome/Raum/CO2";
const char* mqttTopic_Vibra = "myhome/Raum/Vibra"; 
const char* mqttTopic_PVModule = "myhome/Balkon/PV-Module";
const char* mqttTopic_Hum = "myhome/Nord/Hum";
const char* mqttTopic_Temp = "myhome/Nord/Temp";
const char* mqttTopic_Power = "myhome/Wohnung/Bezug";
// const char* mqttTopic_ApartmentPresence = "myhome/Wohnung/AnAbwesendheit";
//
// NTP Server Konfiguration
//
const char* ntpServer = "lokale_IP-Adresse";
// const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600; // CEST Zeitzonenversatz UTC/GMT +1 Stunde (Berlin)
const int   daylightOffset_sec = 3600; // Sommerzeitversatz +1 Stunde 
//
// Splashscreen Verzögerung
//
int SCDelay = 6000;
//
// Waveshare 1.54" ePaper Displays, Kommentierung je nach Display und Entwicklungsboard entsprechend entfernen:
//
//#define Display_v2_ESP32        //bw Version >=2
#define Display_Wrist_v2_ESP32  //bw Version >=2
// #define Display_v2_ESP8266      //bw Version >=2
//
// #define Display_v2_bwr_ESP32    //bwr Version <2, leider noch keine Unterstützung für die neuen Tri-Color Displays in GxEPD.h!!
// #define Display_v1_ESP8266      //bw Version <2, ungetestet!!
//
// Ende benutzerdefinierte Konfiguration
//
const long interval = 60000;
int timeState = 0;
unsigned long previousMillis = 0; 
unsigned long wifipreviousMillis = 0;
unsigned long wifiinterval = 30000;

const char* Version = "1.6";
const char compile_date[] = __DATE__ " " __TIME__;

// Im Boardverwalter für ESP32, Version 2.0.3 installieren.
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
  #include "WiFi.h"
  #include <ESPmDNS.h> 
#else
  #error "Nur für Entwicklungsboards basierend auf ESP8266 oder ESP32!"
#endif

#include "time.h"
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// Display Bibliotheken

#ifdef Display_v1_ESP8266
#include <GxGDEP015OC1/GxGDEP015OC1.h>			
#endif

#ifdef Display_v2_ESP8266
#include <GxGDEH0154D67/GxGDEH0154D67.h>		
#endif

#ifdef Display_v2_ESP32
#include <GxGDEH0154D67/GxGDEH0154D67.h>    
#endif

#ifdef Display_v2_bwr_ESP32 // leider noch keine Unterstützung für die neuen Tri-Color Displays in GxEPD.h!!
#include <GxGDEW0154Z04/GxGDEW0154Z04.h>
#endif

#ifdef Display_Wrist_v2_ESP32
#include <GxDEPG0150BN/GxDEPG0150BN.h>
#endif

#include <PubSubClient.h>
#include <WiFiClient.h>
#include <WebOTA.h>

#ifdef Display_v1_ESP8266 //ungetestet
// WEMOS D1 mini
#define EPD_BUSY D1
#define EPD_RESET D4
#define EPD_DC D3
// #define EPD_CS D8
// #define SPI_SCK D5
// #define SPI_DIN D7

GxIO_Class io(SPI, /*CS*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RESET);
GxEPD_Class display(io, /*RST=*/EPD_RESET, /*BUSY=*/EPD_BUSY);

// GxIO_Class io(SPI, SS/* CS */,  D3/* DC */,  D4/* RST */);		
// GxEPD_Class display(io);										                  
#endif

/* WEMOS D1 mini
 *  
 *  Busy violett D1
 *  RST  weiss   D0
 *  DC   grün    D2
 *  CS   orange  D8
 *  CLK  gelb    D5
 *  DIN  blau    D7
 *  GND  braun   G
 *  VCC  grau    3V3
 */
 
#ifdef Display_v2_ESP8266  // WEMOS D1 mini
#define EPD_BUSY D1
#define EPD_RESET D0
#define EPD_DC D2
#define EPD_CS D8
#define SPI_SCK D5
#define SPI_DIN D7

GxIO_Class io(SPI, /*CS*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RESET);
GxEPD_Class display(io, /*RST=*/EPD_RESET, /*BUSY=*/EPD_BUSY);

// GxIO_Class io(SPI, SS/* CS */, D2/* DC */, D0/* RST */);    
// GxEPD_Class display(io, D0, D1);                            
#endif

#ifdef Display_Wrist_v2_ESP32  // LILYGO® TTGO Wrist-E-Paper ESP32
#define SPI_SCK 14
#define SPI_DIN 13
#define EPD_CS 15
#define EPD_DC 2
//#define SRAM_CS -1
#define EPD_RESET 17
#define EPD_BUSY 16

#define PIN_MOTOR 4
#define PIN_KEY 35
#define PWR_EN 5
#define Backlight 33
#define Bat_ADC 34 
#endif

/* LILYGO® TTGO MINI ESP32
 *  
 *  Busy violett 4
 *  RST  weiss   16
 *  DC   grün    17
 *  CS   orange  5
 *  CLK  gelb    18
 *  DIN  blau    23
 *  GND  braun   GND
 *  VCC  grau    3V3
 */

#ifdef Display_v2_ESP32  // LILYGO® TTGO MINI ESP32
#define EPD_BUSY 4
#define EPD_RESET 16
#define EPD_DC 17
// #define SRAM_CS 5
#define EPD_CS 5
#define SPI_SCK 18
#define SPI_DIN 23
#endif

#ifdef Display_v2_bwr_ESP32  // LILYGO® TTGO ESP32
#define EPD_BUSY 4
#define EPD_RESET 16
#define EPD_DC 17
// #define SRAM_CS 5
#define EPD_CS 5
#define SPI_SCK 18
#define SPI_DIN 23
#endif

#ifdef ESP32
GxIO_Class io(SPI, /*CS*/ EPD_CS, /*DC=*/EPD_DC, /*RST=*/EPD_RESET);
GxEPD_Class display(io, /*RST=*/EPD_RESET, /*BUSY=*/EPD_BUSY);
#endif

//Schriftarten einbinden
#include <Adafruit_GFX.h>
#include <Fonts/Picopixel.h>
#include <Fonts/TomThumb.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

// #include "bilder.h"
#include "epdSplash.h"

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(mqttClient);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
  
}

// Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

   /*
    if(topic=="stats4loxberry/mqttgateway/keepaliveepoch"){
    //x, y, width, hight, color 
    //int ePoch = atoi(messageTemp);
    int ePoch = messageTemp.toInt();
    ePoch =((ePoch-1230768000)+28800);
    messageTemp=(String(ePoch));
    Serial.print(messageTemp);
    display.fillRect(60, 138, 140, 62, GxEPD_WHITE);
    display.setFont(&FreeSansBold24pt7b);
    display.setTextSize(1);
    display.setCursor(70, 185);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false); 
  }
  
    if(topic=="NodeRED/KWL_Stufe"){
    display.fillRect(57, 90, 43, 40, GxEPD_WHITE);
    display.setFont(&FreeSansBold24pt7b);
    display.setTextSize(1);
    display.setCursor(57, 126);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);    
  }

  if(topic=="NodeRED/KWL_Bypass"){
    display.fillRect(36, 70, 63, 20, GxEPD_WHITE);

    display.setFont(&FreeSans9pt7b);
    display.setTextSize(1);
    display.setCursor(36, 84);
    if(messageTemp == "bypass"){
      display.print("Bypass");
    }
    else if(messageTemp == "waermetauscher"){
      display.print("Warm");
    }
    display.updateWindow(0, 0, 200, 200, false);    
  } 
  */

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

  if(topic==mqttTopic_CO2){
    // x, y, width, hight, color
    display.fillRect(0, 25, 80, 41, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 55);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }

  if(topic==mqttTopic_Temp){
    display.fillRect(105, 20, 72, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(105, 55);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }

  if(topic==mqttTopic_Hum){
    // messageTemp += "%";
    display.fillRect(105, 90, 62, 40, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(105, 121);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }

 if(topic==mqttTopic_PVModule){
    // x, y, width, hight, color
    // messageTemp += "W";
    display.fillRect(5, 90, 95, 40, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 121);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);  
  }  

  if(topic==mqttTopic_Power){
    float comp = messageTemp.toFloat(); // String to float
    if (comp > 999.9){ // falls der Bezug größer als 9999.9 wird, die Schriftart verkleinern, damit das Nachbarfeld nicht überschrieben wird
    display.fillRect(5, 156, 95, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold12pt7b);
    display.setTextSize(1);
    display.setCursor(5, 190);
    messageTemp=(String(comp,1)); // float back to String with one decimal point
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
    } else {
    display.fillRect(5, 156, 95, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 190);
    messageTemp=(String(comp,1));
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
    }
  }

  /*if(topic==mqttTopic_ApartmentPresence){
    int ApartPresence = messageTemp.toInt();
    if ( ApartPresence == 1){
        Serial.println(ApartPresence);
        display.powerDown();
      } else {
        
      }  
  }  
  */

#ifdef Display_Wrist_v2_ESP32
if(topic==mqttTopic_Vibra){
    int Vibra = messageTemp.toInt();
    if ( Vibra == 1){
        Serial.println(Vibra);
        VibraMotor();
      } else {
        
      }  
  } 
#endif
  
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect(mqttClient, mqttUser, mqttPassword)) {
      Serial.println("connected");  
      // client.subscribe("NodeRED/KWL_Stufe");
      // client.subscribe("NodeRED/KWL_Bypass");
      client.subscribe(mqttTopic_CO2);
      client.subscribe(mqttTopic_Temp);
      client.subscribe(mqttTopic_Hum);
      client.subscribe(mqttTopic_PVModule);
      client.subscribe(mqttTopic_Power);
      // client.subscribe(mqttTopic_ApartmentPresence); 
       #ifdef Display_Wrist_v2_ESP32
      client.subscribe(mqttTopic_Vibra);    
       #endif
      } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // delay(5000);
      webota.delay(5000);
    }
  }
}

#ifdef ESP8266
char buffer[80];

void printLocalTime()
{
  time_t rawtime;
  struct tm * timeinfo;
  time (&rawtime);
  timeinfo = localtime (&rawtime);
  strftime (buffer,80, "%d.%m.%Y",timeinfo);
  Serial.print("Datum: ");
  Serial.println(buffer);
  strftime (buffer,80, "%H:%M",timeinfo);
  Serial.print("Uhrzeit: ");
  Serial.println(buffer);
  
  // x, y, width, hight, color
  display.fillRect(105, 154, 95, 52, GxEPD_WHITE);
  display.setFont(&FreeSansBold9pt7b);
  display.setTextSize(1);
  display.setCursor(105, 175);
  strftime (buffer,80, "%d.%m.%Y",timeinfo);
  display.print(buffer);
  display.setCursor(105, 192);
  strftime (buffer,80, "%H:%M",timeinfo);
  display.print(buffer);
  display.updateWindow(0, 0, 200, 200, false);  
}
#endif

#ifdef ESP32
void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  // Serial.println(&timeinfo, "%A, %d %B %Y %H:%M:%S");
  Serial.print("Datum: ");
  Serial.println(&timeinfo, "%d.%m.%Y");
  Serial.print("Uhrzeit: ");
  Serial.println(&timeinfo, "%H:%M");

  // x, y, width, hight, color
  display.fillRect(105, 154, 95, 52, GxEPD_WHITE);
  display.setFont(&FreeSansBold9pt7b);
  display.setTextSize(1);
  display.setCursor(105, 175);
  display.print(&timeinfo, "%d.%m.%Y");
  display.setCursor(105, 192);
  display.print(&timeinfo, "%H:%M");
  display.updateWindow(0, 0, 200, 200, false);
}
#endif

void showsplash()
{
  // Splashscreen 
  display.drawBitmap(gImage_epdSplash, 0, 0, 200, 200, GxEPD_BLACK);
  display.update();
  //webota.delay(10000);
  // display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  // Splashscreen
  display.setFont(&FreeSansBold9pt7b);
  display.setTextSize(1);
  display.setCursor(35, 60);
  display.println("ePaper Display");
  display.setCursor(45, 90);
  display.print("Version: "); 
  display.println(Version);
  display.setCursor(45, 110);
  display.println("by ThatRed");
  // display.setFont(&TomThumb);
  display.setCursor(45, 140);
  display.println("based on code");
  display.setCursor(45, 160);
  display.println("by klenzel");
  display.setTextColor(GxEPD_WHITE);
  display.setFont(&Picopixel);
  display.setCursor(65, 195);
  display.print(compile_date);
  display.update();
}


#ifdef Display_Wrist_v2_ESP32
void VibraMotor()
{
  pinMode(PIN_MOTOR, OUTPUT);
  digitalWrite(PIN_MOTOR, HIGH);
  webota.delay(200);
  digitalWrite(PIN_MOTOR, LOW);
  webota.delay(100);
  digitalWrite(PIN_MOTOR, HIGH);
  webota.delay(200);
  digitalWrite(PIN_MOTOR, LOW);
}
#endif

// Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

void setup() {
  Serial.begin(115200);
  setup_wifi();
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  MDNS.begin(mqttClient);
  webota.init(80, "/update");

  #ifdef Display_Wrist_v2_ESP32
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  VibraMotor();
  #endif

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED 
  display.init();
  display.fillScreen(GxEPD_WHITE);
  // display.update();
  display.setRotation(0);

  showsplash();
  
  webota.delay(SCDelay);
  
  display.fillScreen(GxEPD_WHITE);
  
  display.fillRect(100, 0, 2, 200, GxEPD_BLACK);
  display.fillRect(0, 66, 200, 2, GxEPD_BLACK);
  display.fillRect(0, 133, 200, 2, GxEPD_BLACK);
  // x, y, width, hight, color
  
  /*
  display.drawExampleBitmap(fan, 0, 76, 46 , 49, GxEPD_BLACK);
  display.drawExampleBitmap(uhr, 0, 140, 56 , 56, GxEPD_BLACK);
  display.drawExampleBitmap(temp2, 88, 4, 24 , 56, GxEPD_BLACK);
  display.drawExampleBitmap(hum2, 105, 73, 18 , 56, GxEPD_BLACK);
  */

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);
  display.setCursor(5, 17);
  display.print("CO   ppm");
  // leider wird die ₂ in CO₂ nicht dargestellt, daher wird die 2 etwas nach unten verschoben:
  display.setCursor(32, 22);
  display.print("2");
  display.setCursor(105, 17);
  display.print("Nord  C");
  display.setCursor(105, 84);
  display.print("Nord %");
  display.setCursor(5, 84);
  display.print("PV W");
  display.setCursor(5, 151);
  display.print("Bezug W");
  display.setCursor(105, 151);
  display.print("Date&Time");

  // display.fillCircle(80, 30, 5, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  // display.fillCircle(80, 30, 2, GxEPD_WHITE);  //Xpos,Ypos,r,Farbe

  // leider wird das ° Zeichen in °C nicht dargestellt, daher wird es gezeichnet:
  display.fillCircle(151, 6, 2, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  display.fillCircle(151, 6, 1, GxEPD_WHITE);  //Xpos,Ypos,r,Farbe
 
  display.update();
  Serial.println("setup done");

}

void loop() {
 
  if (!client.connected()) {
    reconnect();
  }
  
  if(!client.loop())
    client.connect(mqttClient);

  webota.handle();
  
  unsigned long currentMillis = millis();

  // Uhrzeit jede Minute updaten
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the time
    previousMillis = currentMillis;

    // if the timeState is 0 set it to 1 and vice-versa:
    if (timeState == 0) {
      timeState = 1;
     } else {
      timeState = 0;
     } 
      printLocalTime();
  }

  unsigned long wificurrentMillis = millis();
  // if WiFi is down, try reconnecting every CHECK_WIFI_TIME seconds
  if ((WiFi.status() != WL_CONNECTED) && (wificurrentMillis - wifipreviousMillis >=wifiinterval)) {
    Serial.print(millis());
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    wifipreviousMillis = wificurrentMillis;
  }
} 
