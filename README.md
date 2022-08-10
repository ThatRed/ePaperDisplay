 *  ePaper Display (EPD) by ThatRed based on klenzel's code
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
 *  Im Boardverwalter für ESP32, Version 2.0.4 installieren.
 *
 *  Den DC to DC Converter auf 5V einstellen.
 *  
 *  3D-Dateien: 
 *  Thingiverse: https://www.thingiverse.com/thing:5402812
 *  Gira System 55 EePaper Display Wrist flush: https://www.tinkercad.com/things/iELsnfjnHlb  
 *  Gira System 55 EePaper Display Wrist: https://www.tinkercad.com/things/0SN0mNvNaRC
 *  Gira System 55 EePaper Display 3: https://www.tinkercad.com/things/0aLU2I10yRt
 *
 *  LILYGO® TTGO Wrist-E-Paper ESP32: https://a.aliexpress.com/_uvCWAF
 *  Wichtig! Es wird ein Programmieradapter benötigt! Der USB-C Port auf der Platine ist kein echter USB-Port!!
 *  MP1584EN DC-DC 3A Buck Converter (24V->5V): https://www.amazon.de/dp/B07MY2XP21/ref=cm_sw_r_cp_api_i_C4W4QEAWF79WXW4AJQAR?_encoding=UTF8&psc=1
 *
 *  1.7: Wifi Part etwas überarbeitet
