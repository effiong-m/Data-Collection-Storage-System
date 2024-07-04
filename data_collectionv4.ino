#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include <WiFiNINA.h>
#include <DHT.h>

#define DHTPIN 2       
#define DHTTYPE DHT22  
#define TMP36PIN A0    
#define BUTTON_PIN 3   
#define LED_PIN 5      

const char ssid[] = "#####";
const char pass[] = "*****";

DHT dht(DHTPIN, DHTTYPE);

float dht22_temperature;
float dht22_humidity;
float tmp36_temperature;

bool collectingData = false;  // Flag to check if data collection is active

// Arduino Cloud Setup
WiFiConnectionHandler ArduinoIoTPreferredConnection(ssid, pass);

void initProperties() {
  ArduinoCloud.addProperty(dht22_temperature, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(dht22_humidity, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(tmp36_temperature, READ, ON_CHANGE, NULL);
}

void setup() {
  Serial.begin(9600);
  delay(1500);

  pinMode(BUTTON_PIN, INPUT);  
  pinMode(LED_PIN, OUTPUT);    

  initProperties();

  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  dht.begin();
}

void loop() {
  ArduinoCloud.update();

  if (digitalRead(BUTTON_PIN) == HIGH) {  
    delay(50);                           
    while (digitalRead(BUTTON_PIN) == HIGH)
      ;  

    collectingData = !collectingData;  // Toggle data collection state

    if (collectingData) {
      digitalWrite(LED_PIN, HIGH);  
    } else {
      digitalWrite(LED_PIN, LOW);  
    }

  
  }

  if (collectingData) {
    dht22_temperature = dht.readTemperature();
    dht22_humidity = dht.readHumidity();
    tmp36_temperature = (analogRead(TMP36PIN) * (5.0 / 1023.0) - 0.5) * 100.0;

    Serial.print("DHT22 Temperature: ");
    Serial.print(dht22_temperature);
    Serial.println(" °C");

    Serial.print("DHT22 Humidity: ");
    Serial.print(dht22_humidity);
    Serial.println(" %");

    Serial.print("TMP36 Temperature: ");
    Serial.print(tmp36_temperature);
    Serial.println(" °C");

  
  }
}
