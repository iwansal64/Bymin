#pragma once
#include <MAX30105.h>
#include <heartRate.h>
#include <utilities.h>

class HRSensorManager
{
private:
    int duration;
    MAX30105 *particleSensor;
    long lastBeat = 0;
    float currentHR = 0;
    long currentIr;

    static const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
    byte listBPM[RATE_SIZE];         // Array of heart rates
    byte indexBPM;

public:
    HRSensorManager(MAX30105 *particleSensor);

    void set_duration(int new_duration);

    int update_hr();
    float get_average_hr();
    float get_current_hr();

    bool exist_finger();
    bool ready_to_read();

    void setup();
};

HRSensorManager::HRSensorManager(MAX30105 *particleSensor)
{
    this->particleSensor = particleSensor;
}

void HRSensorManager::setup()
{
}

void HRSensorManager::set_duration(int new_duration)
{
    this->duration = new_duration;
}

int HRSensorManager::update_hr()
{
    this->currentIr = this->particleSensor->getIR();
    // Serial.println(this->currentIr);
    if (checkForBeat(this->currentIr))
    {
        long currentTime = millis();
        long difference = currentTime - this->lastBeat;
        this->lastBeat = currentTime;

        this->currentHR = 60 / (difference / 1000.0);

        // Check if this BPM is valid
        if (this->currentHR < 255 && this->currentHR > 20)
        {
            this->listBPM[this->indexBPM] = this->currentHR;
            this->indexBPM %= this->RATE_SIZE;
            this->indexBPM += 1;
        }
    }

    return this->currentHR;
}

float HRSensorManager::get_average_hr()
{
    int averageBPM = 0;
    for (size_t index = 0; index < this->RATE_SIZE; index++)
    {
        averageBPM += this->listBPM[index];
    }
    averageBPM /= RATE_SIZE;

    return averageBPM;
}

float HRSensorManager::get_current_hr()
{
    return this->currentHR;
}

bool HRSensorManager::ready_to_read()
{
    for (size_t i = 0; i < this->RATE_SIZE; i++)
    {
        if (this->listBPM[i] == 0)
        {
            return false;
        }
    }
    return true;
}