#include <DHT.h>
#define DHTType DHT22

const int led_pin = 5;         
const int button_pin = 3;      
const int tmp_sensor_pin = A0;   
const int dht_sensor_pin = 2;

bool prev_button_state = LOW;  
bool led_state = LOW;         
DHT dht = DHT(dht_sensor_pin, DHTType);

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  bool button_state = digitalRead(button_pin);
  if (button_state == LOW && prev_button_state == HIGH) {
    led_state = !led_state;  
    if (led_state) {   // look into this block more
      digitalWrite(led_pin, HIGH);
      Serial.println("Data collection started");
    } 
    else {
        digitalWrite(led_pin, LOW);
        Serial.println("Data collection stopped");
    }
  }

  prev_button_state = button_state;

  if (led_state) {
    read_write_temp();
    read_write_temp_humidity();
  }
  
  delay(2000); // 5s delay between readings
}


// Read Temp Fcn from TMP36 Sensor
void read_write_temp() {
  int sensor_val = analogRead(tmp_sensor_pin);
  float volt = sensor_val * (5.0 / 1023.0); // analog value to voltage
  float temp = (volt - 0.5) * 100.0; // voltage to temperature in C
  
  // Print the temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
}

// Read Temp & Humidity Fcn from DHT Sensor
void read_write_temp_humidity() {
  float humi = dht.readHumidity();
  float temp2 = dht.readTemperature();
    if (isnan(humi) || isnan(temp2)) {
      Serial.println("Failed to read from the DHT sensor");
      Serial.print("Humidity: ");
      return;
    }
  
  Serial.print("Humidity: ");
  Serial.print(humi);
  //Print out the Temperature
  Serial.print("% || Temperature: ");
  Serial.print(temp2);
  Serial.print("°C ");

    //Print new line
  Serial.println();

}
