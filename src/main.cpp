

#include <Wire.h>
#include <stdio.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <U8g2lib.h>
#include "Adafruit_SGP30.h"
#include "FS.h"
#include <pu2clr_pcf8574.h>
#include <string.h>
#include "DHT.h"
#include "sendHTMLh"
#include "menuState.h"
#include "allMenuDisplay.h"
#include "webActions.h"

#define DHTTYPE DHT22
#define AP_SSID "mush-room"
#define AP_PASS "8675309"

using namespace std;

vector <String> wifi_data;
vector <String> ap_data;
vector <String> limit_data;



uint8_t data1;
uint8_t data2;
uint8_t data3;
uint8_t data_state1;
uint8_t data_state2;
uint8_t data_state3;

void handleRoot();
void handleNotFound();
void readStoredData();
void enableWifi();
void handleForm();
void fileindex();
void bootstrap();
void handle_RelayOn();
void handle_RelayOff();
void readTemp();
void writeStoredWifiData();
void writeStoredApData();
void jquery();
void jquerySlim();
void jqueryFull();
void popper();
void bootstrapmin();
void updateSensor();
<<<<<<< HEAD
void handle_disableWifi();
=======
void displayMenu();
>>>>>>> 81c419ccdce815b0dec02d5ed74b66681f81e9d4

int loopTime = 1000;
bool displayed = 0;
String state_menu = "sensor_data";
char wifi_sid[10];
String wifi_pw = "";
char wifi_sid_add[0];
int alert_temp = 85;
int alert_co2 = 600;
int alert_hum = 60;

int relay_temp = 85;
int relay_co2 = 600;
int relay_hum = 50;

bool relay_hum_toggle = 0;
bool relay_fan_toggle = 0;

String stateMenu = "sensor_data";

char *menuDisplay[] = {"one", "two", "three"};



char relay_temp_s[3];
char buff[4];
char *buff1;
char asciiChar[0];

int menu_state = 0;
int menuSize = 0 ;
int charCount = 65;
int totalChars = 0;


float Temperature;
float Humidity;

uint8_t DHTPin = D6; 
DHT dht(DHTPin, DHTTYPE); 

uint8_t Push_button_2 = D5;
uint8_t Push_button_1 = D6;
uint8_t Push_button_3 = D7;
uint8_t Relay1 = D8;

int counter = 0;
int button_counter = 0;

const char* ssid = "mush-room";
const char* pass = "86753099";

uint8_t history[1024];

PCF pcf;
Adafruit_SGP30 sgp;

ESP8266WebServer server(80);

<<<<<<< HEAD
String warnHTML(String alertText) {

  String ptr="<script>\n";
  ptr += "alert( \" ";
  ptr += alertText;
  ptr += " \") \n ";
  ptr += "window.location =\"/\"";
  ptr += "</script>\n";

  return ptr;
}

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
=======
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
>>>>>>> 81c419ccdce815b0dec02d5ed74b66681f81e9d4

void u8g2_prepare(){
u8g2.setFont(u8g2_font_helvB18_tf); u8g2.setFontRefHeightExtendedText(); u8g2.setDrawColor(1); u8g2.setFontPosTop(); u8g2.setFontDirection(0);
};

String updateSensorAjax(int Temperature, int Humidity, int co2){

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
  return ptr;
<<<<<<< HEAD
}

