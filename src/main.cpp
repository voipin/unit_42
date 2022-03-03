#include <Wire.h>
#include <U8g2lib.h>
#include <stdio.h>
#include "Adafruit_SGP30.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "FS.h"
#include <pu2clr_pcf8574.h>
#include <string.h>
#include <Adafruit_NeoPixel.h>


PCF pcf;

ESP8266WebServer server(80);

using namespace std;

#include "DHT.h"

#define DHTTYPE DHT22




// Set WiFi credentials
//#define WIFI_SSID "KidsWifi"
//#define WIFI_PASS "xxyy123456"

// Set WiFi credentials
#define AP_SSID "mush-room"
#define AP_PASS "8675309"


Adafruit_SGP30 sgp;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

const char* ssid = "mush-room";
const char* pass = "86753099";

//const char* wifi_ssid = "KidsWifi";
//const char* wifi_pass = "xxyy123456";

vector <String> wifi_data;
vector <String> ap_data;
vector <String> limit_data;
vector <String> time_data;
vector <String> light_data;

uint8_t history[1024];

uint8_t data1;
uint8_t data2;
uint8_t data3;
uint8_t data_state1;
uint8_t data_state2;
uint8_t data_state3;

uint8_t light_off_hour;
uint8_t light_off_minute;
uint8_t light_on_hour;
uint8_t light_on_minute;
bool light_manual;
bool fan_manual;
bool hum_manual;
bool light_auto;
bool fan_auto;
bool hum_auto;
bool manual_override;



void handleRoot();
void handleNotFound();
void readStoredData();
bool enableWifi();
void handleForm();
void fileindex();
void bootstrap();
//void handle_RelayOn();
//void handle_RelayOff();
void readTemp();
void writeStoredWifiData();
void writeStoredApData();
void jquery();
void jquerySlim();
void jqueryFull();
void popper();
void bootstrapmin();
void updateSensor();
void slide();
void jsRelayControl();
void timeTrack();
void writeStoredTimeData();
void writeStoredLimitData();
void writeStoredLightData();
void handle_LightOn();
void handle_LightOff();
void handle_HumidityOn();
void handle_HumidityOff();
void handle_FanOn();
void handle_FanOff();
void FactoryResetStoredData();

//void lightStatus();
//void lightClockStop();
//void lightClockStart();
void jsWindowLoad();
void manualOverrideOn();
void manualOverrideOff();
void allWhite();
void theaterChase(uint32_t color, int wait);


String clockSet(String clockData);
String clockOutput();





// const char COPYRIGHT_SYMBOL[] = { 0xa9, '\0' };
void u8g2_prepare() {
u8g2.setFont(u8g2_font_helvB18_tf); u8g2.setFontRefHeightExtendedText(); u8g2.setDrawColor(1); u8g2.setFontPosTop(); u8g2.setFontDirection(0);
}





char *menuDisplay[] = {"one", "two", "three"};
char *charSet[]= {"A", "B", "C"};

//char *menuDisplay[] = {};

/* return absolute humidity [mg/m^3] with approximation formula
* @param temperature [°C]
* @param humidity [%RH]
*/
uint8_t DHTPin = D6;
uint8_t LEDPin = D5;
uint8_t FANPin = D7;
uint8_t HUMIDITYPin = D8;



DHT dht(DHTPin, DHTTYPE); 

#define LED_PIN D5
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 20

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
//const int Push_button_1 = 13;

uint8_t Push_button_1 = D5;
uint8_t Push_button_2 = D6;
//uint8_t Push_button_3 = D7;

uint8_t Relay1 = D8;


int loopTime = 1000;
bool displayed = 0;

int menu_state = 0;
int menuSize = 0 ;
int charCount = 65;
int totalChars = 0;

String state_menu = "sensor_data";
String stateMenu = "sensor_data";

char wifi_sid[10];
String wifi_pw = "";
char wifi_sid_add[0];

float Temperature;
float Humidity;

int alert_temp = 85;
int alert_co2 = 600;
int alert_hum = 60;

int relay_temp = 85;
int relay_co2 = 600;
int relay_hum = 50;


bool relay_hum_toggle = 0;
bool relay_fan_toggle = 0;

//time keeping variables
unsigned long timeNow = 0;
unsigned long timeLast = 0;


int startingHour = 18; // set your starting hour here, not below at int hour. This ensures accurate daily correction of time

int seconds = 0;
int minutes = 5;
int hours = startingHour;
int days = 0;

String tz;

//time accuracy settings

int dailyErrorFast = 0; // set the average number of milliseconds your microcontroller's time is fast on a daily basis
int dailyErrorBehind = 0; // set the average number of milliseconds your microcontroller's time is behind on a daily basis

int correctedToday = 1; // do not change this variable, one means that the time has already been corrected today for the error in your boards crystal. This is true for the first day because you just set the time when you uploaded the sketch.  

String warnHTML(String alertText) {

  String ptr="<script>\n";
  ptr += "alert( \" ";
  ptr += alertText;
  ptr += " \") \n ";
  ptr += "window.location =\"/\"";
  ptr += "</script>\n";

  return ptr;
}


	const unsigned char unit42 [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xE0, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xE0, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0x30, 0x03, 0x00, 0x0C, 0xF0, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xC0, 0x03, 0x30, 0x03, 0x10, 0x1C, 0xB8, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x30, 0x00, 0x30, 0x1C, 0x18, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0xE3, 0x78, 0x2C, 0x18, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x30, 0x2C, 0x18, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x30, 0x6C, 0x30, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x30, 0xCC, 0x30, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x30, 0xFE, 0x60, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x30, 0x0C, 0x60, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x03, 0x23, 0x33, 0x30, 0x0C, 0xC0, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0xC3, 0x33, 0x18, 0x0C, 0xF8, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x74, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xA4, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xA4, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xA4, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x2A, 0xA4, 0xAA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x11, 0x14, 0xA6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};




















String choiceHTML(String alertText,String choice1, String choice2) {

String ptr="<!DOCTYPE html><script>\n";
ptr += "function onLoad() {\n";
ptr += "console.log(\"----no wireless configurations----\");\n";
ptr += " result = getConfirmation(\"";
ptr += alertText;
ptr += "\");\n";
ptr += "function getConfirmation(message) {\n";
ptr += "var retVal = confirm(message);\n";
ptr += " if (retVal == true) {\n";
ptr += "window.location=\"/";
ptr += choice1;
ptr += "\";\n";
ptr += " } else {\n";
ptr += "window.location=\"/";
ptr += choice2;
ptr += "\"; \n";
ptr += "}\n";
ptr += "}\n";
ptr += "}\n";
ptr += "onLoad();\n";
ptr +="</script>";

return ptr;
 

}

String updateSensorAjax(int Temperature, int Humidity, int co2){

  timeTrack();
  String new_time = clockOutput();

  String ptr ="<ul class=\"list-inline\" >\n";
 
          ptr +="                    <li>Temp ";
          ptr += Temperature;
          ptr +="                     </li>\n";
          ptr +="                    <li>Co2 ";
          ptr += co2;
          ptr +="                     ppm</li>\n";
          ptr +="                    <li>Humidity ";
          ptr += Humidity;
          ptr +="                     %</li>\n";
          ptr +="                </ul>\n";
          ptr +="<li> Time ";
          ptr +=new_time;
  return ptr;
}

