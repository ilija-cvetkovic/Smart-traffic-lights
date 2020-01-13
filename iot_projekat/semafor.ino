#include <ESP8266WiFi.h>    //bliblioteka za esp8266 
#include <FirebaseArduino.h>  //biblioteka za firebase bazu podatka
#include <SoftwareSerial.h> //biblioteka koja nam dozvoljava serijsku komunikaciju 



//SoftwareSerial s(D6,D8); //definisemo serial pinove na esp8266 nodemcu
 
//definisemo vezu sa Firebaase
#define FIREBASE_HOST "YOUR FIREBASE HOST"  //naša baza podatka 
#define FIREBASE_AUTH "YOUR FIREBASE AUTH" //kljuc za autentifikaciju baze 
  
//Definisanje ssid-a i password-a mreze
#define WIFI_SSID "YOUR WIFI NAME" //ime wifi mreže
#define WIFI_PASSWORD "YOUR WIFI PASSWORD" //šifra

//senzor lm35 na analogni pin   
int outputpin= A0;

//ultrasonic senzor 1 (odredjuje o kom vozilu se radi)
const int trigPin = 4;  //D2
const int echoPin = 5;  //D1

//ultrasonic senzor (meri da li je vozilo pristuno)
const int trig_pin2=4; //D2
const int echo_pin2=14; //D5

long duration; //vreme ta koje će da se vrati zvuk za prvi senzor
int distance; //udaljenost prvog senzora od vozila
long vreme_senzor; //vreme ta koje će da se vrati zvuk za drugi senzor
int udaljenost_senzor; //udaljenost drugog senzora od vozila
//pomoocne promenjive
int pom=0; 
int a=0, b=0, k=0;
int ukupno=0; //ukupan broj vozila

void setup() {
//    s.begin(9600);  
    Serial.begin(9600); 
    //ultrasonic
    pinMode(trigPin, OUTPUT); 
    pinMode(echoPin, INPUT); 
    
    //konekcija sa mrezom
    WiFi.disconnect();               
    WiFi.begin(WIFI_SSID,WIFI_PASSWORD);  
    Serial.print("Connecting to ");
    Serial.print(WIFI_SSID);
    Serial.print(" ");
    while((!(WiFi.status() == WL_CONNECTED))){
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("Connected to ");
    Serial.println(WIFI_SSID);
    Serial.print("IP Address is : ");
    Serial.println(WiFi.localIP());    

    //konekcija sa Firebase bazu podataka (realtime database)
    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);     
}

void loop()       
{ 
    //LM35 
    int analogValue = analogRead(outputpin);
    if(!analogValue){
      Serial.println("Senzor nije dobro procitao vrednost");
      return;
    }
    
    float millivolts = (analogValue/1024.0) * 3300; //racunanje trenutne temeprature
    float cel = millivolts/10;
    Serial.print("Temp: ");
    Serial.println(cel);

    Firebase.setFloat("temp", cel);  //slanje trenutne temperature senzora u Firebase bazu
    if (Firebase.failed()) {
        Serial.print("setting /temp failed:");
        Serial.println(Firebase.error());  
        return;
    }
  
    //Podesavanje ultrasonic senzora  1
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.034/2; //racunanje udaljenosti od vozila
    Serial.print("Distance: ");
    Serial.println(distance); 

    //Podesavanje ultrasonic senzora senzor 2
    digitalWrite(trig_pin2, LOW);
    delayMicroseconds(2);
    digitalWrite(trig_pin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig_pin2, LOW);
    vreme_senzor = pulseIn(echo_pin2, HIGH);
    udaljenost_senzor = vreme_senzor*0.034/2; //racunanje udaljenosti od vozila
    Serial.println("Udaljenost senzora 2: ");
    Serial.println(udaljenost_senzor);
    
    //upisujemo ukupnu prolaznost povizla u Firebase bazu podataka
    Firebase.setInt("ukupno", ukupno);
   
    //Odredjivanje o kom vozilu se radi
    if(udaljenost_senzor < 12 && pom==0){
        //odredjujemo o kom vozilu se radi
        if(distance < 11 && distance > 8){
                Firebase.setInt("automobil", ++a); //Povecavamo vrednost automobil u Firebase bazu za 1 
                ukupno++; //ukupan broj vozila
                pom = 1; 
          }
          else if(distance <= 8 && distance > 6){
                Firebase.setInt("autobus", ++b); //Povecavamo vrednost autobus u Firebase bazu za 1
                ukupno++;
                pom = 1; 
          }
          else if(distance <= 6 && distance >= 2){
                Firebase.setInt("kamion", ++k); //Povecavamo vrednost kamion u Firebase bazu za 1
                ukupno++;
                pom = 1; 
          }
      }
     if(udaljenost_senzor>12){
         pom=0; 
     } 
}
