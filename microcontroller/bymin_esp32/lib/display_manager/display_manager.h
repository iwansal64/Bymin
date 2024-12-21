#pragma once
#include <TFT_eSPI.h>
#define NO_DISPLAY

class DisplayManager : public TFT_eSPI
{
private:
    uint16_t default_bg = TFT_BLACK;
    uint16_t default_fg = TFT_WHITE;

public:
    DisplayManager(uint16_t default_bg = TFT_BLACK, uint16_t default_fg = TFT_WHITE);
    void setup();
    void clear();
    void put_text(String content, uint16_t x, uint16_t y, uint8_t color, uint8_t background_color, uint8_t size = 2);
    void put_text(String content, uint16_t x, uint16_t y, uint8_t size = 2);
    void clear_horizontal_line(uint8_t y);
    void replace_horizontal_line(String content, uint8_t offset_x, uint8_t y, uint8_t color, uint8_t background_color, uint8_t size = 2);
    void replace_horizontal_line(String content, uint8_t offset_x, uint8_t y, uint8_t size = 2);
};
DisplayManager::DisplayManager(uint16_t default_bg, uint16_t default_fg) : TFT_eSPI()
{
    this->default_bg = default_bg; //? Set default background color
    this->default_fg = default_fg; //? Set default text color
}

/* Setup */
void DisplayManager::setup()
{
#ifndef NO_DISPLAY
    this->begin();               //? Start initializing
    this->setRotation(1);        //? Setting rotation to landscape
    this->fillScreen(TFT_BLACK); //? Fill screen with black color
#endif
}

/* Clearing screen */
void DisplayManager::clear()
{
#ifndef NO_DISPLAY
    this->fillScreen(this->default_bg);
#endif
}

/* Putting text */
void DisplayManager::put_text(String content, uint16_t x, uint16_t y, uint8_t color, uint8_t background_color, uint8_t size)
{
#ifndef NO_DISPLAY
    this->setTextColor(color, background_color); //? Set text color to the default value
    this->setTextSize(size);                     //? Set text size
    this->drawString(content, x, y);             //? Draw the Text
#endif
}

/* Putting text with default color */
void DisplayManager::put_text(String content, uint16_t x, uint16_t y, uint8_t size)
{
#ifndef NO_DISPLAY
    this->setTextColor(this->default_fg, this->default_bg); //? Set text color to the default value
    this->setTextSize(size);                                //? Set text size
    this->drawString(content, x, y);                        //? Draw the text
#endif
}

void DisplayManager::clear_horizontal_line(uint8_t y)
{
    this->put_text("                                           ", 0, y);
}

void DisplayManager::replace_horizontal_line(String content, uint8_t offset_x, uint8_t y, uint8_t size)
{
    this->clear_horizontal_line(y);
    this->put_text(content, offset_x, y, size);
}

void DisplayManager::replace_horizontal_line(String content, uint8_t offset_x, uint8_t y, uint8_t color, uint8_t background_color, uint8_t size)
{
    this->clear_horizontal_line(y);
    this->put_text(content, offset_x, y, color, background_color, size);
}

/* =====================================> MQTT DISPLAY MANAGER <===================================== */

class MqttDisplayManager : public DisplayManager
{
private:
public:
    MqttDisplayManager(uint16_t default_bg = TFT_BLACK, uint16_t default_fg = TFT_WHITE);
    void show_connecting_state(uint8_t state, String ssid, uint8_t dot = 0);
    void show_ensureconnection_state(uint8_t state, String servername, uint32_t port, uint8_t message = 0);
    void show_sendingdata_state(uint8_t state, String message = "");
};

MqttDisplayManager::MqttDisplayManager(uint16_t default_bg, uint16_t default_fg) : DisplayManager(default_bg, default_fg)
{
}

/* Showing Connection State */
void MqttDisplayManager::show_connecting_state(uint8_t state, String ssid, uint8_t dot)
{
    this->clear(); // Clear current display

    if (state)
    {
        this->replace_horizontal_line("Connected to: " + ssid + "!", 10, 10, 2);
    }
    else
    {
        this->replace_horizontal_line("Connecting to: " + ssid, 10, 10, 2);

        //? Creating a string of dots for connecting info example: ". . ."
        String dots = "";
        for (int i = 0; i < dot; i++)
        {
            dots += ". ";
        }
        this->replace_horizontal_line(dots, 8, 12, 1);
    }
}

