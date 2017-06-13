#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>  
#include <MoodLamp.h>

#define MAX_PARAMS 5
#define MAX_JSON 255

String host_ip;

const int INIT_COLOR      = 360;
const int INIT_BRIGHTNESS = 0;

const byte PIN_RED   = D1;
const byte PIN_GREEN = D2;
const byte PIN_BLUE  = D3;


const char ROUTE_DEVICE[]          =  "/moodlamp";
//const char ROUTE_COMMANDS[]        =  "/moodlamp/commands";
const char ROUTE_COLOR_RED[]       =  "/moodlamp/red";
const char ROUTE_COLOR_ORANGE[]    =  "/moodlamp/orange";
const char ROUTE_COLOR_YELLOW[]    =  "/moodlamp/yellow";
const char ROUTE_COLOR_LIME[]      =  "/moodlamp/lime";
const char ROUTE_COLOR_GREEN[]     =  "/moodlamp/green";
const char ROUTE_COLOR_AQUA[]      =  "/moodlamp/aqua";
const char ROUTE_COLOR_CYAN[]      =  "/moodlamp/cyan";
const char ROUTE_COLOR_OCEAN[]     =  "/moodlamp/ocean";
const char ROUTE_COLOR_BLUE[]      =  "/moodlamp/blue";
const char ROUTE_COLOR_PURPLE[]    =  "/moodlamp/purple";
const char ROUTE_COLOR_VIOLET[]    =  "/moodlamp/violet";
const char ROUTE_COLOR_MAGENTA[]   =  "/moodlamp/magenta";
const char ROUTE_COLOR_PINK[]      =  "/moodlamp/pink";
const char ROUTE_COLOR_WHITE[]     =  "/moodlamp/white";

const char ROUTE_DEVICE_ON[]       =  "/moodlamp/on";
const char ROUTE_DEVICE_OFF[]      =  "/moodlamp/off";

const char ROUTE_COLOR_WHEEL[]     =  "/moodlamp/wheel";
const char ROUTE_COLOR_FADE[]      =  "/moodlamp/fade";


const char  DEVICE_NAME[]  = "moodlamp";

  

  const char * DEVICE_PARAM_COLOR = "color";
  const char * DEVICE_PARAM_BRIGHTNESS = "brightness";
  const char * DEVICE_PARAM_SPEED = "speed";
  const char * DEVICE_PARAM_ACTIVE = "active";





ESP8266WebServer server(80);

MoodLamp moodlamp(PIN_RED,PIN_GREEN,PIN_BLUE);


void sendJson(){
  
  String s;
  StaticJsonBuffer<MAX_JSON> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
   
  root["name"] = DEVICE_NAME;
  root[DEVICE_PARAM_COLOR] = moodlamp.color();
  root[DEVICE_PARAM_BRIGHTNESS] = moodlamp.brightness();

  JsonObject& wheel = root.createNestedObject("wheel");
  wheel[DEVICE_PARAM_ACTIVE] = moodlamp.wheel();
  wheel[DEVICE_PARAM_SPEED] = moodlamp.wheel_speed();

  JsonObject& fade = root.createNestedObject("fade");
  fade[DEVICE_PARAM_ACTIVE] = moodlamp.fade();
  fade[DEVICE_PARAM_SPEED] = moodlamp.fade_speed();

  
  root.printTo(s);
  server.setContentLength(root.measureLength());
  server.send(200, "application/json", s);
  
}


void handleRoot() {
  
 String s;
  
 StaticJsonBuffer<200> jsonBuffer;
 JsonObject& root = jsonBuffer.createObject();
 JsonArray& devices = root.createNestedArray("devices");
 JsonObject& device_moodlamp = devices.createNestedObject();
 device_moodlamp["route"] = ROUTE_DEVICE;
 device_moodlamp["name"] = DEVICE_NAME; 
  
 root.printTo(s);
 
 server.setContentLength(root.measureLength());
 server.send(200, "application/json", s);
  
}

void changeColor(int color, int brightness){
  String s;
  
  moodlamp.color(color);
  moodlamp.brightness(brightness);

  sendJson();

  
}


void handleChange(){

  String s;
  String color;
  String brightness;

  color = server.arg("color");
  brightness = server.arg("brightness");

  if(color != ""){
    moodlamp.color(color.toInt());
  }

  if(brightness != ""){
    moodlamp.brightness(brightness.toInt());
  }
  
  sendJson();
  
}

void handleColorRed(){
  changeColor(COLOR_RED, moodlamp.last_brightness());
}

