#pragma once
#include <MAX30105.h>
#include <display_manager.h>

class TempSensorManager
{
private:
    MAX30105 *particleSensor;
    TempDisplayManager tempDisplayManager;

public:
    TempSensorManager(MAX30105 *particleSensor);
    void setup();
    float get_temp_celcius();
    float show_temp_celcius();
};

TempSensorManager::TempSensorManager(MAX30105 *particleSensor) : tempDisplayManager()
{
    this->particleSensor = particleSensor;
}

void TempSensorManager::setup()
{
}

float TempSensorManager::get_temp_celcius()
{
    float celcius = round(this->particleSensor->readTemperature() * 10) / 10;
    // Serial.print("Temperature: ");
    // Serial.print(round(celcius));
    // Serial.println("°C");
    return celcius;
}

float TempSensorManager::show_temp_celcius()
{
    float result = this->get_temp_celcius();
    this->tempDisplayManager.show_temperature_state(result);
    return result;
    // Serial.print("Temperature: ");
    // Serial.print(round(celcius));
    // Serial.println("°C");
}