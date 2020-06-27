#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

     
const char* ssid = "OPTILOOPATTIC";
const char* password = "!mijehof205!";

//Your Domain name with URL path or IP address with path
const char* serverName = "http://18.222.8.67:8888/api/v2/mongodb/_table/Sensors";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;

unsigned long timerDelay = 15000;


const char* tempgrow;



void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
  

}

void loop() {






 //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {

if(Serial.available()){
  

    
DynamicJsonBuffer jb;
JsonObject& root = jb.parseObject(Serial);

      const char* MetaData = root["Meta"];
      const char* TempG = root["TempGrow"];
      const char* TempB = root["TempBloom"];
      const char* HumG = root["HumGrow"];
      const char* HumB = root["HumBloom"];
      
String request = (String)"{\"resource\":[{\"id\":50,\"MetaData\":"+MetaData+(String)",\"TempGrow\":"+TempG+(String)",\"TempBloom\":"+TempB+(String)",\"HumidityG\":"+HumG+(String)",\"HumidityB\":"+HumB+(String)"}],\"ids\":[0],\"filter\":\"string\",\"params\":[\"string\"]}";
 
  Serial.print(request);


    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-DreamFactory-API-Key", "9fa0af1c2aef033e9683b9570c27c4f281808584c04d752bd089f92c14640660");
      int httpResponseCode = http.POST(request);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  }
       
}
