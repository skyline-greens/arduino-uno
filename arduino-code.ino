#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 2
#define DHTTYPE DHT11

#define AIR_PUMP_PIN 3  
#define WATER_PUMP_PIN 4  
#define LIGHT_PIN 5  

#define LDR_PIN A0      
#define CO2_PIN A1      
#define SOIL_PIN A2      
#define WATER_PIN A3     

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();

    pinMode(AIR_PUMP_PIN, OUTPUT);
    pinMode(WATER_PUMP_PIN, OUTPUT);
    pinMode(LIGHT_PIN, OUTPUT);
}

void loop() {
    if (Serial.available()) {  
        String command = Serial.readStringUntil('\n');
        command.trim();
        
        if (command == "AIR_PUMP ON") {
            digitalWrite(AIR_PUMP_PIN, HIGH);
        } else if (command == "AIR_PUMP OFF") {
            digitalWrite(AIR_PUMP_PIN, LOW);
        } else if (command == "WATER_PUMP ON") {
            digitalWrite(WATER_PUMP_PIN, HIGH);
        } else if (command == "WATER_PUMP OFF") {
            digitalWrite(WATER_PUMP_PIN, LOW);
        } else if (command == "LIGHT ON") {
            digitalWrite(LIGHT_PIN, HIGH);
        } else if (command == "LIGHT OFF") {
            digitalWrite(LIGHT_PIN, LOW);
        }
    }

    // Read sensors every 2 seconds
    static unsigned long lastReadTime = 0;
    if (millis() - lastReadTime >= 2000) {
        lastReadTime = millis();
        
        float temp = dht.readTemperature();
        float hum = dht.readHumidity();
        int ldrValue = analogRead(LDR_PIN);
        int co2Value = analogRead(CO2_PIN);
        int soilValue = analogRead(SOIL_PIN);
        int waterValue = analogRead(WATER_PIN);

        if (!isnan(temp) && !isnan(hum)) {
            StaticJsonDocument<200> jsonData;
            jsonData["temp"] = temp;
            jsonData["hum"] = hum;
            jsonData["ldr"] = ldrValue;
            jsonData["co2"] = co2Value;
            jsonData["soil"] = soilValue;
            jsonData["water"] = waterValue;

            String output;
            serializeJson(jsonData, output);
            Serial.println(output);
        }
    }
}