String sendHTML(String form_relay_toggle_fan, vector <String> wifi_data, int Temperature, int Humidity, int co2, vector <String> ap_data, String new_time, String tz) {

String ptr ="<!doctype html>\n";
ptr +="<html lang=\"en\">\n";
ptr +="<head>\n";
ptr +="    <meta charset=\"utf-8\">\n";
ptr +="    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1, shrink-to-fit=no\">\n";
ptr +="    <meta name=\"description\" content=\"\">\n";
ptr +="    <meta name=\"author\" content=\"\">\n";
ptr +="    <link rel=\"icon\" href=\"/docs/4.0/assets/img/favicons/favicon.ico\">\n";
ptr +="    <title>Unit 42</title>\n";
ptr +="    \n";
ptr +="    <!-- Bootstrap core CSS -->\n";
ptr +="    <link href=\"bootstrap.min.css\" rel=\"stylesheet\">\n";
ptr +="    <!-- Custom styles for this template -->\n";
ptr +="    <link href=\"slide.css\" rel=\"stylesheet\">\n";
ptr +="</head>\n";
ptr +="<body>\n";
ptr +="<style>\n";
ptr +=".time {\n";
ptr +="        margin: 10px 10px; \n";
ptr +="    }\n";
ptr +="    table {\n";
ptr +="        width:100%;\n";
ptr +="    }\n";
ptr +="    li {\n";
ptr +="        text-align: left;\n";
ptr +="        margin: 5px 0;\n";
ptr +="    }\n";
ptr +="    .li-more-margin {\n";
ptr +="        margin: 10px 0;\n";
ptr +="    }\n";
ptr +="    .relay-text{\n";
ptr +="        text-align: left;\n";
ptr +="        width:75%;\n";
ptr +="    }\n";
ptr +="    .relay-input{\n";
ptr +="        width:25%;\n";
ptr +="    }\n";
ptr +="    .btn {\n";
ptr +="        margin-top: 30px;\n";
ptr +="    }\n";
ptr +="    .temp-input{\n";
ptr +="        width:25%;\n";
ptr +="    }\n";
ptr +="    .card-body {\n";
ptr +="        align-content: center;\n";
ptr +="    }\n";
ptr +="    input {\n";
ptr +="        height: 30px;\n";
ptr +="        flex: 0 0 200px;\n";
ptr +="        margin-left: 10px;\n";
ptr +="    }\n";
ptr +="</style>\n";
ptr +="<script src=\"jquery1.js\"></script>\n";
ptr +="<script src=\"relay_control.js\"></script>\n";
ptr +="<script src=\"window_load.js\"></script>\n";
ptr +="<script>";

ptr +="var intervalID = setInterval(function() {";

ptr +="            $(\"#sensor_update\").load(\"update_sensor\");";
ptr +="            console.log(\"update sensors\");";

ptr +="    }, 5000);";





ptr +="</script>";



ptr +="<div class=\"form-group\">\n";
ptr +="<form id=\"control_form\" class=\"form-group\" action=\"/submit_page\">\n";
ptr +="<input name=\"clockset\" type=\"hidden\" id=\"clockset\" value=0 >\n";
ptr +="<div class=\"pricing-header px-3 py-3 pt-md-5 pb-md-4 mx-auto text-center\">\n";
ptr +="    <h1 class=\"display-4\">Control</h1>\n";
ptr +="    <p class=\"lead\">Control all systems</p>\n";
ptr +="</div>\n";
ptr +="<div class=\"container\">\n";
ptr +="    <div class=\"card-deck text-center\">\n";
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Readings</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"card-body mx-auto\">\n";
ptr +="                <ul id=\"sensor_update\" class=\"list-inline\" >\n";
ptr +="                    <li>Temp ";
ptr += Temperature;
ptr +="                     </li>\n";
ptr +="                    <li>Co2 ";
ptr += co2;
ptr +="                     ppm</li>\n";
ptr +="                    <li>Humidity ";
ptr += Humidity;
ptr +="                     %</li>\n";
ptr +="                </ul>\n";
ptr +="            </div>\n";
ptr +="        </div>\n";
//start set clock
ptr +="         <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\"> \n";
ptr +="         <div class=\"card-header\"> \n";
ptr +="         <h4 class=\"my-0 font-weight-normal\">Set Clock</h4> \n";
ptr +="         </div>\n";
ptr +="         <div class=\"card-body mx-auto\"> \n";
ptr +="         <input name=\"clock\" type=\"time\" value=\"";
ptr += new_time;
ptr +="\"> ";

ptr += "<div class=\"time\" ><label for=\"tz\">TimeZone :</label></div>\n";
ptr +="                   <select name=\"tz\" id=\"tz\">\n";
if (tz=="est"){ptr +="                     <option value=\"est\" selected>EST</option>\n";}
else {ptr +="                     <option value=\"est\">EST</option>\n";}

ptr +="                     <option value=\"cst\">CST</option>\n";
ptr +="                     <option value=\"mst\">MST</option>\n";
if (tz=="pst"){ ptr +="                     <option value=\"pst\" selected>PST</option>\n";}
else { ptr +="                     <option value=\"pst\">PST</option>\n";}
ptr +="                     <option value=\"utc\">UTC</option>\n";
ptr +="                     </select>\n";
ptr +="  <div class=\"time\"> \n";
ptr +="            <button type=\"button\" onclick=\"clockSet()\" class=\"btn btn-lg btn-block btn-primary\">Set Clock</button></div>\n";
ptr +="               </div>\n";

ptr +="           </div> \n";
// end set clock

//wifi setup
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\" >\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Wifi</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"card-body mx-auto\"\">\n";
ptr +="                <ul class=\"list-unstyled\">\n";
    if (wifi_data[0] == "on") {
        ptr +=" <li class=\"li-more-margin\">Enable Local Network : <label class=\"switch\"> <input name=\"cl_active\" type=\"checkbox\"/ checked>  <span class=\"slider round\"></span> </label> </li>\n";
         }else {
        ptr +="                    <li class=\"li-more-margin\">Enable Local Network : <label class=\"switch\"> <input name=\"cl_active\" type=\"checkbox\"/> <span class=\"slider round\"></span> </label> </li>\n";
        }
ptr +="                    <li>SSID <input value=\"";
ptr += wifi_data[1];
ptr += "\" name=\"cl_ssid\" size=\"10\"/></li>\n";

ptr +="                    <li>Pass <input value=\"";
ptr +=wifi_data[2];
ptr += "\" name=\"cl_pass\" size=\"10\"/></li>\n";
ptr +="                </ul>\n";
ptr +="                <ul class=\"list-unstyled\">\n";
if (ap_data[0] == "on") {
ptr +="                    <li class=\"li-more-margin\">Enable AP Network :  <label class=\"switch\"><input class=\"c\" name=\"ap_active\" type=\"checkbox\"/ checked><span class=\"slider round\"></span></li>\n";
}else{
ptr +="                    <li class=\"li-more-margin\">Enable AP Network : <label class=\"switch\"><input class=\"c\" name=\"ap_active\" type=\"checkbox\"/ ><span class=\"slider round\"></span></li>\n";
}
ptr +="                    <li>SSID <input value=\"";
ptr +=ap_data[1];
ptr += "\" name=\"ap_ssid\" size=\"10\"/></li>\n";

ptr +="                    <li>Pass <input value=\"";
ptr +=ap_data[2];
ptr += "\" name=\"ap_pass\" size=\"10\"/></li>\n";               
ptr +="                </ul>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="            </div>\n";
ptr +="        </div>\n";
// wifi setup

// light setup
ptr +="  <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="              <div class=\"card-header\">\n";
ptr +="               <h4 class=\"my-0 font-weight-normal\">Light</h4>\n";
ptr +="              </div>\n";
ptr +="              <div class=\"card-body mx-auto time\">\n";
ptr +="                  <div class=\"time\" >Light on<input name=\"light_on\" type=\"time\" value=\"";
ptr += light_data[0];
ptr += "\"></div>\n";
ptr +="                  <div class=\"time\" >Light off<input name=\"light_off\" type=\"time\" value=\"";
ptr +=light_data[1];
ptr +="\"></div>\n";
if (light_data[2]=="on") {
ptr +="                  <div class= \"time\"> Alert strobe <label class=\"switch\"><input name=\"light_strobe\" type=\"checkbox\" checked><span class=\"slider round\"></span> </label></div>\n";
}else{
  ptr +="                  <div class= \"time\"> Alert strobe <label class=\"switch\"><input name=\"light_strobe\" type=\"checkbox\"><span class=\"slider round\"></span> </label></div>\n";
}
if (light_auto==true) {
ptr +="                  <div class= \"time\"> Light On/Off <label class=\"switch\"><input name=\"light_strobe\" id=\"light\" type=\"checkbox\" checked><span class=\"slider round\"></span> </label></div>\n";
}else{
  ptr +="                  <div class= \"time\"> Light On/Off <label class=\"switch\"><input  id=\"light\" name=\"light_status\" type=\"checkbox\"><span class=\"slider round\"></span> </label></div>\n";
}

ptr +="                  <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="            </div>\n";

ptr +="          </div>\n";
//end light setup




ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Relay Limit</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"row\">\n";
ptr +="            <div class=\"card-body mx-auto\">\n";

ptr +="                <ul class=\"list-unstyled\">\n";

ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                    <table>\n";
ptr +="                        <tr>\n";
ptr +="                            <td class=\"relay-text\">Temp</td>\n";
ptr +="                            <td class=\"temp-input\" ><input name=\"relay_limit_temp\" size=\"3\" value=\" ";
ptr += limit_data[0];
ptr += " \"/> </td>\n";
ptr +="                        </tr>\n";
ptr +="                    </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Co2 </td>\n";
ptr +="                                <td class=\"relay-input\"> <input size=\"3\" name=\"relay_limit_co2\" value=\" ";
ptr += limit_data[1];
ptr += "\"/> </td>\n";
ptr +="                            </tr>\n";
ptr +="                        </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Humidity </td>\n";
ptr +="                                <td class=\"relay-input\"> <input name=\"relay_limit_hum\" size=\"3\" value=\" ";
ptr += limit_data[2];
ptr += "\"/> </td>\n";
ptr +="                            </tr>\n";
ptr +="                        </table>\n";
ptr +="                    </li>\n";



ptr +="                </ul>\n";

ptr +="            </div>\n";

ptr +="        </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";

ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\">\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Relay Toggle</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"row\">\n";




ptr +="                <div class=\"card-body mx-auto\">\n";

ptr +="                 <div class=\"time\"> ";
ptr +="                 <div class=\"relay-text\">Manual</div>\n";
if (manual_override == true) {
ptr +="                 <div class=\"relay-input\"><label class=\"switch\"><input id=\"manual_override\" value=\"on\" name=\"manual_override\" type=\"checkbox\" checked/><span class=\"slider round\"></span></div>\n";
}else{

  ptr +="                 <div class=\"relay-input\"><label class=\"switch\"><input id=\"manual_override\" value=\"on\" name=\"manual_override\" type=\"checkbox\"/><span class=\"slider round\"></span></div>\n";

}

ptr +="                    <ul class=\"list-group\">\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\">Fan</td>\n";
if (fan_auto == true){
  ptr +="                                    <td class=\"relay-input\"><label class=\"switch\"><input id=\"fan\" value=\"on\" name=\"relay_toggle_fan\" type=\"checkbox\" checked/><span class=\"slider round\"></span></td>\n";

  }else
  {
    ptr +="                                    <td class=\"relay-input\"> <label class=\"switch\"><input id=\"fan\" name=\"relay_toggle_fan\" type=\"checkbox\" /><span class=\"slider round\"></span></td>\n";
    }

ptr +="                                </tr>\n";

ptr +="                            </table>\n";
ptr +="                            <span class=\"badge badge-primary badge-pill\">14</span>\n";
ptr +="                        </li>\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";

ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\" >Humidity </td>\n";
if (hum_auto == true ){
ptr +="                                    <td class=\"relay-input\"> <label class=\"switch\"><input id=\"humidity\" name=\"relay_toggle_hum\" type=\"checkbox\" checked /> <span class=\"slider round\"></span></td>\n";
}else{
  ptr +="                                    <td class=\"relay-input\"> <label class=\"switch\"><input id=\"humidity\" name=\"relay_toggle_hum\" type=\"checkbox\" /> <span class=\"slider round\"></span></td>\n";

}
ptr +="                                </tr>\n";

ptr +="                            </table>\n";

ptr +="                            <span class=\"badge badge-primary badge-pill\">2</span>\n";
ptr +="                        </li>\n";

ptr +="                    </ul>\n";

ptr +="                </div>\n";

ptr +="            </div>\n";

ptr +="        </div>\n";
ptr +="</form>\n";
ptr += " <div id=\"fan_action\" hidden ></div>";
ptr += " <div id=\"light_action\" hidden ></div>";
ptr += " <div id=\"humidity_action\" hidden ></div>";
ptr += " <div id=\"manual_action\" hidden ></div>";
ptr +="    <footer class=\"pt-4 my-md-5 pt-md-5 border-top\">\n";
ptr +="        <div class=\"row\">\n";
ptr +="            <div class=\"col-12 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="            <div class=\"col-6 col-md\">\n";

ptr +="            </div>\n";
ptr +="        </div>\n";
ptr +="    </footer>\n";
ptr +="</div>\n";



ptr +="<!-- Bootstrap core JavaScript\n";
ptr +="================================================== -->\n";
ptr +="<!-- Placed at the end of the document so the pages load faster -->\n";

ptr +="<script src=\"popper.min.js\"></script>\n";
ptr +="<script src=\"bootstrap.min.js\"></script>\n";
ptr +="<script src=\"holder.min.js\"></script>\n";
ptr +="<script>\n";
ptr +="    Holder.addTheme('thumb', {\n";
ptr +="        bg: '#55595c',\n";
ptr +="        fg: '#eceeef',\n";
ptr +="        text: 'Thumbnail'\n";
ptr +="    });\n";
ptr +="</script>\n";
ptr +="</body>\n";
ptr +="</html>\n";

return ptr;

}

