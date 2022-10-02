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
 *  Adafruit_GFX.h
 *  Fonts/Picopixel.h
 *  Fonts/FreeSans9pt7b.h
 *  Fonts/FreeSansBold9pt7b.h
 *  Fonts/FreeSansBold12pt7b.h
 *  Fonts/FreeSansBold18pt7b.h
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
 *  Im Boardverwalter für ESP32, Version 2.0.4 installieren.
 *  
 *  3D-Dateien: https://www.thingiverse.com/thing:5402812
 *  
 *  Anfang benutzerdefinierte Konfiguration:
 *
 */ 
//
// Benutzerkonfiguration
//
//
// Wifi
//
const char* ssid = "SSID";
const char* password = "pass";
//
// MQTT
//
//
// Welcher MQTT Raum?
//
//#define Az
//#define Sz
//#define Wz
#define Kl
//
// MQTT Server
//
const char* mqtt_server = "192.168.xxx.xxx";
const char* mqttUser = "user";
const char* mqttPassword = "pass";
//
// MQTT Topics
//
//
// MQTT Räume
//
#ifdef Az
const char* mqttClient = "ePaperDisplayAz";
String hostname = "Az";
const char* mqttTopic_CO2 = "myhome/Az/CO2";
const char* mqttTopic_Vibra = "myhome/Az/Vibra";
const char* mqttTopic_PVModule = "myhome/Balkon/PV-Module";
const char* mqttTopic_Power = "myhome/Wohnung/Bezug";
#endif
//
#ifdef Sz
const char* mqttClient = "ePaperDisplaySz";
String hostname = "Sz";
const char* mqttTopic_CO2 = "myhome/Sz/CO2";
const char* mqttTopic_Vibra = "myhome/Sz/Vibra"; 
const char* mqttTopic_PVModule = "myhome/Balkon/PV-Module";
const char* mqttTopic_Power = "myhome/Wohnung/Bezug";
#endif
//
#ifdef Wz
const char* mqttClient = "ePaperDisplayWz";
String hostname = "Wz";
const char* mqttTopic_CO2 = "myhome/Wz/CO2";
const char* mqttTopic_Vibra = "myhome/Wz/Vibra"; 
const char* mqttTopic_PVModule = "myhome/Balkon/PV-Module";
const char* mqttTopic_Power = "myhome/Wohnung/Bezug"; 
#endif
//
#ifdef Kl
const char* mqttClient = "ePaperDisplayKl";
String hostname = "Kl";
const char* mqttTopic_K_Hum = "myhome/Kl/hum";
const char* mqttTopic_Vibra = "myhome/Kl/Vibra";
const char* mqttTopic_B_Hum = "myhome/B/hum";
const char* mqttTopic_J_Hum = "myhome/J/hum";  
#endif
//
// MQTT allgemein
//
const char* mqttTopic_Hum = "myhome/Nord/Hum";
const char* mqttTopic_Temp = "myhome/Nord/Temp";
// const char* mqttTopic_ApartmentPresence = "myhome/Wohnung/AnAbwesendheit";
//
// NTP Server
//
const char* ntpServer = "192.168.xxx.xxx";
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
// #define Display_v2_ESP32        //bw Version >=2
#define Display_Wrist_v2_ESP32  //bw Version >=2
// #define Display_v2_ESP8266      //bw Version >=2
//
// #define Display_v2_bwr_ESP32    //bwr Version <2, leider noch keine Unterstützung für die neuen Tri-Color Displays in GxEPD.h!!
// #define Display_v1_ESP8266      //bw Version <2, ungetestet!!
//
// Ende Benutzerkonfiguration
//

const long interval = 60000;
int timeState = 0;
int restartState = 0;
unsigned long previousMillis = 0; 
unsigned long wifipreviousMillis = 0;
unsigned long restartpreviousMillis = 0;
unsigned long wifiinterval = 30000;
unsigned long restartinterval = 30000;

String Version = "1.7.6";
const char compile_date[] = __DATE__ " " __TIME__;

//
// Customized HTML Code for WebOTA update site:
//
String epD = "<h1>ePaper Display " + hostname + " Version: " + Version + "</h1>";

//
// Im Boardverwalter für ESP32, Version 2.0.4 installieren.
//
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
  #include "WiFi.h"
  #include <ESPmDNS.h> 
