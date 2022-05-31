import paho.mqtt.client as mqtt

MQTT_ADDRESS = '192.168.1.115'
MQTT_USER = 'CE490'
MQTT_PASSWORD = 'CE490'
MQTT_TOPIC = 'ce490connect'

def on_connect(client, userdata, flags, rc):
	"""The callback for when the client receives a CONNACK response from the server."""
	print('Connected with result code' + str(rc))
	client.subscribe(MQTT_TOPIC)

def on_message(client, userdata, msg):
	"""The callback for when a PUBLISH message is recieved from the server"""
	print(msg.topic + ''+str(msg.payload))
	with open("localdevices0.txt","a") as f1:
		if(len(str(msg.payload)) == 4):
			last_line = f1.write(str(msg.payload)[2:3] + "\n")
		elif(len(str(msg.payload)) == 5):
			last_line = f1.write(str(msg.payload)[2:4] + "\n")
		elif(len(str(msg.payload)) == 6):
			last_line = f1.write(str(msg.payload)[2:5] + "\n")
		else:
			last_line = f1.write(str(msg.payload)[2:19] + "\n") 

def main():
	mqtt_client = mqtt.Client()
	mqtt_client.username_pw_set(MQTT_USER,MQTT_PASSWORD)
	mqtt_client.on_connect = on_connect
	mqtt_client.on_message = on_message

	mqtt_client.connect(MQTT_ADDRESS, 1883)
	mqtt_client.loop_forever()

if __name__ == '__main__':
	print('MQTT to influxDB bridge')
	main()