void limitStatus() {

  int temp;
  int carbon_dioxide;
  int hum;

  temp = limit_data[0].toInt();
  carbon_dioxide = limit_data[1].toInt();
  hum = limit_data[2].toInt();

  if (manual_override == true) {
    //Serial.println("manual override active");
    return;
  }

  //Serial.print("Humidity : ");
  //Serial.println(Humidity);
  //Serial.print("Humidity Limit : ");
  //Serial.println(hum);

  if ( (sgp.eCO2 > carbon_dioxide ||  Temperature > temp) && fan_auto !=true ) {

    
    Serial.println("Co2 limit or temp reached, fan on");
    fan_auto=true;
    digitalWrite(FANPin, HIGH);
    theaterChase(strip.Color(0,   0,   30), 50);
    strip.clear();
    strip.show();
    light_auto = false;
    
    }
      
  if ( (sgp.eCO2 < carbon_dioxide &&  Temperature < temp) && fan_auto == true ) {

        Serial.println("Co2 / temp limit good, fan off");
        fan_auto = false;
        digitalWrite(FANPin, LOW);
        //theaterChase(strip.Color(127,   0,   0), 50);
        strip.clear();
        strip.show();
        light_auto = false;
        

    }
  

  if ( Humidity < hum && hum_auto !=true ) {

     
    Serial.println("hum low limit reached, hum on");
    hum_auto = true;
    digitalWrite(HUMIDITYPin, HIGH);
    theaterChase(strip.Color(0,   30,   0), 50);
    strip.clear();
    strip.show();
    light_auto = false;
    

  }

  if ( Humidity >= hum && hum_auto == true ) {

     
    Serial.println("hum low limit reached, hum off");
    hum_auto = false;
    digitalWrite(HUMIDITYPin, LOW);
    //theaterChase(strip.Color(127,   0,   0), 50);
    strip.clear();
    strip.show();
    light_auto = false;
    

  }




}

void lightClockStart(String clockData){

  String string_hours, string_minutes, string_seconds;
  
  int change_hours =  clockData.indexOf(':') ;
  int change_minutes = clockData.indexOf(':', change_hours+1) ;
  int change_seconds = clockData.indexOf(':', change_minutes+1) ;

  string_hours = clockData.substring(0, 2);
  string_minutes = clockData.substring(3, 5);
  string_seconds = clockData.substring(6, 8);

  light_on_hour = string_hours.toInt();
  light_on_minute = string_minutes.toInt();

}


void lightClockStop(String clockData){

  String string_hours, string_minutes, string_seconds;
  
  int change_hours =  clockData.indexOf(':') ;
  int change_minutes = clockData.indexOf(':', change_hours+1) ;
  int change_seconds = clockData.indexOf(':', change_minutes+1) ;

  string_hours = clockData.substring(0, 2);
  string_minutes = clockData.substring(3, 5);
  string_seconds = clockData.substring(6, 8);

  light_off_hour = string_hours.toInt();
  light_off_minute = string_minutes.toInt();
  
}

void lightStatus(){

  if (hours == light_on_hour) {

    if (minutes == light_on_minute) {
       
      if (light_auto != true){

        //fire off light on command
        Serial.println("Lights on");
        light_auto = true;
        allWhite();
      }
       
       }    

  }
  
  if (hours == light_off_hour) {

    if (minutes == light_off_minute) {
       
      if (light_auto != false){

        //fire off light on command
        Serial.println("Lights off");
        light_auto = false;
        strip.clear();
        strip.show();
        strip.show();
      }
       
       }    

  }


}






uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
};