#else
  #error "Nur für Entwicklungsboards basierend auf ESP8266 oder ESP32!"
#endif

//
// Display Konfigurationsfehler abfangen
//
#ifdef ESP8266
#ifdef Display_Wrist_v2_ESP32
#error "ePaper Wrist funktioniert nur mit ESP32!"
#endif
#endif

#ifdef ESP8266 
#ifdef Display_v2_bwr_ESP32
#error "ePaper Display_v2_bwr funktioniert nur mit ESP32!"
#endif
#endif

#ifdef ESP32
#ifdef Display_v2_ESP8266
#error "ePaper Display_v2_ESP8266 funktioniert nur mit ESP8266!"
#endif
#endif

#ifdef ESP32
#ifdef Display_v1_ESP8266
#error "ePaper Display_v1_ESP8266 funktioniert nur mit ESP8266!"
#endif
#endif

#ifdef ESP32
#include "nvs_flash.h"
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
//#include <Fonts/TomThumb.h>
#include <Fonts/FreeSans9pt7b.h>
//#include <Fonts/FreeSans12pt7b.h>
//#include <Fonts/FreeSans18pt7b.h>
//#include <Fonts/FreeSans24pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
//#include <Fonts/FreeSansBold24pt7b.h>

#include "epdSplash.h"
int status = WL_IDLE_STATUS;
WiFiClient espClient;
PubSubClient client(espClient);

String get_wifi_status(int status){
    switch(status){
        case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
        case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
        case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
        case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
        case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
        case WL_CONNECTED:
        return "WL_CONNECTED";
        case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    }
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  Serial.print("wifi status: ");
  Serial.println(get_wifi_status(status));
  WiFi.mode(WIFI_STA);
  WiFi.hostname(mqttClient);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    unsigned long restartcurrentMillis = millis();
    status = WiFi.status();
    Serial.print("wifi status: ");
    Serial.println(get_wifi_status(status));
    //Serial.print(".");
    delay(500);
    WiFi.disconnect();
    delay(500);
    WiFi.reconnect();
    delay(500);
    // restart nach einer Minute updaten
    if (restartcurrentMillis - restartpreviousMillis >= restartinterval) {
    // save the last time you updated the time
      restartpreviousMillis = restartcurrentMillis;
    // if the timeState is 0 set it to 1 and vice-versa:
        if (restartState == 0) {
            restartState = 1;
            } else {
               restartState = 0;
            } 
         #ifdef ESP32
         ESP_ERROR_CHECK(nvs_flash_erase());
         nvs_flash_init();
         #endif
         
         delay(500);
         ESP.restart();
        }   
      }  
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP()); 
}

// Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