String sendHTML(String form_relay_toggle_fan, vector <String> wifi_data, int Temperature, int Humidity, int co2, vector <String> ap_data) {

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
ptr +="    <link href=\"pricing.css\" rel=\"stylesheet\">\n";
ptr +="</head>\n";
ptr +="<body>\n";
ptr +="<style>\n";
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
ptr +="<script>";

ptr +="var intervalID = setInterval(function() {";

ptr +="            $(\"#sensor_update\").load(\"update_sensor\");";
ptr +="            console.log(\"update sensors\");";

ptr +="    }, 5000);";
ptr +="</script>";


ptr +="<div class=\"form-group\">\n";
ptr +="<form class=\"form-group\" action=\"/submit_page\">\n";
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
ptr +="        <div class=\"card box-shadow mx-auto\" style=\"width: 18rem;\" >\n";
ptr +="            <div class=\"card-header\">\n";
ptr +="                <h4 class=\"my-0 font-weight-normal\">Wifi</h4>\n";
ptr +="            </div>\n";
ptr +="            <div class=\"card-body mx-auto\"\">\n";
ptr +="                <ul class=\"list-unstyled\">\n";
    if (wifi_data[0] == "on") {
        ptr +=" <li class=\"li-more-margin\">Enable Local Network : <input name=\"cl_active\" type=\"checkbox\"/ checked></li>\n";
         }else {
        ptr +="                    <li class=\"li-more-margin\">Enable Local Network : <input name=\"cl_active\" type=\"checkbox\"/></li>\n";
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
ptr +="                    <li class=\"li-more-margin\">Enable AP Network : <input class=\"c\" name=\"ap_active\" type=\"checkbox\"/ checked></li>\n";
}else{
ptr +="                    <li class=\"li-more-margin\">Enable AP Network : <input class=\"c\" name=\"ap_active\" type=\"checkbox\"/ ></li>\n";
}
ptr +="                    <li>SSID <input value=\"";
ptr +=ap_data[1];
ptr += "\" name=\"ap_ssid\" size=\"10\"/></li>\n";

ptr +="                    <li>Pass <input value=\"";
ptr +=ap_data[2];
ptr += "\" name=\"ap_pass\" size=\"10\"/></li>\n";               
ptr +="                </ul>\n";
ptr +="            </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";
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
ptr +="                            <td class=\"temp-input\" ><input name=\"relay_limit_temp\" size=\"3\" value=\"80\"/> </td>\n";
ptr +="                        </tr>\n";
ptr +="                    </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Co2 </td>\n";
ptr +="                                <td class=\"relay-input\"> <input size=\"3\" name=\"relay_limit_co2\" value=\"600\"/> </td>\n";
ptr +="                            </tr>\n";
ptr +="                        </table>\n";
ptr +="                    </li>\n";
ptr +="                    <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                        <table>\n";
ptr +="                            <tr>\n";
ptr +="                                <td class=\"relay-text\"> Humidity </td>\n";
ptr +="                                <td class=\"relay-input\"> <input name=\"relay_limit_hum\" size=\"3\" value=\"45\"/> </td>\n";
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


ptr +="                    <ul class=\"list-group\">\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";
ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\">Fan</td>\n";
if (form_relay_toggle_fan == "on"){
  ptr +="                                    <td class=\"relay-input\"><input value=\"on\" name=\"relay_toggle_fan\" type=\"checkbox\" checked/></td>\n";

  }else
  {
    ptr +="                                    <td class=\"relay-input\"> <input name=\"relay_toggle_fan\" type=\"checkbox\" /></td>\n";
    }

ptr +="                                </tr>\n";

ptr +="                            </table>\n";
ptr +="                            <span class=\"badge badge-primary badge-pill\">14</span>\n";
ptr +="                        </li>\n";
ptr +="                        <li class=\"list-group-item d-flex justify-content-between align-items-center\">\n";

ptr +="                            <table>\n";
ptr +="                                <tr>\n";
ptr +="                                    <td class=\"relay-text\" >Humidity </td>\n";
ptr +="                                    <td class=\"relay-input\"> <input name=\"relay_toggle_hum\" type=\"checkbox\" /> </td>\n";
ptr +="                                </tr>\n";

ptr +="                            </table>\n";

ptr +="                            <span class=\"badge badge-primary badge-pill\">2</span>\n";
ptr +="                        </li>\n";

ptr +="                    </ul>\n";

ptr +="                </div>\n";

ptr +="            </div>\n";
ptr +="            <button type=\"submit\" class=\"btn btn-lg btn-block btn-primary\">Save</button>\n";
ptr +="        </div>\n";
ptr +="</form>\n";
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
=======
};
>>>>>>> 81c419ccdce815b0dec02d5ed74b66681f81e9d4





uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
};


