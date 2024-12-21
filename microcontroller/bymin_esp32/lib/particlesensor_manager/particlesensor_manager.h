#pragma once
#include <Wire.h>
#include <MAX30105.h>
#include <hrsensor_manager.h>
#include <display_manager.h>

class ParticleSensorManager
{
private:
    MAX30105 *particleSensor;
    HRSensorManager hrsensor_manager;
    HRDisplayManager hrdisplay_manager;

    float current_hr = 0;
    bool current_finger_state = false;

public:
    ParticleSensorManager(MAX30105 *particleSensor);
    void update_particlesensor();
    bool exist_finger();

    float get_average_hr();
    bool ready_hr_sensor();

    float show_average_hr();
    void show_finger_state();

    void setup();
};

ParticleSensorManager::ParticleSensorManager(MAX30105 *particleSensor) : hrsensor_manager(particleSensor), hrdisplay_manager()
{
    this->particleSensor = particleSensor;
}

void ParticleSensorManager::setup()
{

    // Serial.println("SETUP");
    if (!this->particleSensor->begin(Wire, I2C_SPEED_FAST))
    {
        Serial.println("ERROR WHEN BEGINNING PARTICLE SENSOR");
        while (1)
            ;
    }
    Serial.println("CONNECTED TO PARTICLE SENSOR SUCCESSFULLY!");

    byte ledBrightness = 60; // Options: 0=Off to 255=50mA
    byte sampleAverage = 4;  // Options: 1, 2, 4, 8, 16, 32
    byte ledMode = 3;        // Options: 1 = Red only, 2 = Red + IR, 3 = Red + IR + Green
    byte sampleRate = 100;   // Options: 50, 100, 200, 400, 800, 1000, 1600, 3200
    int pulseWidth = 411;    // Options: 69, 118, 215, 411
    int adcRange = 4096;     // Options: 2048, 4096, 8192, 16384

    this->particleSensor->setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

    this->hrsensor_manager.setup();
}

void ParticleSensorManager::update_particlesensor()
{
    this->hrsensor_manager.update_hr();
}

float ParticleSensorManager::get_average_hr()
{
    return this->hrsensor_manager.get_average_hr();
}

bool ParticleSensorManager::exist_finger()
{
    return this->hrsensor_manager.exist_finger();
}

bool ParticleSensorManager::ready_hr_sensor()
{
    return this->hrsensor_manager.ready_to_read();
}

float ParticleSensorManager::show_average_hr()
{
    if (this->ready_hr_sensor())
    {
        //? Get average heartrate
        float hr = this->get_average_hr() + 20;
        //? If the current hr is equals to the new hr don't waste time to change anything
        if (current_hr != hr)
        {
            // Serial.print("HR : ");
            // Serial.print(hr);
            // Serial.println(" bpm.");
            this->hrdisplay_manager.show_hr_state(hr);
            current_hr = hr;
            return hr;
            // ex output: {"Heart Rate":"90","Temperature":"30°C"}
        }
    }
    return 0;
}

void ParticleSensorManager::show_finger_state()
{
    //? If a finger present
    bool exist_finger = this->exist_finger();
    //? If current finger state is not same as the new state
    if (exist_finger != current_finger_state)
    {
        // Serial.println(current_finger_state ? "Fingerprint Exited" : "Fingerprint Detected");
        this->hrdisplay_manager.show_fingerprint_state(exist_finger);
        this->current_finger_state = exist_finger;
    }
}