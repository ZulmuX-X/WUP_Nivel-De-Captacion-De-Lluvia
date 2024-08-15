# Introduccion
Este proyecto utiliza varios sensores como el Modulo DHT11 (Sensor de temperatura y humedad), sensor de flujo de agua, sensor ultrasonico, entre otros dispositivos. Que funcionando en conjunto realizan el monitoreo de cuanta agua entra al contenedor y cuanto porcentaje existe del mismo, asu vez detecta los cambios de temperatura y humedad, mostrando los datos  en un sitio web podiendo moritorizar los cambios en tiempo real y a lo largo del tiempo.

# Objetivo
Combinar las tecnologias de deteccion de flujo de agua, nivel de agua, temperatura y humedad, comunicacion inalambrica y visualizacion grafica para dar solucion al monitoreo en tiempo real facil de usar y accesible.

# Materiales
## Hardware 
- ESP32 CAM
- Protoboard
- Modulo DHT11 (Sensor de temperatura y humedad)
- Sensor de flujo de agua (YF-S201)
- Sensor Ultrasonico (Hc-sr04)
- Bomba de agua sumergible
- Transistor PNP (2N2222)
- Cable USB a Mini US
- Baterías 9V
- Tubos de PVC
- Contenedor
- Jumpers

## Software
- IDE Arduino

# Requisitos previos
## IDE Arduino
Para poder subir el codigo a nuestro ESP-32 CAM. Requerimos tener previamente instalado el software IDE Arduino.
[Arduino Software](https://www.arduino.cc/en/software)

```python
sudo apt install arduino arduino-core
```
## Agregar tarjeta EPS32
Para agregar la url de la tarjeta del Esp32 debemos ir al Arduino IDE y seleccionar Archivo -> Preferencias

[![image.png](https://i.postimg.cc/MZg0LP9S/image.png)](https://postimg.cc/s19ZQcF6)

Luego en "Gestor de URLs Adicionales de Tarjeta" agregamos: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

[![image.png](https://i.postimg.cc/5t98zZ2w/image.png)](https://postimg.cc/3k6y5fpR)

## Agregar librerias
Las librerías utilizadas en este proyecto son "ArduinoJson", "PubPublish", "DHT11" entre otrod, se pueden instalar desde el gestor de librería del IDE Arduino. También se pueden descargar directamente e instalarlas clonando  git hub a las librerias del programa
```python
git clone "url"
```

[![image.png](https://i.postimg.cc/7hS5dVLH/image.png)](https://postimg.cc/dZtQTGdp)
[![image.png](https://i.postimg.cc/FK17NmKn/image.png)](https://postimg.cc/4KkJQDVz)

## Agregar Node-Red y MySQL
En una terminal debemos colocar los siguientes comandos
[![image.png](https://i.postimg.cc/xdWTQzhc/image.png)](https://postimg.cc/7CnyMfxk)

```python
sudo apt-get update
sudo apt-get upgrade
bash <(curl -sL https://raw.githubusercontent.com/node-red/linux-installers/master/deb/update-nodejs-and-nodered)
```

Este comando puede variar por *mariadb*
```python
sudo apt-get install mysql-server
```
# Instrucciones
## Circuito sugerido