int menuState(uint8_t button, uint8_t button2, uint8_t button3){

char relay_temp_s[3];
char buff[4];
char *buff1;
char asciiChar[0];

// menu state 0 Display data
// state_menu main_menu
// setup menu 
// 1.env
// 2.wifi
// 3.exit

bool exitBool = (stateMenu=="env_menu" ||stateMenu == "env_alert_1" | stateMenu == "env_alert_temp" ||
 stateMenu == "env_alert_2" ||  stateMenu == "env_alert_co2" || stateMenu == "env_alert_hum" || stateMenu == "wifi"
 || stateMenu == "main_menu" || stateMenu == "relay" || stateMenu == "relay_toggle" || stateMenu == "relay_temp" ||
 stateMenu == "relay_limit" || stateMenu == "relay_co2" || stateMenu == "relay_hum" || stateMenu == "relay_toggle_fan" ||
 stateMenu == "relay_toggle_hum") || stateMenu == "system";



if (  exitBool && button3 == 0 ){
      //exit
      
      stateMenu = "sensor_data";
      displayed = 0;
      loopTime = 500;
      return 0;
    }


  
// go from sensor data to main_menu
if (stateMenu == "sensor_data" && button2 == LOW ){
         Serial.println("button2 pressed main_menu set");

      stateMenu = "main_menu";
      displayed = 0;
      loopTime = 200;
      return 0;
    
    }

      
//go from main_menu to env_main_menu
if (stateMenu == "main_menu" && button == LOW){
  //env menu base
      menu_state = 2;
      stateMenu = "env_menu";
      displayed = 0;
      loopTime = 200;
      
      menuDisplay[0] = { "env-menu" };
      menuDisplay[1] = { "alert" };
      menuDisplay[2] = { "relay" };
      
      return 0;

    }

    

// menu state env_main_menu
// env menu
// 1.alert
// 2.relay
// 3.exit

    if (stateMenu == "main_menu" && button2 == LOW){
      stateMenu = "system";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "system-menu" };
      menuDisplay[1] = { "wifi" };
      menuDisplay[2] = { "factory-reset" };
      return 0;

    }

    if (stateMenu == "system" && button == LOW){
      stateMenu = "wifi";
      displayed = 0;
      loopTime = 200;
      //menuDisplay[0] = { "system-menu" };
      //menuDisplay[1] = { "wifi" };
      //menuDisplay[2] = { "factory-reset" };
      return 0;

    }
    if (stateMenu == "system" && button2 == LOW){
      stateMenu = "factory_reset";
      displayed = 0;
      loopTime = 200;
      //menuDisplay[0] = { "system-menu" };
      //menuDisplay[1] = { "wifi" };
      //menuDisplay[2] = { "factory-reset" };
      return 0;

    }
    if (stateMenu == "factory_reset" && button == LOW){
      stateMenu = "main_menu";
      displayed = 0;
      loopTime = 200;
      Serial.println("back to life, back to the factory.");
      FactoryResetStoredData();
      //menuDisplay[0] = { "system-menu" };
      //menuDisplay[1] = { "wifi" };
      //menuDisplay[2] = { "factory-reset" };
      return 0;

    }
     if (stateMenu == "factory_reset" && button2 == LOW){
      stateMenu = "main_menu";
      Serial.println("oh no, please don't hurt my factory");
      displayed = 0;
      loopTime = 200;
      //menuDisplay[0] = { "system-menu" };
      //menuDisplay[1] = { "wifi" };
      //menuDisplay[2] = { "factory-reset" };
      return 0;

    }
   

    
    if (stateMenu == "wifi" && button == LOW){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      
        //strcat(wifi_sid_add,charSet[charCount]);
        snprintf(asciiChar,2,"%c",33);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      //strcpy(wifi_sid_add,asciiChar);
      strcat(wifi_sid_add,asciiChar);
      
      return 0;
    }

    

    if (stateMenu == "wifi_sid" && button == LOW){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;

      charCount = charCount + 1;
      if (charCount > 126){
        charCount = 33;
      } 
      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcpy(wifi_sid_add,asciiChar);
      
      //strcpy(wifi_sid_add,charSet[charCount]);
      
      return 0;
    }

    if (stateMenu == "wifi_sid" && button2 == LOW){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      //charCount = charCount - 1;

      //if (charCount < 33) {
      //  charCount = 126;
      //}

      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcpy(wifi_sid_add,asciiChar);

      return 0;
    }

    if (stateMenu == "wifi_sid" && button3 == LOW){
      stateMenu = "wifi_sid";
      displayed = 0;
      loopTime = 200;
      
      //strcat(wifi_sid,charSet[charCount]);
      snprintf(asciiChar,2,"%c",charCount);
        
      //strcpy(wifi_sid_add,charSet[charCount]);
      strcat(wifi_sid,asciiChar);

      

      charCount = 65;
      totalChars = totalChars + 1;
      return 0;
    
    }

    
    


    if (stateMenu == "wifi" && button == LOW){
      stateMenu = "wifi_pw";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }


//menu state relay
// relay menu
// 1. limit
// 2. toggle
    

//menu relay_1
// 1. co2/hum
// 2. temp
// 3. exit

    if (stateMenu == "relay" && button == LOW){
      stateMenu = "relay_limit";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-limit" };
      menuDisplay[1] = { "temp" };
      menuDisplay[2] = { "co2/hum" };

      return 0;
    }
    if (stateMenu == "relay" && button2 == LOW){
      stateMenu = "relay_toggle";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-toggle" };
      menuDisplay[1] = { "fan" };
      menuDisplay[2] = { "hum" };

      return 0;
    }
    if (stateMenu == "relay_toggle" && button == LOW){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle" && button2 == LOW){
      stateMenu = "relay_toggle_hum";
      displayed = 0;
      loopTime = 200;
      return 0;
    }

    if (stateMenu == "relay_toggle_fan" && button == LOW){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      relay_fan_toggle = 1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle_fan" && button2 == LOW){
      stateMenu = "relay_toggle_fan";
      displayed = 0;
      relay_fan_toggle = 0;
      loopTime = 200;
      return 0;
    }

     if (stateMenu == "relay_toggle_hum" && button == LOW){
      stateMenu = "relay_toggle_hum";
      displayed = 0;
      relay_hum_toggle = 1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "relay_toggle_hum" && button2 == LOW){
      stateMenu = "relay_toggle_hum";
      displayed = 0;
      relay_hum_toggle = 0;
      loopTime = 200;
      return 0;
    }




    if (stateMenu == "relay_limit" && button == LOW){
      stateMenu = "relay_temp";
      displayed = 0;
      loopTime = 200;
      

      return 0;
    }
    if (stateMenu == "relay_limit" && button2 == LOW){
      stateMenu = "relay_limit_1";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-limit" };
      menuDisplay[1] = { "co2" };
      menuDisplay[2] = { "hum" };
      return 0;
    }

    if (stateMenu == "relay_limit_1" && button == LOW){
      stateMenu = "relay_co2";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }
    if (stateMenu == "relay_co2" && button == LOW){
      //env menu temp
      stateMenu = "relay_co2";
      displayed = 0;
      relay_co2 = relay_co2 +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_co2" && button2 == LOW){
      stateMenu = "relay_co2";
      displayed = 0;
      relay_co2 = relay_co2 -1;
      loopTime = 200;
      return 0;
    }
    


    if (stateMenu == "relay_limit_1" && button == LOW){
      stateMenu = "relay_hum";
      displayed = 0;
      loopTime = 200;
      
      return 0;
    }
    if (stateMenu == "relay_hum" && button == LOW){
      //env menu temp
      stateMenu = "relay_hum";
      displayed = 0;
      relay_hum = relay_hum +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_hum" && button2 == LOW){
      stateMenu = "relay_hum";
      displayed = 0;
      relay_hum = relay_hum -1;
      loopTime = 200;
      return 0;
    }



    if (stateMenu == "relay_temp" && button == LOW){
      //env menu temp
      stateMenu = "relay_temp";
      displayed = 0;
      relay_temp = relay_temp +1;
      
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "relay_temp" && button2 == LOW){
      stateMenu = "relay_temp";
      displayed = 0;
      relay_temp = relay_temp -1;
      loopTime = 200;
      return 0;
    }

//menu relay_2


if (stateMenu == "env_menu" && button == LOW){
      stateMenu = "env_alert";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
// menu state env_menu_alert_1
// 1. temp
// 2. co2 / hum
// 3 .exit
   if (stateMenu == "env_menu" && button2 == LOW){
      stateMenu = "relay";
      displayed = 0;
      loopTime = 200;
      menuDisplay[0] = { "relay-menu" };
      menuDisplay[1] = { "limit" };
      menuDisplay[2] = { "toggle" };
      return 0;
    }
  //if (stateMenu == "env_alert" && button == LOW){
  //    //env menu temp
  //    stateMenu = "env_alert_1";
  //    displayed = 0;
  //    loopTime = 200;
  //    return 0;
  //  }
  if (stateMenu == "env_alert" && button == LOW){
      //env menu temp
      stateMenu = "env_alert_temp";
      displayed = 0;
      loopTime = 200;
      return 0;
    }
// menu state env_menu_alert_temp
//env alert temp
//set temp to alert : alert_temp
// 3. save and exit

if (stateMenu == "env_alert_temp" && button == LOW){
      //env menu temp
      stateMenu = "env_alert_temp";
      displayed = 0;
      alert_temp = alert_temp +1;
      loopTime = 200;
      return 0;
    }
  if (stateMenu == "env_alert_temp" && button2 == LOW){
      stateMenu = "env_alert_temp";
      displayed = 0;
      alert_temp = alert_temp -1;
      loopTime = 200;
      return 0;
    }

  if (stateMenu == "env_alert" && button2 == LOW){
      //env menu temp
      stateMenu = "env_alert_2";
      displayed = 0;
      loopTime = 200;
      return 0;
    }

//menu state env_menu_sub_2
// 1. co2
// 2. hum
// 3 .exit

  if (stateMenu == "env_alert_2" && button == LOW){
      //env menu temp
      stateMenu = "env_alert_co2";
      displayed = 0;
      loopTime = 10;
      return 0;
    }

//menu state env_menu_alert_co2
//env alert co2
//set co2 to alert : alert_co2
// 3. save and exit

    if (stateMenu == "env_alert_co2" && button == LOW){
      //env menu temp
      stateMenu = "env_alert_co2";
      displayed = 0;
      alert_co2 = alert_co2 +1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "env_alert_co2" && button2 == LOW){
      stateMenu = "env_alert_co2";
      displayed = 0;
      alert_co2 = alert_co2 -1;
      loopTime = 200;
      return 0;
    }
    


    if (stateMenu == "env_alert_2" && button2 == LOW){
      //env menu temp
      stateMenu = "env_alert_hum";
      displayed = 0;
      loopTime = 200;
      return 0;
    }


    if (stateMenu == "env_alert_hum" && button == LOW){
      //env menu temp
      stateMenu = "env_alert_hum";
      displayed = 0;
      alert_hum = alert_hum +1;
      loopTime = 200;
      return 0;
    }
    if (stateMenu == "env_alert_hum" && button2 == LOW){

      stateMenu = "env_alert_hum";
      displayed = 0;
      alert_hum = alert_hum -1;
      loopTime = 200;
      return 0;
    }

    return 0;
}