#ifdef Display_Wrist_v2_ESP32
void VibraMotor(){
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

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

 #ifndef Kl
  if(topic==mqttTopic_CO2){
    // x, y, width, hight, color
    display.fillRect(0, 25, 80, 41, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 55);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }

 if(topic==mqttTopic_PVModule){
    // x, y, width, hight, color
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
    // x, y, width, hight, color
    display.fillRect(5, 156, 95, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold12pt7b);
    display.setTextSize(1);
    display.setCursor(5, 190);
    messageTemp=(String(comp,1)); // float back to String with one decimal point
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
    } else {
    // x, y, width, hight, color
    display.fillRect(5, 156, 95, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 190);
    messageTemp=(String(comp,1));
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
    }
  }
 #endif

 #ifdef Kl
    if(topic==mqttTopic_K_Hum){
    // x, y, width, hight, color
    display.fillRect(5, 90, 95, 40, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 121);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);  
  }  
    
    if(topic==mqttTopic_B_Hum){
    // x, y, width, hight, color
    display.fillRect(5, 156, 95, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 190);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false); 
  }
        
    if(topic==mqttTopic_J_Hum){
    // x, y, width, hight, color
    display.fillRect(0, 25, 80, 41, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(5, 55);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }   
 #endif

  if(topic==mqttTopic_Temp){
    // x, y, width, hight, color
    display.fillRect(105, 20, 72, 44, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(105, 55);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
  }

  if(topic==mqttTopic_Hum){
    // x, y, width, hight, color
    display.fillRect(105, 90, 62, 40, GxEPD_WHITE);
    display.setFont(&FreeSansBold18pt7b);
    display.setTextSize(1);
    display.setCursor(105, 121);
    display.print(messageTemp);
    display.updateWindow(0, 0, 200, 200, false);
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
    Serial.println("Attempting MQTT connection...");
    if (client.connect(mqttClient, mqttUser, mqttPassword)) {
      Serial.println("connected");  
     
     #ifndef Kl 
      client.subscribe(mqttTopic_CO2);
      client.subscribe(mqttTopic_PVModule);
      client.subscribe(mqttTopic_Power);
     #endif
     
     #ifdef Kl
      client.subscribe(mqttTopic_K_Hum);
      client.subscribe(mqttTopic_B_Hum);
      client.subscribe(mqttTopic_J_Hum);
     #endif 
      
      client.subscribe(mqttTopic_Temp);
      client.subscribe(mqttTopic_Hum);
      // client.subscribe(mqttTopic_ApartmentPresence); 
       
     #ifdef Display_Wrist_v2_ESP32
      client.subscribe(mqttTopic_Vibra);    
     #endif
      
      } else {
      status = WiFi.status();
      Serial.print("wifi status: ");
      Serial.println(get_wifi_status(status));
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      WiFi.disconnect();
      delay(500);
      WiFi.reconnect();
      delay(500);
      if (status == WL_DISCONNECTED) {
        //exit;
        return;
        }
      }
      // delay(5000);
      webota.delay(5000);
    }
  }
//}

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


// Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED

void setup() {
  Serial.begin(115200);
  #ifdef ESP32
  WiFi.disconnect(false,true);
  #endif
  WiFi.setSleep(false);
  setup_wifi();
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  MDNS.begin(mqttClient);
  
  webota.init(80, "/update");
  webota.set_custom_html(epD.c_str());
  
  #ifdef Display_Wrist_v2_ESP32
  SPI.begin(SPI_SCK, -1, SPI_DIN, EPD_CS);
  VibraMotor();
  #endif

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED 
  Serial.println("ePaper init");
  display.init();
  display.fillScreen(GxEPD_WHITE);
  display.setRotation(0);
  Serial.println("Show splash screen");
  showsplash();
  
  webota.delay(SCDelay);
  
  Serial.println("ePaper init");
  display.fillScreen(GxEPD_WHITE);

  Serial.println("Show grid");
  display.fillRect(100, 0, 2, 200, GxEPD_BLACK);
  display.fillRect(0, 66, 200, 2, GxEPD_BLACK);
  display.fillRect(0, 133, 200, 2, GxEPD_BLACK);
  // x, y, width, hight, color

  // Farben: GxEPD_WHITE, GxEPD_BLACK, GxEPD_RED
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSans9pt7b);
  display.setTextSize(1);

  #ifndef Kl
  display.setCursor(5, 17); 
  display.print("CO   ppm");
  // leider wird die ₂ in CO₂ nicht dargestellt, daher wird die 2 etwas nach unten verschoben:
  display.setCursor(32, 22);
  display.print("2");
  display.setCursor(5, 84);
  display.print("PV W");
  display.setCursor(5, 151);
  display.print("Bezug W");
  #endif
  
  #ifdef Kl
  display.setCursor(5, 17); 
  display.print("JVTech %");
  display.setCursor(5, 84);
  display.print("GZahler %");
  // leider wird das ä nicht dargestellt, daher werden die Punkte gezeichnet:
  display.fillCircle(34, 72, 1, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  display.fillCircle(36, 72, 1, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  display.setCursor(5, 151);
  display.print("Bunker %");
  #endif
  
  display.setCursor(105, 151);
  display.print("Date&Time");
  display.setCursor(105, 84);
  display.print("Nord %");
  display.setCursor(105, 17);
  display.print("Nord  C");
  // leider wird das ° Zeichen in °C nicht dargestellt, daher wird es gezeichnet:
  display.fillCircle(151, 6, 2, GxEPD_BLACK);  //Xpos,Ypos,r,Farbe
  display.fillCircle(151, 6, 1, GxEPD_WHITE);  //Xpos,Ypos,r,Farbe
  
  display.update();
  Serial.println("setup done");

}

void loop() {

   // status = WiFi.status();
   // Serial.println(get_wifi_status(status));
 
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
    status = WiFi.status();
    Serial.println(get_wifi_status(status));
    delay(500);
    Serial.println("Reconnecting to WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();
    delay (5000);
    wifipreviousMillis = wificurrentMillis;
  }
} 
