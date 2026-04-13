#include <Arduino.h>
#include "SensorManager.h"
#include "DisplayManager.h"
#include "config.h"

SensorManager sensor;
DisplayManager display;

float lastT=0, lastH=0;

Status getStatus(float t, float h, SensorState st){
    if (st == SENSOR_SIM) return FAULT;
    if (t > TEMP_MAX) return HOT;
    if (t < TEMP_MIN) return COLD;
    if (h > HUM_MAX) return HUMID;
    if (h < HUM_MIN) return DRY;
    return OPTIMAL;
}

void setup(){
    Serial.begin(115200);
    delay(2000); // Give CDC time to connect
    Serial.println("Booting...");
    
    Serial.println("Init Sensor...");
    sensor.begin();
    
    Serial.println("Init Display...");
    display.begin();
    
    Serial.println("Setup Complete!");
}

void loop(){
    static unsigned long lastSensorRead = 0;

    // --- NON-BLOCKING SENSOR LOOP ---
    if (millis() - lastSensorRead > SENSOR_INTERVAL){
        lastSensorRead = millis();

        float t, h;
        Serial.println("[Core] Polling SHT40...");
        
        if(sensor.read(t, h)){
            bool trendT = t > lastT;
            bool trendH = h > lastH;

            Status s = getStatus(t, h, sensor.state());

            display.draw(t, h, s, trendT, trendH);

            lastT = t;
            lastH = h;

            Serial.printf("[Core] Temp: %.2fC | Humidity: %.1f%%\n", t, h);
        } else {
            Serial.println("[Warning] Sensor read failed. Awaiting recovery.");
        }
    }

    // --- FUTURE-PROOFING STUBS ---
    
    // TODO (Rule 14: OTA Updates)
    // To allow jar updates without opening:
    // 1. Detect a GPIO Button Press
    // 2. WiFi.begin("SSID", "PASS")
    // 3. ElegantOTA.begin(&server)
    // 4. Temporarily suspend sensor reads while upgrading.

    // TODO (Rule 11: Low Power)
    // ESP32-C3 drops to ~130uA in light sleep. 
    // WARNING: Light sleep will disconnect the built-in USB CDC (Serial Monitor)!
    // Only enable `esp_light_sleep_start()` when deploying to a battery pack 
    // and no longer requiring active serial debugging.

    delay(10); // Feed the watchdog and prevent task starvation
}