/* Showing Ensure Connection State */
void MqttDisplayManager::show_ensureconnection_state(uint8_t state, String servername, uint32_t port, uint8_t message)
{
    if (state == 2)
    {
        this->replace_horizontal_line("MQTT Client Connected!..", 10, 30, 1);
        this->clear_horizontal_line(32);
        this->clear_horizontal_line(34);
    }
    else if (state == 1)
    {
        this->replace_horizontal_line("MQTT Client is not connected..", 10, 30, 1);
        this->clear_horizontal_line(32);
        this->clear_horizontal_line(34);
        this->replace_horizontal_line("Reconnecting to the server...", 10, 32, 1);
        this->replace_horizontal_line("Connecting to: " + servername + ":" + String(port), 10, 34, 1);
    }
    else
    {
        this->replace_horizontal_line("Error when connecting to MQTT client..", 10, 30, 1);
        this->clear_horizontal_line(32);
        this->clear_horizontal_line(34);
        this->replace_horizontal_line("Error:" + String(message), 10, 32, 1);
        this->replace_horizontal_line("Try Reconnecting in 5s", 10, 34, 1);
    }
}

/* Showing Sending Data State */
void MqttDisplayManager::show_sendingdata_state(uint8_t state, String message)
{
    this->clear(); // Clear current display

    if (state == 2)
    {
        this->replace_horizontal_line("Data Recorded!", 10, 10, 1);
    }
    else if (state == 1)
    {
        this->replace_horizontal_line("Sending Data to Server..", 10, 10, 1);
    }
    else
    {
        this->replace_horizontal_line("Error Sending Data!", 10, 10, 1);
        this->replace_horizontal_line("Connection Error", 10, 12, 1);
    }
}

/* =====================================> HEART RATE DISPLAY MANAGER <===================================== */

class HRDisplayManager : public DisplayManager
{
private:
public:
    HRDisplayManager(uint16_t default_bg = TFT_BLACK, uint16_t default_fg = TFT_WHITE);
    void show_hr_state(float hr);
    void show_fingerprint_state(bool state);
};

HRDisplayManager::HRDisplayManager(uint16_t default_bg, uint16_t default_fg) : DisplayManager(default_bg, default_fg)
{
}

void HRDisplayManager::show_hr_state(float hr)
{
    this->replace_horizontal_line("HR : " + String(hr) + " BPM.       ", 10, 10, 2);
}

void HRDisplayManager::show_fingerprint_state(bool state)
{
    this->replace_horizontal_line(state ? "Fingerprint Detected!" : "Fingerprint Exited!", 1, 1, 1);
}

/* =====================================> TEMPERATURE DISPLAY MANAGER <===================================== */

class TempDisplayManager : public DisplayManager
{
private:
public:
    TempDisplayManager(uint16_t default_bg = TFT_BLACK, uint16_t default_fg = TFT_WHITE);
    void show_temperature_state(float celcius);
};

TempDisplayManager::TempDisplayManager(uint16_t default_bg, uint16_t default_fg) : DisplayManager(default_bg, default_fg)
{
}

void TempDisplayManager::show_temperature_state(float celcius)
{
    this->replace_horizontal_line("Temp : " + String(celcius, 2) + " °C      ", 10, 20, 2);
}

/* =====================================> DFPLAYER DISPLAY MANAGER <===================================== */

class DFPlayerDisplayManager : public DisplayManager
{
private:
public:
    DFPlayerDisplayManager(uint16_t default_bg = TFT_BLACK, uint16_t default_fg = TFT_WHITE);
    void show_play_sound(uint8_t file_num);
    void show_setup_state(bool state);
};

DFPlayerDisplayManager::DFPlayerDisplayManager(uint16_t default_bg, uint16_t default_fg) : DisplayManager(default_bg, default_fg)
{
}

void DFPlayerDisplayManager::show_play_sound(uint8_t file_num)
{
    this->replace_horizontal_line("Playing Sound-" + String(file_num), 10, 40, 1);
}

void DFPlayerDisplayManager::show_setup_state(bool state)
{
    this->replace_horizontal_line(state ? "DFPlayer Connected!" : "DFPlayer Not Connected..", 10, 40, 1);
}
