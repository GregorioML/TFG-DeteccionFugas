import paho.mqtt.client as mqtt
import json

BROKER = '10.159.13.199'
PORT = 1883
TOPIC = 'datos_planta'
USERNAME = 'usuario_suscriptor'
PASSWORD = 'fuga_suscriptor'


# Crear cliente
client = mqtt.Client()

# Configurar usuario y contraseña
client.username_pw_set(USERNAME, PASSWORD)

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Connected to MQTT Broker!")
        client.subscribe(TOPIC)
        print(f"Subscribed to {TOPIC}") 
    else:
        print(f"Failed to connect, return code {rc}\n")


def on_message(client, userdata, msg):
    data = json.loads(msg.payload.decode())
    print(f"Tiempo: {data['tiempo']}")
    print(f"Vol. descargas: {data['vol_descargas']}")
    print(f"Vol. almacenamiento: {data['vol_almacenamiento']}")
    print(f"Vol. ventas 1: {data['vol_ventas1']}")
    print(f"Vol. ventas 2: {data['vol_ventas2']}")
    print(f"Vol. fugas: {data['vol_fugas']}")
    print(f"Próx. vol. descarga: {data['prox_vol_descarga']}")
    print(f"Próx. vol. ventas 1: {data['prox_vol_ventas1']}")
    print(f"Próx. vol. ventas 2: {data['prox_vol_ventas2']}")
    print(f"Vol. acum. descargas: {data['vol_acum_descargas']}")
    print(f"Vol. acum. ventas 1: {data['vol_acum_ventas1']}")
    print(f"Vol. acum. ventas 2: {data['vol_acum_ventas2']}")
    print(f"Vol. acum. fugas: {data['vol_acum_fugas']}")
    print(f"Próx. inst. descarga: {data['prox_inst_descarga']}")
    print(f"Próx. inst. ventas 1: {data['prox_inst_ventas1']}")
    print(f"Próx. inst. ventas 2: {data['prox_inst_ventas2']}")
    print(f"Cantidad descargas: {data['cantidad_descargas']}")
    print(f"Cantidad ventas 1: {data['cantidad_ventas1']}")
    print(f"Cantidad ventas 2: {data['cantidad_ventas2']}")
    print(f"Altura fuga: {data['altura_fuga']}")
    print(f"Dimensiones fuga: {data['dimensiones_fuga']}")
    print("\n")

client.on_connect = on_connect
client.on_message = on_message


client.connect(BROKER, PORT)
client.loop_forever()       #Para mantener el hilo ejecutándose hasta que se haya procesado todos los mensajes entrantes