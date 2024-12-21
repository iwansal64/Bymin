#pragma once
#include <Adafruit_MLX90614.h>
#include <display_manager.h>

class TempSensorManager
{
private:
    Adafruit_MLX90614 *temperatureSensor;
    TempDisplayManager tempDisplayManager;

public:
    TempSensorManager(Adafruit_MLX90614 *temperatureSensor);
    void setup();
    double get_temp_celcius();
    double show_temp_celcius();
};

TempSensorManager::TempSensorManager(Adafruit_MLX90614 *temperatureSensor) : tempDisplayManager()
{
    this->temperatureSensor = temperatureSensor;
}

void TempSensorManager::setup()
{
    if (!this->temperatureSensor->begin())
    {
        // Serial.println("Error connecting to MLX sensor. Check wiring.");
        while (1)
            ;
    };
}

double TempSensorManager::get_temp_celcius()
{
    return round(this->temperatureSensor->readObjectTempC() * 10) / 10;
    // Serial.print("Temperature: ");
    // Serial.print(round(celcius));
    // Serial.println("°C");
}

double TempSensorManager::show_temp_celcius()
{
    double result = this->get_temp_celcius();
    this->tempDisplayManager.show_temperature_state(result);
    return result;
    // Serial.print("Temperature: ");
    // Serial.print(round(celcius));
    // Serial.println("°C");
}