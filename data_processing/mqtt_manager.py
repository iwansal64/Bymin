import models, paho.mqtt.client as mqtt, paho
from typing import Callable

MQTT_SERVER_URI = "0.tcp.ap.ngrok.io"
MQTT_SERVER_PORT = 11380
MQTT_SERVER_TOPICS = ["/nodeee3/data1"]

def get_health_data() -> models.HealthData:
    '''Download Health Data from MQTT Server'''
    
    
def connect(on_connect: Callable[[paho.mqtt.client.Client, any, paho.mqtt.client.ConnectFlags, paho.mqtt.reasoncodes.ReasonCode, paho.mqtt.properties.Properties], None], on_message) -> None:
    '''Connect to MQTT Server'''
    mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
    mqttc.on_connect = on_connect
    mqttc.on_message = on_message

    try:
        mqttc.connect(MQTT_SERVER_URI, MQTT_SERVER_PORT)
    except ConnectionRefusedError:
        print("Can't connect to the server, It keeps refusing our request to connect!")
        exit(1)
        
    print("CONNECTED")
    mqttc.loop_forever()
    