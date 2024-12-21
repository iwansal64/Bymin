#include <Arduino.h>
#include <Wire.h>
#include <MAX30105.h>
#include <particlesensor_manager.h>
#include <mqtt_manager.h>
#include <tempsensor_manager.h>
#include <dfplayer_manager.h>

MAX30105 particleSensor;

ParticleSensorManager particleSensorManager(&particleSensor);
MqttManager mqttManager;
DFPlayerManager dfplayerManager(30, RX, TX);

bool cur;

void setup()
{
    //? Setup Serial
    Serial.begin(115200);
    Serial.println("START");

    //? Setup Particle Sensor Manager
    particleSensorManager.setup();
    Wire.begin(21, 22);

    //? Setup MQTT Manager
    mqttManager.setup();
    mqttManager.connect();

    //? Setup DFPlayer
    // dfplayerManager.setup();
}

void loop()
{
    //? Updating particle sensor data
    particleSensorManager.update_particlesensor();

    //? Check if the particle sensor is ready or not
    if (particleSensorManager.ready_hr_sensor())
    {
        float average_hr = particleSensorManager.show_average_hr();

        if (average_hr > 0)
        {
            float celcius = particleSensorManager.show_temperature();
            if (celcius > 0 && celcius < 500)
            {
                Serial.println("Avg HR : " + String(average_hr) + "BPM. Temperature : " + String(celcius) + " °C");
                mqttManager.send_data(average_hr, celcius);
            }
        }
    }

    if (particleSensorManager.exist_finger() != cur)
    {
        cur = !cur;
        Serial.println(cur ? "FINGER" : "FINGTOD");
    }

    mqttManager.ensure_connection();
}
