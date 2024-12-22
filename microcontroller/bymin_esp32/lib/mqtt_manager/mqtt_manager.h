#pragma once
#include <mqtt_client.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <display_manager.h>

#define TOPIC "/nodeee1/data1"
#define SSID "fh_a5a180"
#define PASSWORD "24G_is_the_best"
#define MQTT_SERVER_URI "0.tcp.ap.ngrok.io"
#define MQTT_SERVER_PORT 11380

#define MAX_RECURSIVE 10

class MqttManager
{
private:
    WiFiClient wifiClient;
    PubSubClient client;
    int retries = 0;
    bool send_topic(String payload, String topic = TOPIC);
    MqttDisplayManager mqtt_display_manager;

public:
    MqttManager();
    void setup();
    void connect();
    void ensure_connection();
    void reconnect();
    bool send_data(float heart_rate, float celcius);
};

MqttManager::MqttManager() : mqtt_display_manager()
{
}

void MqttManager::setup()
{
    client = PubSubClient(this->wifiClient);
    Serial.println("Setting up server");
    client.setServer(MQTT_SERVER_URI, MQTT_SERVER_PORT);

    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(SSID);

    this->mqtt_display_manager.show_connecting_state(false, SSID, 0);

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWORD);

    uint8_t dot = 1;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        this->mqtt_display_manager.show_connecting_state(false, SSID, dot);
        dot += 1;
        Serial.print(". ");
    }
    this->mqtt_display_manager.show_connecting_state(true, SSID);

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    randomSeed(micros());
}

void MqttManager::connect()
{
    this->client.setServer(MQTT_SERVER_URI, MQTT_SERVER_PORT);
}

void MqttManager::ensure_connection()
{
    if (!this->client.connected())
    {
        Serial.println("Client is not connected..");
        Serial.println("Reconnecting...");
        this->mqtt_display_manager.show_ensureconnection_state(1, MQTT_SERVER_URI, MQTT_SERVER_PORT);
        this->reconnect();
    }
}

void MqttManager::reconnect()
{
    Serial.print("Connecting to an MQTT Server : ");
    Serial.print(MQTT_SERVER_URI);
    Serial.print(":");
    Serial.println(MQTT_SERVER_PORT);
    // Loop until reconnected
    while (!this->client.connected())
    {
        // Create a random client ID
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);

        // Attempt to connect
        if (this->client.connect(clientId.c_str()))
        {
            this->mqtt_display_manager.show_ensureconnection_state(2, MQTT_SERVER_URI, MQTT_SERVER_PORT);
            Serial.println("Connected!");
        }
        else
        {
            this->mqtt_display_manager.show_ensureconnection_state(0, MQTT_SERVER_URI, MQTT_SERVER_PORT, this->client.state());
            // Serial.print("failed, state = ");
            // Serial.print(this->client.state());
            // Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

bool MqttManager::send_topic(String payload, String topic)
{
    if (payload == "")
    {
        // Serial.println("Payload Required!");
        return false;
    }
    this->mqtt_display_manager.show_sendingdata_state(1);
    bool return_value;
    if (client.connected())
    {
        return_value = client.publish(topic.c_str(), payload.c_str());
        this->mqtt_display_manager.show_sendingdata_state(2);
        return return_value;
    }
    else
    {
        this->retries++;
        if (this->retries >= MAX_RECURSIVE)
        {
            // Serial.println("Recursive looping detected");
            return false;
        }

        this->mqtt_display_manager.show_sendingdata_state(0);
        this->ensure_connection();
        return this->send_topic(payload, topic);
    }
}

bool MqttManager::send_data(float heart_rate, float celcius)
{
    return this->send_topic("{\"Heart Rate\":\"" + String(heart_rate) + " bpm\",\"Temperature\":\"" + celcius + "°C\"}");
}