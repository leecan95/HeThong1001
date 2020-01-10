#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <WiFiUdp.h>
#include<string.h>
#include <ArduinoJson.h>
const byte RX = D1;
const byte TX = D2;
const char* ssid = "VTNET_WIFI_FREE";
const char* password =  "87654321";
const char* mqttServer = "m24.cloudmqtt.com";
const int mqttPort = 14022;
const char* mqttUser = "myuncjoa";
const char* mqttPassword = "SvVx1BPwTVqk";
char json[] = "[{\"name\":\"esp8266\",\"id\": \"dv69\"}]"; 
char Aqi[100];
char control[100];
//UART
SoftwareSerial mySerial(RX, TX, false, 256);
unsigned long previousMillis = 0;
const long interval = 1000;
String inNumber[13] = "";
String characterString[13] = "";
char test[30];
uint8_t ch[5]= {}; 
int i = 0;
int j = 0;
String point = "";
//End
//Parameter
char hour[] = "";
char temparature[10] = "";
char humidity[10] = "";
char PM25[10]  = "";
String VOCC  = "";
String co2  = "";
int Gspeed = 0;
int POWER = 0;
int NIGHT = 0;
int FILTER = 0;
int CTRL = 0;
char power[10] ="";
char Speed[10]="";
char night_mode[10] ="";
char filter_mode[10]="";
char control_mode[10]="";
WiFiClient espClient;
PubSubClient client(espClient);
//get time
const long utcOffsetInSeconds = 25200;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int FLAG = 0;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup() {
 
  Serial.begin(115200);
  mySerial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  timeClient.begin();
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
 
    }
  }
 
  
  client.subscribe("esp/request/collect/device-info");
  client.subscribe("esp/request/control/power-on");
  client.subscribe("esp/request/control/power-off");
  client.subscribe("esp/request/control/low");
  client.subscribe("esp/request/control/med");
  client.subscribe("esp/request/control/high");
  client.subscribe("esp/response/collect/device-info");
  client.subscribe("esp/response/collect/power-state");
  client.subscribe("esp/request/collect/speed-state");
  client.subscribe("esp/request/collect/power-state");
  client.subscribe("esp/request/collect/control-state");
  client.subscribe("esp/request/collect/temperature");
  client.subscribe("esp/request/collect/humidity");
  client.subscribe("esp/request/collect/PM25");
  client.subscribe("esp/request/collect/co2");
  client.subscribe("esp/request/collect/vooc");
  client.subscribe("esp/request/collect/air-quality");
  client.subscribe("esp/request/collect/night_mode");
  client.subscribe("esp/request/collect/filter_mode");
  client.subscribe("esp/request/collect/control_mode");
  client.subscribe("esp/request/control/night_mode");
  client.subscribe("esp/request/control/filter_mode");
  client.subscribe("esp/request/control/control_mode");
  
 
}
 
