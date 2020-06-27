#include <ArduinoJson.h>


#include <Adafruit_Sensor.h>

#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h> // Library for I2C communication
#include <LiquidCrystal_I2C.h> // Library for LCD
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 20, 4); // Change to (0x27,16,2) for 16x2 LCD.
#define DHTPINGROW 12     // Digital pin connected to the DHT sensor 
#define DHTPINBLM 13     // Digital pin connected to the DHT sensor 
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
#define DHTTYPEBLM    DHT11     // DHT 22 (AM2302)

DHT dht(DHTPINGROW, DHTTYPE);
DHT dht1(DHTPINBLM, DHTTYPEBLM); 

const int OUTFILTER = 3; 
const int INPUTAIR = 4; 
const int INPUTAIR2 = 5; 
const int WATERPUMP = 6; 


uint32_t delayMS;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  // Initialize device.
  dht.begin();
  dht1.begin();
  sensor_t sensor;
  pinMode(OUTFILTER, OUTPUT);
  pinMode(INPUTAIR, OUTPUT);
  pinMode(INPUTAIR2, OUTPUT);
  pinMode(WATERPUMP, OUTPUT);
  digitalWrite(OUTFILTER, LOW);
  digitalWrite(INPUTAIR, LOW);
  digitalWrite(INPUTAIR2, LOW);
  digitalWrite(WATERPUMP, LOW);
  delayMS = sensor.min_delay / 8000;
}


    void checkLogicblm(int tempblm , int humidityblm){
         if(tempblm > 30 ){
        digitalWrite(INPUTAIR2, HIGH);
      }
          if(tempblm < 28 ){
        digitalWrite(INPUTAIR2, LOW);
      }
    }

        void checkLogicgrow(int temp , int humidity){
         if(temp > 28 ){
        digitalWrite(INPUTAIR, HIGH);

      }
          if(temp < 23 ){
        digitalWrite(INPUTAIR, LOW);
      }
    }

String fixonoff (bool val) {
  if(val){
    return "ON";
    }
    if(!val){
      return "OFF";
      }  
  }

    

  int printerHum(int hum, int humblm) {
     lcd.setCursor(13, 0); // Set the cursor on the first column and first row.
     lcd.print("H"); // Print the string "Hello World!"
     lcd.setCursor(14, 0); // Set the cursor on the first column and first row.
     lcd.print(hum); // Print the string "Hello World!"
     lcd.setCursor(19, 0); // Set the cursor on the first column and first row.
     lcd.print("%"); // Print the string "Hello World!"
     lcd.setCursor(13, 1); // Set the cursor on the first column and first row.
     lcd.print("H"); // Print the string "Hello World!"
     lcd.setCursor(14, 1); // Set the cursor on the first column and first row.
     lcd.print(humblm); // Print the string "Hello World!"
     lcd.setCursor(18, 1); // Set the cursor on the first column and first row.
     lcd.print("%"); // Print the string "Hello World!"
    
    }

    int printerTemp(int temp, int tempblm){
       String flt = fixonoff(digitalRead(3));
              String air1 = fixonoff(digitalRead(4));
                     String air2 = fixonoff(digitalRead(5));
      String Pump = fixonoff(digitalRead(6));
 
      lcd.setCursor(0, 0); // Set the cursor on the first column and first row.
      lcd.print("Grow "); // Print the string "Hello World!"
      lcd.setCursor(6, 0); // Set the cursor on the first column and first row.
      lcd.print("C"); // Print the string "Hello World!"
      lcd.setCursor(7, 0); // Set the cursor on the first column and first row.
      lcd.print(temp); // Print the string "Hello World!
      lcd.setCursor(0, 1); // Set the cursor on the first column and first row.
      lcd.print("Bloom"); // Print the string "Hello World!"
      lcd.setCursor(6, 1); // Set the cursor on the first column and first row.
      lcd.print("C"); // Print the string "Hello World!"
      lcd.setCursor(7, 1); // Set the cursor on the first column and first row.
      lcd.print(tempblm); // Print the string "Hello World!"
      lcd.setCursor(0, 2); // Set the cursor on the first column and first row.
      lcd.print("OUT="); // Print the string "Hello World!"
      lcd.setCursor(6, 2); // Set the cursor on the first column and first row.
      lcd.print(flt); // Print the string "Hello World!"
      lcd.setCursor(11, 2); // Set the cursor on the first column and first row.
      lcd.print("AIR="); // Print the string "Hello World!"
      lcd.setCursor(17, 2); // Set the cursor on the first column and first row.
      lcd.print(air1); // Print the string "Hello World!"
      lcd.setCursor(0, 3); // Set the cursor on the first column and first row.
      lcd.print("Pump="); // Print the string "Hello World!"
      lcd.setCursor(7, 3); // Set the cursor on the first column and first row.
      lcd.print(Pump); // Print the string "Hello World!"
     }

    void SendSensorData(int temp , int humidity, int tempblm , int humidityblm)
    {
    
    DynamicJsonBuffer jBuffer;
    JsonObject& root= jBuffer.createObject();
    root ["TempGrow"]=(temp);
    root ["HumGrow"]=(humidity);
    root ["TempBloom"]=(tempblm);
    root ["HumBloom"]=(humidityblm);
    root ["Meta"]=(humidityblm);
    root.prettyPrintTo(Serial);
    Serial.println();
    }

        void SendStatus()
    {
       String flt = fixonoff(digitalRead(3));
              String air1 = fixonoff(digitalRead(4));
                     String air2 = fixonoff(digitalRead(5));
      String Pump = fixonoff(digitalRead(6));


          DynamicJsonBuffer jBuffer;
    JsonObject& root= jBuffer.createObject();
    root ["FILTER"]=(flt);
    root ["AIR1"]=(air1);
    root ["AIR2"]=(air2);
    root ["PUMP"]=(Pump);
    root.prettyPrintTo(Serial);
    Serial.println();
 
    }


    
void loop() {

  delay(1000);
  int val = digitalRead(11);
 
//  if(val ){
//      digitalWrite(WATERPUMP, HIGH);
//    
//    }
//    if(!val  ){
//      digitalWrite(WATERPUMP, LOW);
//    
//    }
  
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h11 = dht.readHumidity();
  float t11 = dht.readTemperature();
  int sensorValue = analogRead(A0);

    // SendSensorData(t11, h11 ,t1, h1);
 
 while(Serial.available() > 0 ){
    String str = Serial.readString();
    str.trim();
       
    if(str == "getTemps"){
      SendSensorData(t11, h11 ,t1, h1);
    }

        if(str == "getStatus"){
      SendStatus();
    }
     if(str == "cPump"){
       digitalWrite(WATERPUMP, LOW);
    }
     if(str == "oPump"){
    digitalWrite(WATERPUMP, HIGH);
    }

    Serial.flush();
  }
 
  

  printerTemp(t11,t1);
  printerHum(h11,h1);
  checkLogicblm(t11,h11);
  checkLogicgrow(t1,h1);



}
