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
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

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
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");
      http.addHeader("X-DreamFactory-API-Key", "36fda24fe5588fa4285ac6c6c2fdfbdb6b6bc9834699774c9bf777f706d05a88");
      http.addHeader("X-DreamFactory-Session-Token", "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJzdWIiOiI1ZmVkYzk5YjJmZmI4M2QyODY0MTI4OTVjMDI4YWM1YiIsImlzcyI6Imh0dHBzOi8vYmUuZXJnb3Byb3AuY29tL2FwaS92Mi9zeXN0ZW0vYWRtaW4vc2Vzc2lvbiIsImlhdCI6MTU5MTU2Njg3NywiZXhwIjoxNTkxNTcwNDc3LCJuYmYiOjE1OTE1NjY4NzcsImp0aSI6IlJNd1FXdmdXTkF1VWhsNFoiLCJ1c2VyX2lkIjoxLCJmb3JldmVyIjpmYWxzZX0.BBBKTHx8s51eqDejBQJTOUkQUwxX4fITjDB9BkETFBQ");
      int httpResponseCode = http.POST("{\"resource\":[{\"id\":0},{\"temp\":22}],\"ids\":[0],\"filter\":\"string\",\"params\":[\"string\"]}");
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
