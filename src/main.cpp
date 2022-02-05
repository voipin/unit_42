

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
void displayMenu();

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

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

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
};





uint32_t getAbsoluteHumidity(float temperature, float humidity) {
    // approximation formula from Sensirion SGP30 Driver Integration chapter 3.15
    const float absoluteHumidity = 216.7f * ((humidity / 100.0f) * 6.112f * exp((17.62f * temperature) / (243.12f + temperature)) / (273.15f + temperature)); // [g/m^3]
    const uint32_t absoluteHumidityScaled = static_cast<uint32_t>(1000.0f * absoluteHumidity); // [mg/m^3]
    return absoluteHumidityScaled;
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
  
  