void callback(char* topic, byte* payload, unsigned int length) {  
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  //for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
  //}
   if (strcmp(topic,"esp/request/collect/device-info")==0){
  //Serial.print("Message info:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'i' && payload[i-2] == 'n'&&payload[i-1] == 'f' && payload[i] == 'o'){
       //Serial.print("Send info:");
       mySerial.print("start\r\n");
       client.publish("esp/response/collect/device-info", json);
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/temperature")==0){
   // Serial.print("Message temp:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
      
       mySerial.print("start\r\n");
       client.publish("esp/response/collect/temperature",temparature);
       
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/air-quality")==0){
    Serial.print("Message IAQ:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send temp:");
       sprintf(Aqi,"{\"temperature\" : \"%s\",\"humidity\" : \"%s\",\"pm25\" : \"%s\",\"co2\" : \"%s\",\"voc\" : \"%s\"}",temparature,humidity,PM25,co2.c_str(),VOCC.c_str());
       mySerial.print("start\r\n");
       client.publish("esp/response/collect/air-quality",Aqi);
      
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/control-state")==0){
    Serial.print("Message control:");
  //for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
   // if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
         sprintf(control,"{\"power\":\"%s\",\"speed\": \"%s\",\"night\":\"%s\",\"filter\": \"%s\",\"control\": \"%s\"}", power, Speed, night_mode, filter_mode, control_mode);
         client.publish("esp/response/collect/control-state",control); 
       
   //}
  //}
  }
 if (strcmp(topic,"esp/request/collect/humidity")==0){
  //Serial.print("Message hum:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send hum:");
       client.publish("esp/response/collect/humidity",humidity);
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/PM25")==0){
  //Serial.print("Message PM25:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send PM25:");
       client.publish("esp/response/collect/PM25",PM25);
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/vooc")==0){
  //Serial.print("Message vooc:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send vooc:");
       client.publish("esp/response/collect/vooc",VOCC.c_str());
    }
  }
  }
  if (strcmp(topic,"esp/request/collect/co2")==0){
 // Serial.print("Message co2:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
      // Serial.print("Send co2:");
       client.publish("esp/response/collect/co2",co2.c_str());
    }
  }
  }
 if (strcmp(topic,"esp/request/control/med")==0){
  //Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    //Serial.print((char)payload[i]);
    if(payload[i-1] == '6' && payload[i] == '9'){
     // Serial.print("OKE medium");
      mySerial.print("medium\r\n");
      client.publish("esp/response/collect/speed-state", "2");  
      //delay(20);
    }
  }
  }
 if (strcmp(topic,"esp/request/control/low")==0){
 // Serial.print("Message 1:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-1] == '6' && payload[i] == '9'){
    //  Serial.print("OKE low");
      mySerial.print("low\r\n");   
      client.publish("esp/response/collect/speed-state", "1");  
      //delay(20);
    }
  }
  }
 if (strcmp(topic,"esp/request/control/high")==0){
  //Serial.print("Message 2:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-1] == '6' && payload[i] == '9'){
     // Serial.print("OKE high");
      mySerial.print("high\r\n");  
      client.publish("esp/response/collect/speed-state", "3"); 
      //delay(20);
    }
  }
  }
 if (strcmp(topic,"esp/request/control/power-on")==0){
  Serial.print("Message on:");
  //for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    //if(payload[i] == '6' && payload[i+1] == '9'){
      Serial.print("OKE on");
      mySerial.print("PON\r\n"); 
      client.publish("esp/response/collect/power-state", "ON");   
      //delay(20);
      client.publish("esp/response/collect/speed-state", "2");  
   // }
 // }
  }
 if (strcmp(topic,"esp/request/control/power-off")==0){
  Serial.print("Message off:");
  //for (int i = 0; i < length; i++) {
    // Serial.print((char)payload[i]);
  // if(payload[i] == '6' && payload[i+1] == '9'){
      Serial.print("OKE off");  
      mySerial.print("POFF\r\n"); 
      client.publish("esp/response/collect/power-state", "OFF");
      //delay(20);
      client.publish("esp/response/collect/speed-state", "0");   
    //}
  //}
  }
  
  if (strcmp(topic,"esp/request/control/night_mode")==0){
  
    for (int i = 0; i < length; i++) {
      //Serial.print((char)payload[i]);
      if(payload[i] == '6' && payload[i+1] == '9'&& payload[i+2] == '-'&& payload[i+3] == 'o'&& payload[i+4] == 'n'){
        Serial.print("night on");  
         mySerial.print("night\r\n"); 
         client.publish("esp/response/collect/night_mode", "on");   
    }
      if(payload[i] == '6' && payload[i+1] == '9'&& payload[i+2] == '-'&& payload[i+3] == 'o'&& payload[i+4] == 'f'){
      Serial.print("night off");  
      mySerial.print("day\r\n");     
      client.publish("esp/response/collect/night_mode", "off"); 
    }
  }
  }
  

  if (strcmp(topic,"esp/request/control/filter_mode")==0){
  
   for (int i = 0; i < length; i++) {
      //Serial.print((char)payload[i]);
     if(payload[i-4] == '6' && payload[i-3] == '9'&& payload[i-2] == '-'&& payload[i-1] == 'f'&& payload[i] == 'r'){
        Serial.print("fresh-air");  
        mySerial.print("fresh\r\n"); 
        client.publish("esp/response/collect/filter_mode", "fresh");   
      }
   else if(payload[i-4] == '6' && payload[i-3] == '9'&& payload[i-2] == '-'&& payload[i-1] == 'i'&& payload[i] == 'n'){
       Serial.print("indoor");  
       mySerial.print("indoor\r\n");     
        client.publish("esp/response/collect/filter_mode", "indoor"); 
    }
  }
  }

  if (strcmp(topic,"esp/request/control/control_mode")==0){
  
    for (int i = 0; i < length; i++) {
      //Serial.print((char)payload[i]);
      if(payload[i-4] == '6' && payload[i-3] == '9'&& payload[i-2] == '-'&& payload[i-1] == 'a'&& payload[i] == 'u'){
     // Serial.print("auto");  
      mySerial.print("auto\r\n"); 
      client.publish("esp/response/collect/control_mode", "auto");   
    }
    else if(payload[i-4] == '6' && payload[i-3] == '9'&& payload[i-2] == '-'&& payload[i-1] == 'm'&& payload[i] == 'a'){
      //Serial.print("manual");  
      mySerial.print("manual\r\n");     
      client.publish("esp/response/collect/control_mode", "manual"); 
    }
  }
  }
  
  if (strcmp(topic,"esp/request/collect/night_mode")==0){
  //Serial.print("Message nightmode:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
      // Serial.print("Send night:");
       if (NIGHT == 1)
       client.publish("esp/response/collect/night_mode","on");
       else if(NIGHT == 0)
       client.publish("esp/response/collect/night_mode","off");
    }
  }
  }

  if (strcmp(topic,"esp/request/collect/filter_mode")==0){
  //Serial.print("Message filtermode:");
    for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
      if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       Serial.print("Send filter:");
       if (FILTER == 1)
       client.publish("esp/response/collect/filter_mode","fresh");
       else if(FILTER == 0)
       client.publish("esp/response/collect/filter_mode","indoor");
     }
    }
  }

   if (strcmp(topic,"esp/request/collect/control_mode")==0){
  //Serial.print("Message control_mode:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send control:");
       if (CTRL == 1)
       client.publish("esp/response/collect/control_mode","manual");
       else if(CTRL == 0)
       client.publish("esp/response/collect/control_mode","auto");
    }
  }
  }
 if (strcmp(topic,"esp/request/collect/power-state")==0){
  Serial.print("Message power:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send power:");
       if (POWER == 1)
       client.publish("esp/response/collect/power-state","ON");
       else if(POWER == 0)
       client.publish("esp/response/collect/power-state","OFF");
    }
  }
  }
 if (strcmp(topic,"esp/request/collect/speed-state")==0){
  //Serial.print("Message speed:");
  for (int i = 0; i < length; i++) {
     //Serial.print((char)payload[i]);
    if(payload[i-3] == 'd' && payload[i-2] == 'v'&&payload[i-1] == '6' && payload[i] == '9'){
       //Serial.print("Send speed:");
       if (Gspeed == 0){
        client.publish("esp/response/collect/speed-state","0");
       }
       else if (Gspeed == 1){
        client.publish("esp/response/collect/speed-state","1");
       }
       else if(Gspeed == 2){
        client.publish("esp/response/collect/speed-state","2");
       }
       else if(Gspeed == 3){
        client.publish("esp/response/collect/speed-state","3");
       }
       
    }
  }
  }
  
  //Serial.println();
  //Serial.println("-----------------------");
 
}
void clearall(){
      i = 0;
      inNumber[0] = "";
      inNumber[1] = "";
      inNumber[2] = "";
      inNumber[3] = "";
      inNumber[4] = "";
      inNumber[5] = "";
      inNumber[6] = "";
      inNumber[7] = "";
      inNumber[8] = "";
      inNumber[9] = "";
      inNumber[10] = "";
      inNumber[11] = "";
      inNumber[12] = "";
      characterString[0] = "";
      characterString[1] = "";
      characterString[2] = "";
      characterString[3] = "";
      characterString[4] = "";
      characterString[5] = "";
      characterString[6] = "";
      characterString[7] = "";
      characterString[8] = "";
      characterString[9] = "";
      characterString[10] = "";
      characterString[11] = "";
      characterString[12] = "";
      
  
}
void callUART(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
    Serial.println("--------------------------");
  }
   while (mySerial.available() > 0) {
    int inChar = mySerial.read();
    if (inChar == ':'){
       i++;
       point = inChar;
        }
    else if(isDigit(inChar)) {
      // convert the incoming byte to a char and add it to the string:
      inNumber[i] += (char)inChar;
    }
    else{
      characterString[i] += (char)inChar;  
    }
     if (inChar == '\n') {
      //temparature = inNumber[0];
      sprintf(temparature,"%d.%d",inNumber[0].toInt(),inNumber[1].toInt());
      sprintf(humidity,"%d.%d",inNumber[2].toInt(),inNumber[3].toInt());
      //humidity = inNumber[2];
      co2 = inNumber[4];
      VOCC = inNumber[5];
      //PM25 = inNumber[5]; 
      sprintf(PM25,"%d.%d",inNumber[6].toInt(),inNumber[7].toInt());
      for (j=0;j<15;j++){
        if (characterString[0][j]=='T' && characterString[0][j+1]=='E'){
          //Serial.println("Gia tri nhiet do la");
            //Serial.print(inNumber[0].toInt());
           // Serial.print(".");
           // Serial.println(inNumber[1].toInt());
           //client.publish("esp/response/collect/temperature",inNumber[0].c_str(),true);
          }
      }
        if(characterString[2] =="HU") {
        // Serial.println("Gia tri do am la");
        // Serial.println(inNumber[2].toInt());
         //Serial.println
          //client.publish("esp/response/collect/humidity",inNumber[1].c_str(),true);
       }
       if(characterString[4] =="CO"){
         // Serial.println("Gia tri Co2 la");
         // Serial.println(inNumber[4].toInt());
         //client.publish("esp/response/collect/co2",inNumber[2].c_str(),true);
       }
       for (j=0;j<15;j++){
        if (characterString[5][j]=='V' && characterString[5][j+1]=='O'){
        // Serial.println("Gia tri VOCC la"); 
       // Serial.println(inNumber[5].toInt());
       // client.publish("esp/response/collect/vooc",inNumber[3].c_str(),true);
        }
       }
       for (j=0;j<15;j++){
         if (characterString[6][j]=='P' && characterString[6][j+1]=='M'){
           // Serial.println("Gia tri PM25 la"); 
           // Serial.println(PM25);
          //  Serial.println(inNumber[4].toInt());
 
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
       for (j=0;j<15;j++){
         if (characterString[8][j]=='P' && characterString[8][j+1]=='O'){
         // Serial.println("Gia tri POWER la"); 
          // Serial.println(inNumber[8].toInt());
            POWER = inNumber[8].toInt();
            if (POWER == 1){   
                  //sprintf(power,"ON");
                  strcpy((char *) power, (char *) "ON");
            }
            else if(POWER == 0){
                 //sprintf(power,"OFF");
                 strcpy((char *) power, (char *) "OFF");
            }
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
       for (j=0;j<15;j++){
         if (characterString[9][j]=='S' && characterString[9][j+1]=='P'){
          // Serial.println("Gia tri SPEED la"); 
          // Serial.println(inNumber[9].toInt());
            
            Gspeed = inNumber[9].toInt();
             if (Gspeed == 0){

              strcpy((char *) Speed, (char *) "0");
             }
             else if (Gspeed == 1){
              
              strcpy((char *) Speed, (char *) "1");
             }
             else if(Gspeed == 2){
               
               strcpy((char *) Speed, (char *) "2");
             }
             else if(Gspeed == 3){
             
              strcpy((char *) Speed, (char *) "3");
             }
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
       for (j=0;j<15;j++){
         if (characterString[10][j]=='N' && characterString[10][j+1]=='I'&&characterString[10][j+2]=='G'){
           // Serial.println("NIGH MODE "); 
           // Serial.println(inNumber[10].toInt());
            
            NIGHT = inNumber[10].toInt();
            if (NIGHT == 1)
              //sprintf(night_mode,"on");
              strcpy((char *) night_mode, (char *) "on");
             else if(NIGHT == 0)
               //sprintf(night_mode,"off");
               strcpy((char *) night_mode, (char *) "off");
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
        for (j=0;j<15;j++){
         if (characterString[11][j]=='F' && characterString[11][j+1]=='I'&&characterString[11][j+2]=='L'){
          // Serial.println("FILTER MODE "); 
         //  Serial.println(inNumber[11].toInt());
            
            FILTER = inNumber[11].toInt();
             if (FILTER == 1)
            // sprintf(filter_mode,"fresh_air");
             strcpy((char *) filter_mode, (char *) "fresh");
             
            else if(FILTER == 0)
              //sprintf(filter_mode,"indoor");
              strcpy((char *) filter_mode, (char *) "indoor");
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
      for (j=0;j<15;j++){
         if (characterString[12][j]=='C' && characterString[12][j+1]=='T'&&characterString[12][j+2]=='R'){
             //Serial.println("control "); 
             //Serial.println(inNumber[12].toInt());
            
            CTRL = inNumber[12].toInt();
             if (CTRL == 1)
             //sprintf(control_mode,"manual");
             strcpy((char *) control_mode, (char *) "manual");
              else if(CTRL == 0)
              //sprintf(control_mode,"auto");
               strcpy((char *) control_mode, (char *) "auto");
            //client.publish("esp/response/collect/PM25",inNumber[4].c_str(),true);
        }
      }
      clearall();
    }
}
}

void loop() {
  callUART();
  client.loop();
  
}
