#include <SoftwareSerial.h>
#include <ArduinoJson.h>
SoftwareSerial s(0,1);


int LED_CRVENA = 2;
int LED_ZUTA= 3;
int LED_ZELENA = 4;
unsigned long v = 0;
unsigned long crv,zuta,zel;


void setup(){
      Serial.begin(9600);
      s.begin(9600);
      while (!Serial) continue;
      pinMode(LED_CRVENA, OUTPUT);
      pinMode(LED_ZUTA, OUTPUT);
      pinMode(LED_ZELENA, OUTPUT);
}


void loop(){
      StaticJsonBuffer<1000> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(s);
      if (root == JsonObject::invalid()) return;
      int ukupno = root["ukupno"];
      int i = root["i"];
      
      Serial.println(ukupno);
      Serial.println(i);
      
      if(i==1){
          if(ukupno < 200){
               v = 0;
           }
          else if(ukupno>=200 && ukupno<500) {
                v = v + 500;
          }
          else if(ukupno>=500 && ukupno<1000) {
                v = v + 1000;
          }
          else{
                v = v + 2000;
          }
          zuta = 4000 - v;
          crv = 11000 - v;
          zel = 6000 - v;
          digitalWrite(LED_ZELENA, LOW);
          digitalWrite(LED_ZUTA, HIGH);
          delay(zuta);
      
          digitalWrite(LED_ZUTA, LOW);
          digitalWrite(LED_CRVENA, HIGH);
          delay(crv);
          
          digitalWrite(LED_ZUTA, HIGH);
          delay(zuta);
      
          digitalWrite(LED_ZUTA, LOW);
          digitalWrite(LED_CRVENA, LOW);
          digitalWrite(LED_ZELENA, HIGH);
          delay(zel);
      }
      else{
        digitalWrite(LED_ZELENA, LOW);
        digitalWrite(LED_ZUTA, HIGH);
        delay(4000);
    
        digitalWrite(LED_ZUTA, LOW);
        digitalWrite(LED_CRVENA, HIGH);
        delay(11000);
        
        digitalWrite(LED_ZUTA, HIGH);
        delay(4000);
    
        digitalWrite(LED_ZUTA, LOW);
        digitalWrite(LED_CRVENA, LOW);
        digitalWrite(LED_ZELENA, HIGH);
        delay(6000);
      }
}
