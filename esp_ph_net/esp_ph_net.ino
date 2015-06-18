
#include <ESP8266WiFi.h>
#include <Wire.h>  
#define DEBUG
#define DEBUG_PRINTER Serial
#ifdef DEBUG
#define DEBUG_PRINT(...) { DEBUG_PRINTER.print(__VA_ARGS__); }
#define DEBUG_PRINTLN(...) { DEBUG_PRINTER.println(__VA_ARGS__); }
#else
#define DEBUG_PRINT(...) {}
#define DEBUG_PRINTLN(...) {}
#endif
#define address2 99               
char computerdata[20];           
byte received_from_computer=1;   
byte serial_event=0;             
byte code=0;                    
char ph_data[20];                
byte in_char=0;                
byte i=0;                        
int time=1400;                  
float ph_float;  
float ph2_float;  
const char* ssid     = "ER9X";
const char* password = "asdfasdf";
float temp;
void connectWifi();
void reconnectWifiIfLinkDown();
void uploadThingsSpeak(float t);
 
void setup() {
    Serial.begin(115200);
    Wire.begin();  
    delay(10);
    connectWifi();
    delay(100);                //waiting sensor start
}
 
void loop() {
    tong();
    ph2_float=0;
    uploadThingsSpeak(temp);
    delay(5000);
    reconnectWifiIfLinkDown();
    
}
 
void reconnectWifiIfLinkDown() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("WIFI DISCONNECTED");
        connectWifi();
    }
}
 
void connectWifi() {
    DEBUG_PRINTLN();
    DEBUG_PRINTLN();
    DEBUG_PRINT("Connecting to ");
    DEBUG_PRINTLN(ssid);
 
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        DEBUG_PRINT(".");
    }
 
    DEBUG_PRINTLN("");
    DEBUG_PRINTLN("WiFi connected");
    DEBUG_PRINTLN("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP());
}
void uploadThingsSpeak(float t) {
    static const char* host = "api.thingspeak.com";
    static const char* apiKey = "O9RCIHUIEPSY1NU9";
 
    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        DEBUG_PRINTLN("connection failed");
        return;
    }
 
    // We now create a URI for the request
    String url = "/update/";
    //  url += streamId;
    //-----------------------------------------------
    url += "?key=";
    url += apiKey;
    url += "&field1=";
    url += t;
    //-----------------------------------------------
//    url += "?key=";
//    url += apiKey;
//    url += "&field3=";
//    url += t;
//    url += "&field4=";
//    url += h;    
    //-----------------------------------------------
    
    
    
    DEBUG_PRINT("Requesting URL: ");
    DEBUG_PRINTLN(url);
 
    // This will send the request to the server
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
}
void tong(){                   
  
    for(int x=0;x<10;x++){
    Wire.beginTransmission(address2); 
    Wire.write(114);       
    Wire.endTransmission();        
    
    
    delay(1400);                   
    
    Wire.requestFrom(address2,20,1); 
    code=Wire.read();               
    
    switch (code){                 
      case 1:                      
       // Serial.println("Success"); 
      break;                       
    
     case 2:                        
       Serial.println("Failed");   
     break;                        
    
     case 254:                     
       Serial.println("Pending");   
     break;                        
     
     case 255:                     
       Serial.println("No Data");  
     break;                        
    }
  while(Wire.available()){          
   in_char = Wire.read();           
   ph_data[i]= in_char;             
   i+=1;                            
    if(in_char==0){                
         i=0  ;                  
        Wire.endTransmission();    
        break;                     
    }
  }
  ph_float=atof(ph_data); 
  ph2_float=ph2_float+ph_float;
  // Serial.println(ph_float); 
  }
   ph2_float=ph2_float/10;
  if(ph2_float>9)
  ph2_float=ph2_float*1.058;
  if(ph2_float<9)
  ph2_float=ph2_float*1.023391813f;
  temp=ph2_float;  
  Serial.println(ph_data); 
  Serial.println(ph2_float); 
  }



 

