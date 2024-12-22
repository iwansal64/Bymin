import paho.mqtt.client as client
import paho.mqtt.reasoncodes as reason_codes
import paho.mqtt.properties as properties

import json
import models

import mqtt_manager
import pdf_manager
import print_manager

MQTT_SERVER_TOPIC_SELECT = 0

def on_connect(client: client.Client, userdata: any, flags: client.ConnectFlags, reason_code: reason_codes.ReasonCode, properties: properties.Properties):
    '''When Client Connected to MQTT Server'''
    print("CONNECTED")
    client.subscribe(mqtt_manager.MQTT_SERVER_TOPICS[MQTT_SERVER_TOPIC_SELECT])
    
def on_message(client: client.Client, userdata: any, msg: client.MQTTMessage):
    '''When Client Get Message from MQTT Server'''
    print("MESSAGE RECIEVED!")
    if msg.topic == mqtt_manager.MQTT_SERVER_TOPICS[MQTT_SERVER_TOPIC_SELECT]:
        pdf_manager.create_pdf_from_health_data(models.HealthData(json.loads(msg.payload)))

print("CONNECTING...")
mqtt_manager.connect(on_connect=on_connect, on_message=on_message)