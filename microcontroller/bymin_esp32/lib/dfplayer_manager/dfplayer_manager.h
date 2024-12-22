#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>
#include <display_manager.h>

class DFPlayerManager
{
private:
    uint8_t volume;
    DFRobotDFPlayerMini dfplayer;
    HardwareSerial *dfplayerSerial;

    DFPlayerDisplayManager dfplayer_display_manager;

public:
    DFPlayerManager(uint8_t default_volume, HardwareSerial *serial);
    void setup();
    void play_sound(uint8_t file_num);
};

DFPlayerManager::DFPlayerManager(uint8_t default_volume, HardwareSerial *serial) : dfplayerSerial(serial), dfplayer_display_manager()
{
    this->volume = default_volume;
}

void DFPlayerManager::setup()
{
    Serial.println("BEGIN SERIAL2");
    this->dfplayerSerial->begin(9600, SERIAL_8N1);
    Serial.println("END SERIAL2");
    if (!this->dfplayer.begin(*this->dfplayerSerial, true, true))
    {
        Serial.println("DFPlayer Mini initialization failed!");
        this->dfplayer_display_manager.show_setup_state(false);
        while (true)
            ;
    }
    Serial.println("DFPlayer Mini initialization success!");
    this->dfplayer_display_manager.show_setup_state(true);
}

void DFPlayerManager::play_sound(uint8_t file_num)
{
    this->dfplayer.volume(this->volume);
    this->dfplayer.play(file_num);
    this->dfplayer_display_manager.show_play_sound(file_num);
}