void drawMenu(){
       //char *menuDisplay[] = {"one", "two", "three"};
       u8g2.begin();
       u8g2.enableUTF8Print();
       u8g2.setFont(u8g2_font_helvB08_tf);
       u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
    
       u8g2.setCursor(12,10);
       u8g2.print(menuDisplay[0]);
    
    
      u8g2.setCursor(12,30);
      u8g2.print("1. ");
      u8g2.print(menuDisplay[1]);
    
    
      u8g2.setCursor(12,40);
      u8g2.print("2. ");
      u8g2.print(menuDisplay[2]);
    
      u8g2.setCursor(12,50);
      u8g2.print("3. exit ");
    
    u8g2.sendBuffer();
  
}




void mainMenu(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("setup menu");
  u8g2.setCursor(12,30);
  u8g2.print("1. env");
  u8g2.setCursor(12,40);
  u8g2.print("2. system ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();
}

void wifi(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("url http://");
  u8g2.print(WiFi.softAPIP());
  u8g2.setCursor(12,30);
  u8g2.print("ssid: ");
  u8g2.print(ssid);
  u8g2.setCursor(12,40);
  u8g2.print("pw: ");
  u8g2.print(pass);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  u8g2.sendBuffer();

}

void factoryReset(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("Factory Reset?");
  
  u8g2.setCursor(12,30);
  u8g2.print("1. yes!");
  
  u8g2.setCursor(12,40);
  u8g2.print("2. oh no! ");
  
  u8g2.setCursor(12,50);
 
  u8g2.sendBuffer();

}




void relay(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("Relay set");
  u8g2.setCursor(12,30);
  u8g2.print("1. value");
  u8g2.setCursor(12,40);
  u8g2.print("2. toggle ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}

void envAlert1(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. temp");
  u8g2.setCursor(12,40);
  u8g2.print("2. co2 ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}


void envAlert(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. temp");
  u8g2.setCursor(12,40);
  u8g2.print("2. co2 ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}
void envAlert2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env menu alert");
  u8g2.setCursor(12,30);
  u8g2.print("1. co2");
  u8g2.setCursor(12,40);
  u8g2.print("2. hum ");
  u8g2.setCursor(12,50);
  u8g2.print("3. exit ");
  
  u8g2.sendBuffer();

}
void envAlertTemp(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set temp to alert : ");
  
  u8g2.print(alert_temp);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}
void envAlertCo2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert co2");
  u8g2.setCursor(12,30);
  u8g2.print("Set co2 to alert : ");
  
  u8g2.print(alert_co2);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void envAlertHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("env alert hum");
  u8g2.setCursor(12,30);
  u8g2.print("Set Hum to alert : ");
  
  u8g2.print(alert_hum);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayTemp(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Temp : ");
  
  u8g2.print(relay_temp);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayCo2(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Co2 : ");
  
  u8g2.print(relay_co2);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-temp");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Hum : ");
  
  u8g2.print(relay_hum);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayToggleHum(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-hum toggle");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay Hum : ");
  
  u8g2.print(relay_hum_toggle);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}

void relayToggleFan(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("relay-fan toggle");
  u8g2.setCursor(12,30);
  u8g2.print("Set relay fan : ");
  
  u8g2.print(relay_fan_toggle);
  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}
void WifiSid(){
  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  u8g2.print("Wifi sid");
  u8g2.setCursor(12,30);

  
  u8g2.print(wifi_sid);
  u8g2.print(wifi_sid_add);

  u8g2.setCursor(12,50);
  u8g2.print("3. save and exit");
  
  u8g2.sendBuffer();

}







void displayMenu(){
  if (!displayed){
  
  if (stateMenu=="main_menu"){
    mainMenu();
  }
  if (stateMenu=="system"){
    drawMenu();
  }
  if (stateMenu=="factory_reset"){
    factoryReset();
  }
  

  if (stateMenu=="env_menu"){
    drawMenu();
  }
  if (stateMenu=="relay"){
    drawMenu();
  }
  if (stateMenu=="relay_limit"){
    drawMenu();
  }
  if (stateMenu=="relay_limit_1"){
    drawMenu();
  }
  if (stateMenu=="relay_toggle"){
    drawMenu();
  }
  if (stateMenu=="wifi_sid"){
    WifiSid();
  }
  if (stateMenu=="relay_toggle_hum"){
    relayToggleHum();
  }
  if (stateMenu=="relay_toggle_fan"){
    relayToggleFan();
  }

  if (stateMenu=="relay_temp"){
    relayTemp();
  }
  if (stateMenu=="relay_co2"){
    relayCo2();
  }
  if (stateMenu=="relay_hum"){
    relayHum();
  }
  




  if (stateMenu=="wifi"){
    wifi();
  }


  if (stateMenu=="env_alert"){
    envAlert();
  
  }
  if (stateMenu=="env_alert_1"){
    envAlert1();
  }
  if (stateMenu=="env_alert_2"){
    envAlert2();
  }
  if (stateMenu=="env_alert_temp"){
    envAlertTemp();
  }
  if (stateMenu=="env_alert_co2"){
    envAlertCo2();
  }
  if (stateMenu=="env_alert_hum"){
    envAlertHum();
  }


  
  }
  displayed = 1;
}



void displayEnvData(){
  //u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  //u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  if (WiFi.localIP()){
    u8g2.setCursor(12,10);
    u8g2.print("http://");
    u8g2.print(WiFi.localIP());

  }else {

      if (WiFi.softAPIP()){

        u8g2.setCursor(12,10);
        u8g2.print("http://");
        u8g2.print(WiFi.softAPIP());

          }
       }
  
  u8g2.setCursor(12,20);
  //u8g2.setCursor(12,10);
  u8g2.print("CO2 :");
  //u8g2.setCursor(45,55);
  u8g2.print(sgp.eCO2);
  u8g2.setCursor(12,30);
  u8g2.print("TVOC :");
  //u8g2.setCursor(45,55);
  u8g2.print(sgp.rawEthanol);
  u8g2.setCursor(12,40);
  u8g2.print("TEMP :");
  u8g2.print(Temperature);
  u8g2.setCursor(12,50);
  u8g2.print("Humid :");
  u8g2.print(Humidity);
  String new_time = clockOutput();
  u8g2.setCursor(12,60);
  u8g2.print(new_time);
  //u8g2.sendBuffer();
  //updateOnChange();
  
  if (memcmp(history,u8g2.getBufferPtr(),1024)){
    memcpy(history, u8g2.getBufferPtr(), 1024);
    
    //Serial.println("changed!");
    u8g2.sendBuffer();
    
    }    

}

void booting(){

  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  //u8g2.setCursor(12,10);
  u8g2.print("finding wifi");
  u8g2.setCursor(12,20);
  u8g2.print("wifi search...");
  u8g2.sendBuffer();
  delay(1000);
  
  //updateOnChange();

  
};


void webUpdate(){

  u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  //u8g2.setCursor(12,10);
  u8g2.print("update via");
  u8g2.setCursor(12,20);
  u8g2.print("web interface...");
  u8g2.sendBuffer();
  delay(1000);
  
  //updateOnChange();

  
};


void updateOnChange(){
  
  //Serial.println("checking for update);
  if (memcmp(history,u8g2.getBufferPtr(),1024)){
    memcpy(history, u8g2.getBufferPtr(), 1024);
    Serial.println("changed!");
    u8g2.sendBuffer();
    
    }
  
  };











void setup() {
  
  
  
  Serial.begin(115200);
  while (!Serial); 
  pcf.setup(0x20);        // The PCF8574 is configured to 0x20 I2C address. Check A0, A1 and A2 pins of the device.
  pcf.write(0B11111111);  // Turns all pins/ports HIGH
  Wire.begin();  
  sgp.begin();
  pinMode(DHTPin, INPUT);
  dht.begin();
  pinMode(LEDPin, OUTPUT);
  strip.begin();
  //strip.clear();
  strip.show();
  pinMode(FANPin, OUTPUT);
  pinMode(HUMIDITYPin, OUTPUT);
  //strip.setBrightness(255);
  //allWhite();4
  //delay(1000);
  //strip.clear();
  //strip.show();


  
  u8g2.begin();
  u8g2.clearBuffer();                   //clear the internal memory
  u8g2.drawBitmap(0,0,16,64,unit42);  //bootScreen is the above BitMap
  u8g2.sendBuffer();
  delay(4000);                          //View Nan photo for 4 seconds

  
  WiFi.mode(WIFI_AP_STA);
 

  
  u8g2.enableUTF8Print();
  //SPIFFS.begin();
  if ( !SPIFFS.begin() ) {
    Serial.println("Error file system has occured while mounting");
    }else{
  readStoredData();
    }

    lightClockStart(light_data[0]);
    lightClockStop(light_data[1]);
  
  // set light status on light_data[3] to off on reboot

  light_data.push_back("on");
  
  light_data[3] = "on";

  String active = "on";
  
  Serial.println("Wifi data:");

  

  
  Serial.println(wifi_data[0]);
  
  Serial.println(wifi_data[0].length());
  Serial.println(active.length());
  Serial.println(wifi_data[1]);
  Serial.println(wifi_data[2]);


  //override for wifi not connecting test

 
  
  if (wifi_data[0] == "on" ) {
    Serial.println("Wifi enabled");
    u8g2.begin();
  u8g2.enableUTF8Print();   
  //8g2.setFont(u8g2_font_helvB18_tf);
  u8g2.setFont(u8g2_font_helvB08_tf);
  u8g2.setFontDirection(0);
  //u8g2.clearBuffer();
  u8g2.setCursor(12,10);
  //u8g2.setCursor(12,10);
  u8g2.print("finding wifi");
  u8g2.setCursor(12,20);
  u8g2.print("wifi search...");
  u8g2.sendBuffer();
  delay(1000);
    enableWifi();

  }

 

  if (ap_data[0] == "on" ) {
    Serial.println("ap enabled");
    WiFi.softAP(ap_data[1], ap_data[2]);
        
  }
  
  
  //WiFi.softAP(ap_data[1], ap_data[2]);

  Serial.println("clock time:");
  String get_time = clockOutput();
  Serial.println(get_time);


  server.begin();
    
  //pinMode(Push_button_1,OUTPUT);
  pinMode(Push_button_2,INPUT);
  //pinMode(Push_button_3,INPUT);
  //pinMode(Relay1,OUTPUT);

  //WiFi.mode(WIFI_AP);
  
  server.on("/", handleRoot);
  server.on("/submit_page", handleForm);
  server.on("/test", fileindex);
  server.onNotFound(handleNotFound);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("/jquery1.js", jquery);
  server.on("/jquery2.min.js", jqueryFull);
  server.on("/jquery3.min.js", jquerySlim);
  server.on("/relay_control.js", jsRelayControl);
  server.on("/window_load.js", jsWindowLoad);
  server.on("bootstrap.min.css", bootstrap);
  server.on("/popper.min.js", popper);
  server.on("/bootstrap.min.js", bootstrapmin);
  server.on("bootstrap.min.js", bootstrapmin);
  server.on("/slide.css", slide);
  server.on("/update_sensor",updateSensor);

  //server.on("/relay_on", handle_RelayOn);
  //server.on("/relay_off", handle_RelayOff);
  server.on("/fan_relay_on", handle_FanOn);
  server.on("/fan_relay_off", handle_FanOff);
  server.on("/humidity_relay_on", handle_HumidityOn);
  server.on("/humidity_relay_off", handle_HumidityOff);
  server.on("/light_relay_on", handle_LightOn);
  server.on("/light_relay_off", handle_LightOff);
  server.on("/manual_override_on", manualOverrideOn);
  server.on("/manual_override_off", manualOverrideOff);



  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    //while (1);
  }
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // If you have a baseline measurement from before you can assign it to start, to 'self-calibrate'
  //sgp.setIAQBaseline(0x8E68, 0x8F41);  // Will vary for each sensor!
}

int counter = 0;
int button_counter = 0;

void loop() {

  
  timeTrack();
  server.handleClient();

  lightStatus();
  //allWhite();
  limitStatus();

  data1 = pcf.digitalRead(4);
  data2 = pcf.digitalRead(5);
  data3 = pcf.digitalRead(6);

  //button 1 state
  if (data1 == LOW && !data_state1) {

      data_state1 = 1;  
      Serial.println("button 1 pressed");
      if (stateMenu != "sensor_data") {
          menuState(data1,data2,data3);
         
      }
      
       return;
      
       }

  if (data1 == HIGH && data_state1)
    {
      data_state1 = 0;  
      Serial.println("button 1 released");
      return;
    }
  
  //button 2 state
      if (data2 == LOW && !data_state2)
    {
      data_state2 = 1;  
      Serial.println("button 2 pressed");

      menuState(data1,data2,data3);
      return;
    }

    
    if (data2 == HIGH && data_state2)
    {

      data_state2 = 0;  
      Serial.println("button 2 released");
      return;
      
    }
    
    //button 3 state
    if (data3 == LOW && !data_state3)
    {
      data_state3 = 1;  
      Serial.println("button 3 pressed");

      if (stateMenu != "sensor_data") {
          menuState(data1,data2,data3);
          
      }
      return;
    }


    if (data3 == HIGH && data_state3)
    {
      data_state3 = 0;  
      Serial.println("button 3 released");
      return;
      
    }


  
  if (stateMenu != "sensor_data") {
   
   //delay(200);
   
   displayMenu();
   return;
   

  }
  if (stateMenu == "sensor_data") {
   
   displayEnvData();

  }
   

  
  counter++;

  if (counter == 300) {


    counter = 0;
    
    readTemp();
    sgp.setHumidity(getAbsoluteHumidity(Temperature, Humidity));

    uint16_t TVOC_base, eCO2_base;
    if (! sgp.getIAQBaseline(&eCO2_base, &TVOC_base)) {
      Serial.println("Failed to get baseline readings");
      return;
    }

    Serial.print("****Baseline values: eCO2: 0x"); Serial.print(eCO2_base, HEX);
    Serial.print(" & TVOC: 0x"); Serial.println(TVOC_base, HEX);
    
    if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    //return;
    //delay(5000);
  }

  Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  

  if (! sgp.IAQmeasureRaw()) {
    Serial.println("Raw Measurement failed");
    //return;
  }

  Serial.print("Raw H2 "); Serial.print(sgp.rawH2); Serial.print(" \t");
  Serial.print("Raw Ethanol "); Serial.print(sgp.rawEthanol); Serial.println("");

  }
  
}

void updateSensor() 
{

server.send(200, "text/html", updateSensorAjax(Temperature, Humidity, sgp.eCO2));

}



void handleRoot() {
  String form_relay_toggle_fan="";
  String new_time = clockOutput();
  //String tz = getTZ();
  Serial.println(new_time);
  server.send(200, "text/html", sendHTML(form_relay_toggle_fan, wifi_data, Temperature, Humidity, sgp.eCO2, ap_data, new_time,tz) );   // Send HTTP status 200 (Ok) and send some text to the browser/client
  //server.send(200, "text/html", sendHTML1());
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void bootstrap(){
   File file = SPIFFS.open("/bootstrap.min.css", "r"); 
   size_t sent = server.streamFile(file, "text/css");
   file.close();
}
void jquery(){
   File file = SPIFFS.open("/jquery.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
   file.close();
}
void jquerySlim(){

   File file = SPIFFS.open("/jquery-slim.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
   file.close();
}
void jqueryFull(){

  File file = SPIFFS.open("/jquery-full.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
   file.close();
}
void slide(){
   File file = SPIFFS.open("/slide.css", "r"); 
   size_t sent = server.streamFile(file, "text/css");
   file.close();
}
void jsRelayControl(){
   File file = SPIFFS.open("/relay_control.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
   file.close();
}
void jsWindowLoad(){
   File file = SPIFFS.open("/window_load.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
   file.close();
}


void popper(){
   File file = SPIFFS.open("/popper.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
}

void bootstrapmin(){
   File file = SPIFFS.open("/bootstrap.min.js", "r"); 
   size_t sent = server.streamFile(file, "application/javascript");
}
void fileindex(){
   File file = SPIFFS.open("/index.html", "r"); 
   size_t sent = server.streamFile(file, "text/html");
}

void manualOverrideOn(){
  manual_override = true;
  Serial.println("manual on");
  server.send(200, "text/html", "manual_on");


}
void manualOverrideOff(){
  manual_override = false;
  Serial.println("manual off");
  server.send(200, "text/html", "manual_off");

}

void handle_FanOn(){
Serial.println("fan on");
 digitalWrite(FANPin, HIGH);
 fan_manual = true;
 server.send(200, "text/html", "fan_on");
};

void handle_FanOff(){
  Serial.println("fan off");
 digitalWrite(FANPin, LOW);
 fan_manual = false;
 server.send(200, "text/html", "fan_off");
};

void handle_LightOn(){
Serial.println("light switch on");
 //digitalWrite(Relay1, HIGH);
 light_data[3] = "on";
 light_manual = true;

  //strip.begin();
  //strip.clear();
  //strip.show();
  //strip.setBrightness(255);
  allWhite();
  delay(1000);
 //allWhite();
 //delay(1000);
 server.send(200, "text/html", "light_on");
};

void handle_LightOff(){
  Serial.println("light off");
 //digitalWrite(Relay1, LOW);
 light_data[3] = "off";
 light_manual = false;
 strip.clear();
 strip.show();
 strip.show();
 strip.show();
 server.send(200, "text/html", "light_off");
};


void handle_HumidityOn(){
Serial.println("humidity on");
 digitalWrite(HUMIDITYPin, HIGH);
 hum_manual = true;
 server.send(200, "text/html", "humidity_on");
};

void handle_HumidityOff(){
  Serial.println("humidity off");
 digitalWrite(HUMIDITYPin, LOW);
 hum_manual = false;
 server.send(200, "text/html", "humidity_off");
};


void handleForm() {

  bool resetFlag=false;

  Serial.println("handleForm"); 

   String form_ap_pass = server.arg("ap_pass");
   String form_ap_ssid = server.arg("ap_ssid");
   String form_ap_active = server.arg("ap_active");
   String form_cl_pass = server.arg("cl_pass");
   String form_cl_ssid = server.arg("cl_ssid");
   String form_cl_active = server.arg("cl_active");
   String form_relay_limit_temp = server.arg("relay_limit_temp");
   String form_relay_limit_hum = server.arg("relay_limit_hum");
   String form_relay_limit_co2 = server.arg("relay_limit_co2");
   String form_relay_toggle_fan = server.arg("relay_toggle_fan");
   String form_relay_toggle_hum = server.arg("relay_limit_hum");
   String form_clock_set = server.arg("clock");
   String form_clock_tz = server.arg("tz");
   String form_clock_set_bool = server.arg("clockset");
   String form_light_on = server.arg("light_on");
   String form_light_off = server.arg("light_off");
   String form_light_strobe = server.arg("light_strobe");

  Serial.println(form_ap_active);
  Serial.println(form_ap_pass);
  Serial.println(form_ap_ssid);
  Serial.println(form_cl_pass);
  Serial.println(form_cl_ssid);
  Serial.println(form_relay_limit_temp);
  Serial.println(form_relay_limit_co2);
  Serial.println(form_relay_toggle_hum);
  Serial.println(form_relay_toggle_fan);
  Serial.println(form_clock_set);

  

  tz = server.arg("tz");
  time_data[0]=tz;

  if (form_clock_set_bool == "1"){
    Serial.println("clock set bool is on, setting clock...");
    String new_time = clockSet(form_clock_set);
    writeStoredTimeData();
  }
  
  //writeStoredTimeData();

  if(form_ap_active != "on" && form_cl_active != "on")
   
   {

     String alertText = "You are about to remove all wireless access, both ap and client. ";
     alertText += "You would have to use the manual controls to turn these on again, continue? ";

    server.send(200, "text/html", choiceHTML(alertText,"disableWifi","") );

    return;
    
    }

   

   // if (  ((form_ap_ssid !=ap_data[1] || form_ap_pass !=ap_data[2] ) && ap_data[0] == "on") || form_ap_active !=ap_data[0])

   if ( ( (form_ap_ssid !=ap_data[1] || form_ap_pass !=ap_data[2] ) && ap_data[0] == "on") || form_ap_active !=ap_data[0])
   {
    
    ap_data[0] = form_ap_active;
    ap_data[1] = form_ap_ssid;
    ap_data[2] = form_ap_pass;
    writeStoredApData();

    Serial.println("write stored AP data, reseting");

    resetFlag=true;

   }else if( (form_ap_ssid !=ap_data[1] || form_ap_pass !=ap_data[2] ) ) {

      ap_data[0] = form_ap_active;
      ap_data[1] = form_ap_ssid;
      ap_data[2] = form_ap_pass;
      writeStoredApData();

    Serial.println("write stored AP data, no reset");
   }

    if ( ( (form_cl_ssid !=wifi_data[1] || form_cl_pass !=wifi_data[2] ) && wifi_data[0] == "on") || form_cl_active !=wifi_data[0])
   
   {

    wifi_data[0] = form_cl_active;
    wifi_data[1] = form_cl_ssid;
    wifi_data[2] = form_cl_pass; 
    writeStoredWifiData();

    Serial.println("write stored Wifi data, reseting");

     resetFlag=true;

   }else if( (form_cl_ssid !=wifi_data[1] || form_cl_pass !=wifi_data[2] ) ) {

    if (form_cl_active !=wifi_data[0] || form_cl_ssid != wifi_data[1] || form_cl_pass !=wifi_data[2])
    {


    wifi_data[0] = form_cl_active;
    wifi_data[1] = form_cl_ssid;
    wifi_data[2] = form_cl_pass;
    writeStoredWifiData();}
    Serial.println("write stored Wifi data, no reset");

   };


   //go ahead and write the limit data to the store

   limit_data[0] = form_relay_limit_temp ;
   limit_data[1] = form_relay_limit_co2 ;
   limit_data[2] = form_relay_limit_hum ;

   writeStoredLimitData();

   light_data[0] = form_light_on ;
   light_data[1] = form_light_off ;
   light_data[2] = form_light_strobe ;


   writeStoredLightData();

   lightClockStart(light_data[0]);
   lightClockStop(light_data[1]);


   if (resetFlag){

     //check for wifi station connection
     //check for connection to wifi client
     
     String alertText = "Don't Panic! You have changed the wireless connectivity and may need to reconnect.";
     alertText += " The new information for connecting to Unit 42 is displayed on the front of the device.";
     
     server.send(200, "text/html", warnHTML(alertText) );

     delay(1000);

     ESP.reset();
   }
  

  
    
  //server.send(200, "text/html", sendHTML(form_relay_toggle_fan, wifi_data, Temperature, Humidity, sgp.eCO2, ap_data, new_time, tz) );
  
  server.send(200, "text/html", "<meta http-equiv=\"refresh\" content=\"0; url='./' \" />" );
  webUpdate();

  
}

void writeStoredLightData(){


  File file = SPIFFS.open("/light.txt", "w");
  if (!file) {
    Serial.println("limit.txt failed to open file for writing");
    return;
   }
   
  file.println(light_data[0]);
  file.println(light_data[1]);
  file.println(light_data[2]);
     

  
  

  Serial.println("light data stored");
  Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  light file verify  *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");


  
  while (file.available()) {
    wifi_data.push_back(file.readStringUntil('\n'));
    Serial.println("validating light file....");
    }
   file.close();

   

   for (String s : light_data ) {
    
    Serial.println(s);
    
    }
    light_data[0].replace("\r","");
    light_data[1].replace("\r","");
    light_data[2].replace("\r","");
    
   
   

}

void writeStoredLimitData(){


  File file = SPIFFS.open("/limit.txt", "w");
  if (!file) {
    Serial.println("limit.txt failed to open file for writing");
    return;
   }
   
  file.println(limit_data[0]);
  file.println(limit_data[1]);
  file.println(limit_data[2]);
     

  
  

  Serial.println("limit data stored");
  Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  limit file verify  *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");


  
  while (file.available()) {
    limit_data.push_back(file.readStringUntil('\n'));
    Serial.println("validating limit file....");
    }
   file.close();

   

   for (String s : limit_data ) {
    
    Serial.println(s);
    
    }
    limit_data[0].replace(" ","");
    limit_data[1].replace(" ","");
    limit_data[2].replace(" ","");
    limit_data[0].replace("\r","");
    limit_data[1].replace("\r","");
    limit_data[2].replace("\r","");
   
   

}




void writeStoredWifiData(){


  File file = SPIFFS.open("/wifi.txt", "w");
  if (!file) {
    Serial.println("wifi.txt failed to open file for writing");
    return;
   }
   
  file.println(wifi_data[0]);
  file.println(wifi_data[1]);
  file.println(wifi_data[2]);
     

  
  

  Serial.println("Wifi data stored");
  Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  wifi file verify  *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");


  
  while (file.available()) {
    wifi_data.push_back(file.readStringUntil('\n'));
    Serial.println("validating wifi file....");
    }
   file.close();

   

   for (String s : wifi_data ) {
    
    Serial.println(s);
    
    }
    wifi_data[0].replace("\r","");
    wifi_data[1].replace("\r","");
    wifi_data[2].replace("\r","");
    //enableWifi();
   

}

void writeStoredApData(){


  File file = SPIFFS.open("/ap.txt", "w");
  if (!file) {
    Serial.println("ap.txt failed to open file for writing");
    return;
   }
   
  file.println(ap_data[0]);
  file.println(ap_data[1]);
  file.println(ap_data[2]);
     

  
  

  Serial.println("ap data stored");
  Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  ap file verify  *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");


  
  while (file.available()) {
    ap_data.push_back(file.readStringUntil('\n'));
    Serial.println("validating ap file....");
    }

   file.close();

   

   for (String s : ap_data ) {
    
    Serial.println(s);
    
    }
    ap_data[0].replace("\r","");
    ap_data[1].replace("\r","");
    ap_data[2].replace("\r","");
    //enableWifi();
   

}

void writeStoredTimeData(){


  File file = SPIFFS.open("/time.txt", "w");
  if (!file) {
    Serial.println("time.txt failed to open file for writing");
    return;

   }
   
  file.println(time_data[0]);
  file.println(time_data[1]);
 
  Serial.println("time data stored");
  Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  time file verify  *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");


  
  while (file.available()) {
    time_data.push_back(file.readStringUntil('\n'));
    Serial.println("validating time file....");
    }

   file.close();

   

   for (String s : time_data ) {
    
    Serial.println(s);
    
    }
    time_data[0].replace("\r","");
    time_data[1].replace("\r","");
    
    //enableWifi();
   

}


void FactoryResetStoredData(){

  
   //write wifi data factory
   
   File file = SPIFFS.open("/wifi.txt", "w");

   if (!file) {
    Serial.println("failed to open file for writting");
    return;
   }

   file.println("on");
   file.println("somenetwork");
   file.println("thisisapassword");
   

   
   file.close();
   
    
    
    // write ap data factory
    
    File file1 = SPIFFS.open("/ap.txt", "w");

   if (!file1) {
    Serial.println("failed to open file for writting");
    return;
   }
   
  

   file1.println("on");
   file1.println("mush-room");
   file1.println("86753099");

   file1.close();

    //write limit data factory
    
    File file2 = SPIFFS.open("/limit.txt", "w");

   if (!file2) {
    Serial.println("failed to open file for writting");
    return;
   }
   
   file2.println("70");
   file2.println("400");
   file2.println("40");
   
   file2.close();

      
  // time write factory

    File file3 = SPIFFS.open("/time.txt", "w");

   if (!file3) {
    Serial.println("failed to open time file for writting");
    return;
   }

   file3.println("PST");
   file3.println("00:00");
   
  
   file3.close();

   

   //begin light file read
   File file4 = SPIFFS.open("/light.txt", "w");

   if (!file4) {
    Serial.println("failed to open light file for writting");
    return;
   }
   
   
    file4.println("01:00");
    file4.println("00:00");
    file4.println("on");
    file4.close();

  
    ESP.reset();

        
};




void readStoredData(){

  
   //read wifi data
   
   File file = SPIFFS.open("/wifi.txt", "r");

   if (!file) {
    Serial.println("failed to open file for reading");
    return;
   }

   
   Serial.println('\n');
   Serial.println("**********************");
   Serial.println("*                    *");
   Serial.println("*  wifi file read    *");
   Serial.println("*                    *");
   Serial.println("**********************");
   Serial.println('\n');
   Serial.println("Contents:");

   while (file.available()) {
    wifi_data.push_back(file.readStringUntil('\n'));
    Serial.println("reading wifi file....");
    
    }
   file.close();
   Serial.println('\n');

   for (String s : wifi_data ) {
    
    
    Serial.println(s);
    
    }
    wifi_data[0].replace("\r","");
    wifi_data[1].replace("\r","");
    wifi_data[2].replace("\r","");
    
    
    // read ap data
    
    File file1 = SPIFFS.open("/ap.txt", "r");

   if (!file1) {
    Serial.println("failed to open file for reading");
    return;
   }
   
   while (file1.available()) {
    ap_data.push_back(file1.readStringUntil('\n'));
    Serial.println("reading ap file....");
    }
    
   file1.close();

   for (String s : ap_data ) {
    
    Serial.println(s);
    
    }

      ap_data[0].replace("\r","");
      ap_data[1].replace("\r","");
      ap_data[2].replace("\r","");


    //read limit data
    
    File file2 = SPIFFS.open("/limit.txt", "r");

   if (!file2) {
    Serial.println("failed to open file for reading");
    return;
   }

   

   while (file2.available()) {
    limit_data.push_back(file2.readStringUntil('\n'));
    Serial.println("reading limit file....");
    }
   file2.close();

   for (String s : limit_data ) {
    
    Serial.println(s);
    
    }

      limit_data[0].replace("\r","");
      limit_data[1].replace("\r","");
      limit_data[2].replace("\r","");

    File file3 = SPIFFS.open("/time.txt", "r");

   if (!file3) {
    Serial.println("failed to open time file for reading");
    return;
   }
   

   while (file3.available()) {
    time_data.push_back(file3.readStringUntil('\n'));
    Serial.println("reading time file....");
    }
   file3.close();

   for (String s : time_data ) {
    
    Serial.println(s);
    
    }

      time_data[0].replace("\r","");
      time_data[1].replace("\r","");
     

    tz = time_data[0];
    String current_time;
    current_time = time_data[1];
    clockSet(current_time);
   

   //begin light file read
   File file4 = SPIFFS.open("/light.txt", "r");

   if (!file4) {
    Serial.println("failed to open light file for reading");
    return;
   }
   

   while (file4.available()) {
    light_data.push_back(file4.readStringUntil('\n'));
    Serial.println("reading light file....");
    }
   file4.close();

   for (String s : light_data ) {
    
    Serial.println(s);
    
    }

   light_data[0].replace("\r","");
   light_data[1].replace("\r","");
   light_data[2].replace("\r","");
  


        
};






bool enableWifi(){

  uint8_t fail_count = 0;
  bool wifi_enable = false;
  
  WiFi.begin(wifi_data[1], wifi_data[2]);
  //WiFi.begin("garbage","datadata123");
  Serial.print("Connecting to ");
  Serial.print(wifi_data[1]);
  
  while (WiFi.status() != WL_CONNECTED && fail_count <= 9)
  {
    delay(1000);
    Serial.print(".");
    fail_count = fail_count +1;
    

    }
  if (fail_count == 10 ) {

    Serial.println("Wifi failure, do something bad on the screen ");
    WiFi.mode(WIFI_AP);

    return false;

  }else{

     Serial.println();
    Serial.print("Connected to WIFI ");
    Serial.println(WiFi.localIP());
    return true;
  }

 

}

void readTemp(){
  Temperature = dht.readTemperature(); // Gets the values of the temperature
  Humidity = dht.readHumidity(); // Gets the values of the humidity 
  //server.send(200, "text/html", SendHTML(Temperature,Humidity)); 
  Serial.println("DEBUG Temp");
  //String temp = (int)Temperature;
  Serial.println((int)Temperature);
  Serial.println((int)Temperature * 1.8 + 32);
  Temperature = Temperature * 1.8 +32;

  int temp;
  int carbon_dioxide;
  int hum;

  temp = limit_data[0].toInt();
  carbon_dioxide = limit_data[1].toInt();
  hum = limit_data[2].toInt();

  Serial.print("Temp Limit "); Serial.print(temp); Serial.println("");
  Serial.print("Co2 limit "); Serial.print(carbon_dioxide); Serial.println("");
  Serial.print("Humidity limit "); Serial.print(hum); Serial.println("");


}

void timeTrack() {

timeNow = millis()/1000; // the number of milliseconds that have passed since boot
seconds = timeNow - timeLast;//the number of seconds that have passed since the last time 60 seconds was reached.

  

if (seconds >= 60) {
  timeLast = timeNow;
  minutes = minutes + 1;
}



//if one minute has passed, start counting milliseconds from zero again and add one minute to the clock.

if (minutes == 60){ 
  minutes = 0;
  hours = hours + 1;
}

// if one hour has passed, start counting minutes from zero and add one hour to the clock

if (hours == 24){
  hours = 0;
  days = days + 1;
  }

  //if 24 hours have passed , add one day

if (hours ==(24 - startingHour) && correctedToday == 0){
  delay(dailyErrorFast*1000);
  seconds = seconds + dailyErrorBehind;
  correctedToday = 1;
}

//every time 24 hours have passed since the initial starting time and it has not been reset this day before, add milliseconds or delay the progran with some milliseconds. 
//Change these varialbes according to the error of your board. 
// The only way to find out how far off your boards internal clock is, is by uploading this sketch at exactly the same time as the real time, letting it run for a few days 
// and then determine how many seconds slow/fast your boards internal clock is on a daily average. (24 hours).

if (hours == 24 - startingHour + 2) { 
  correctedToday = 0;
}

}

String clockSet(String clockData) {

  String string_hours, string_minutes, string_seconds;

  int change_hours =  clockData.indexOf(':') ;
  int change_minutes = clockData.indexOf(':', change_hours+1) ;
  int change_seconds = clockData.indexOf(':', change_minutes+1) ;

  string_hours = clockData.substring(0, 2);
  string_minutes = clockData.substring(3, 5);
  string_seconds = clockData.substring(6, 8);
  
  Serial.println("clock data set");
  Serial.println(string_hours);
  Serial.println(string_minutes);
  Serial.println(string_seconds);

  hours = string_hours.toInt();
  minutes = string_minutes.toInt();
  seconds = 0;

  char output_time[8];

  sprintf (output_time,"%02d:%02d", hours, minutes);

  Serial.print("time is now:");
  Serial.println(output_time);
  time_data[1] = output_time;
  
   
  return output_time;
}








String clockOutput(){

char output_time[8];

  sprintf (output_time,"%02d:%02d", hours, minutes);

  return output_time;

}
// light functions


void allWhite() {

  uint8_t index;

  for ( index=0; index<20; index++){
    strip.setPixelColor(index, strip.Color(255,255,255));
  }
  Serial.println("light set white");
  strip.show();
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
} 







  
  