void handleColorOrange(){
  changeColor(COLOR_ORANGE,moodlamp.last_brightness());
}

void handleColorYellow(){
  changeColor(COLOR_YELLOW, moodlamp.last_brightness());
}

void handleColorLime(){
  changeColor(COLOR_LIME, moodlamp.last_brightness());
}

void handleColorGreen(){
  changeColor(COLOR_GREEN, moodlamp.last_brightness());
}

void handleColorAqua(){
  changeColor(COLOR_AQUA, moodlamp.last_brightness());
}
void handleColorCyan(){
  changeColor(COLOR_CYAN, moodlamp.last_brightness());
}

void handleColorOcean(){
  changeColor(COLOR_OCEAN, moodlamp.last_brightness());
}

void handleColorBlue(){
  changeColor(COLOR_BLUE, moodlamp.last_brightness());
}

void handleColorPurple(){
  changeColor(COLOR_PURPLE, moodlamp.last_brightness());
}

void handleColorViolet(){
  changeColor(COLOR_VIOLET, moodlamp.last_brightness());
}

void handleColorMagenta(){
  changeColor(COLOR_MAGENTA, moodlamp.last_brightness());
}

void handleColorPink(){
  changeColor(COLOR_PINK, moodlamp.last_brightness());
}

void handleColorWhite(){
  changeColor(COLOR_WHITE, moodlamp.last_brightness());
}

void handleColorOn(){
  changeColor(moodlamp.color(),moodlamp.last_brightness());
  
  sendJson();
}

void handleColorOff(){
  changeColor(moodlamp.color(),0);
  sendJson();
}

void handleColorWheel(){

  String speed;
  String active;

  speed = server.arg("speed");
  active = server.arg("active");

  if(active != ""){
    moodlamp.wheel(active.toInt());
  }

  if(speed != ""){
    moodlamp.wheel_speed(speed.toInt());
  }

  sendJson();

  
}
void handleColorFade(){

  String speed;
  String active;

  speed = server.arg("speed");
  active = server.arg("active");

  if(active != ""){
    moodlamp.fade(active.toInt());
  }

  if(speed != ""){
    moodlamp.fade_speed(speed.toInt());
  }

  sendJson();
}


void handleNotFound(){


  String s;
  StaticJsonBuffer<MAX_JSON> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
   
  root["route"] = ROUTE_DEVICE;
  root["name"] = DEVICE_NAME; 
  root["error"] = "Invalid Request";
    
  root.printTo(s);
  server.setContentLength(root.measureLength());
  server.send(404, "application/json", s);

}

void setup(void){

  
    // put your setup code here, to run once:
  Serial.setRxBufferSize(1024);
  Serial.begin(115200);

    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
    //reset saved settings
    //wifiManager.resetSettings();
    
    //set custom ip for portal
    //wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("AutoConnectAP");
    //or use this for auto generated name ESP + ChipID
    //wifiManager.autoConnect();

    
    //if you get here you have connected to the WiFi
      Serial.print ( "IP address: " );
      
  Serial.println ( WiFi.localIP() );
 Serial.println("connected...yeey :)");


 server.on ( "/", handleRoot );

 server.on("/moodlamp",handleChange);
 
 server.on("/moodlamp/red",handleColorRed);
 server.on("/moodlamp/orange",handleColorOrange);
 server.on("/moodlamp/yellow",handleColorYellow);
 server.on("/moodlamp/lime",handleColorLime);
 
 server.on("/moodlamp/green",handleColorGreen);
 server.on("/moodlamp/aqua",handleColorAqua);
 server.on("/moodlamp/cyan",handleColorCyan);
 server.on("/moodlamp/ocean",handleColorOcean);
 
 server.on("/moodlamp/blue",handleColorBlue);
 server.on("/moodlamp/purple",handleColorPurple);
 server.on("/moodlamp/violet",handleColorViolet);
 server.on("/moodlamp/magenta",handleColorMagenta);
 server.on("/moodlamp/pink",handleColorPink);

 server.on("/moodlamp/white",handleColorWhite);
 server.on("/moodlamp/on",handleColorOn);
 server.on("/moodlamp/off",handleColorOff);

 server.on("/moodlamp/fade",handleColorFade);
 server.on("/moodlamp/wheel",handleColorWheel);

 
 
 server.onNotFound(handleNotFound);
 
 server.begin();
 
 moodlamp.begin();
 moodlamp.color(INIT_COLOR);
 moodlamp.brightness(INIT_BRIGHTNESS);
 
 Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
  moodlamp.update();

}