<<<<<<< HEAD
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
 stateMenu == "relay_toggle_hum");



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
      stateMenu = "wifi";
      displayed = 0;
      loopTime = 200;
      //menuDisplay[0] = { "wifi-menu" };
      //menuDisplay[1] = { ssid };
      //menuDisplay[2] = { pass };
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
  u8g2.print("2. wifi ");
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

  } else {
  
    if (WiFi.softAPIP()) {
      
      u8g2.setCursor(12,10);
      u8g2.print("http://");
      u8g2.print(WiFi.softAPIP());

    } else {

      u8g2.setCursor(12,10);
      u8g2.print("No Wifi active");
      u8g2.print(WiFi.localIP());

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
  //u8g2.sendBuffer();
  //updateOnChange();
  
  if (memcmp(history,u8g2.getBufferPtr(),1024)){
    memcpy(history, u8g2.getBufferPtr(), 1024);
    
    Serial.println("changed!");
    u8g2.sendBuffer();
    
    }    

}

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










// SETUP //////////////////////////////////////////////////////////////////////////////////////////////////////





=======
>>>>>>> 81c419ccdce815b0dec02d5ed74b66681f81e9d4
void setup() {
  Serial.begin(115200);
  while (!Serial); 
  pcf.setup(0x20);        // The PCF8574 is configured to 0x20 I2C address. Check A0, A1 and A2 pins of the device.
  pcf.write(0B11111111);  // Turns all pins/ports HIGH
  Wire.begin();  
  sgp.begin();
  pinMode(DHTPin, INPUT);

  dht.begin();
  u8g2.begin();
  
  WiFi.mode(WIFI_AP_STA);
 
  
  u8g2.enableUTF8Print();
  //SPIFFS.begin();
  if ( !SPIFFS.begin() ) {
    Serial.println("Error file system has occured while mounting");
    }else{
  readStoredData();
   
    }
  
  String active = "on";
  
  Serial.println("Wifi data:");


  
  Serial.println(wifi_data[0]);
  
  Serial.println(wifi_data[0].length());
  Serial.println(active.length());
  Serial.println(wifi_data[1]);
  Serial.println(wifi_data[2]);
  if (ap_data[0] == "on" ) {
    WiFi.softAP(ap_data[1], ap_data[2]);
  }
  
  
  if (wifi_data[0] == "on" ) {
    Serial.println("Wifi enabled");
    enableWifi();
  }
  
   

  

  server.begin();
    
  pinMode(Push_button_1,INPUT);
  pinMode(Push_button_2,INPUT);
  pinMode(Push_button_3,INPUT);
  pinMode(Relay1,OUTPUT);

  //WiFi.mode(WIFI_AP);
  
  server.on("/", handleRoot(server));
  server.on("/submit_page", handleForm);
  server.on("/test", fileindex);
  server.onNotFound(handleNotFound);
  server.on("/bootstrap.min.css", bootstrap);
  server.on("/jquery1.js", jquery);
  server.on("/jquery2.min.js", jqueryFull);
  server.on("/jquery3.min.js", jquerySlim);
  server.on("bootstrap.min.css", bootstrap);
  server.on("/popper.min.js", popper);
  server.on("/bootstrap.min.js", bootstrapmin);
  server.on("bootstrap.min.js", bootstrapmin);
  server.on("/update_sensor",updateSensor(server));

  server.on("/relay_on", handle_RelayOn);
  server.on("/relay_off", handle_RelayOff);

  server.on("/fan_relay_on", handle_fanRelayOn);
  server.on("/fan_relay_off", handle_fanRelayOff);

  server.on("/humidity_relay_on", handle_fanRelayOn);
  server.on("/humidity_relay_off", handle_fanRelayOff);

  server.on("/disable_wifi", handle_disableWifi);



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



void loop() {

  

  server.handleClient();
  
 

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
      
      
       }

  if (data1 == HIGH && data_state1)
    {
      data_state1 = 0;  
      Serial.println("button 1 released");
    }
  
  //button 2 state
      if (data2 == LOW && !data_state2)
    {
      data_state2 = 1;  
      Serial.println("button 2 pressed");

      menuState(data1,data2,data3);
    }

    
    if (data2 == HIGH && data_state2)
    {
      data_state2 = 0;  
      Serial.println("button 2 released");
      
    }
    
    //button 3 state
    if (data3 == LOW && !data_state3)
    {
      data_state3 = 1;  
      Serial.println("button 3 pressed");

      if (stateMenu != "sensor_data") {
          menuState(data1,data2,data3);
      }
    }


    if (data3 == HIGH && data_state3)
    {
      data_state3 = 0;  
      Serial.println("button 3 released");
      
    }

  
  if (stateMenu != "sensor_data") {
   
   //delay(200);
   
   displayMenu(&u8g2);
   

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


<<<<<<< HEAD
void handle_RelayOn(){
Serial.println("relay on");
 digitalWrite(Relay1, HIGH);
};

void handle_RelayOff(){
  Serial.println("relay off");
 digitalWrite(Relay1, LOW);
};

void handle_disableWifi() {

  Serial.println("WARNING! wifi disabled completely");
  ap_data[0] ="\n";
  wifi_data[0] = "\n";

  writeStoredApData();
  writeStoredWifiData();

  String alertText = " all wifi, both direct access and remote connectivity has been disabled.";

  server.send(200, "text/html", warnHTML(alertText) );

  delay(1000);

  ESP.reset();

}

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

  Serial.println(form_ap_active);
  Serial.println(form_ap_pass);
  Serial.println(form_ap_ssid);
  Serial.println(form_cl_pass);
  Serial.println(form_cl_ssid);
  Serial.println(form_relay_limit_temp);
  Serial.println(form_relay_limit_co2);
  Serial.println(form_relay_toggle_hum);
  Serial.println(form_relay_toggle_fan);

  
   
   // no form of wifi is active, probabaly a bad thing
   if(form_ap_active != "on" && form_cl_active != "on")
   
   {

     String alertText = "You are about to remove all wireless access, both ap and client. ";
     alertText += "You would have to use the manual controls to turn these on again, continue? ";

    server.send(200, "text/html", choiceHTML(alertText,"disableWifi","") );

    return;
    
    }

   if ( (form_ap_ssid !=ap_data[1] || form_cl_pass !=ap_data[2])  && ap_data[0]=="on")
   {
    
    ap_data[0] = form_ap_active;
    ap_data[1] = form_ap_ssid;
    ap_data[2] = form_ap_pass;
    writeStoredApData();

    Serial.println("write stored AP data, reseting");

    resetFlag=true;

    }else { 
      
      if (form_ap_active != ap_data[0] || form_ap_ssid != ap_data[1] || form_ap_pass != ap_data[2])
      
      {
      
      ap_data[0] = form_ap_active;
      ap_data[1] = form_ap_ssid;
      ap_data[2] = form_ap_pass;
      writeStoredApData();
      
      }
    }

   if ( (form_cl_ssid !=wifi_data[1] || form_cl_pass !=wifi_data[2]) && wifi_data[0]=="on" )
   
   {

    wifi_data[0] = form_cl_active;
    wifi_data[1] = form_cl_ssid;
    wifi_data[2] = form_cl_pass; 
    writeStoredWifiData();

    Serial.println("write stored Wifi data, reseting");

     resetFlag=true;

   }else {

    if (form_cl_active !=wifi_data[0] || form_cl_ssid != wifi_data[1] || form_cl_pass !=wifi_data[2])
    {


    wifi_data[0] = form_cl_active;
    wifi_data[1] = form_cl_ssid;
    wifi_data[2] = form_cl_pass;
    writeStoredWifiData();}

   };

   if (resetFlag){

     //check for wifi station connection
     //check for connection to wifi client
     
     String alertText = "Don't Panic! You have changed the wireless connectivity and may need to reconnect.";
     alertText += " The new information for connecting to Unit 42 is displayed on the front of the device.";
     
     server.send(200, "text/html", warnHTML(alertText) );

     delay(1000);

     ESP.reset();
   }
  

  if (form_relay_toggle_fan == "on")
  {
    handle_RelayOn();
    }
    else
    {
      handle_RelayOff();
      }
    
  server.send(200, "text/html", sendHTML(form_relay_toggle_fan, wifi_data, Temperature, Humidity, sgp.eCO2, ap_data) );
  
  webUpdate();

  
}
=======
>>>>>>> 81c419ccdce815b0dec02d5ed74b66681f81e9d4




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
    
   

}







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

    


    
    
};

void enableWifi(){
  
  WiFi.begin(wifi_data[1], wifi_data[2]);
  Serial.print("Connecting to ");
  Serial.print(wifi_data[1]);
  
  while (WiFi.status() != WL_CONNECTED )
  {
    delay(1000);
    Serial.print(".");
    }

  Serial.println();
  Serial.print("Connected to WIFI ");
  Serial.println(WiFi.localIP());
 

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
}
  
  
