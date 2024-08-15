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
Para entrar en el modo programacion de la ESP32-CAM puentearemos el IO0 con un GND y lo energizaremos.

## Circuito sugerido
[![image.png](https://i.postimg.cc/Gp7QMcBL/image.png)](https://postimg.cc/hJmTvRb6)

Una ves descargado y abrierto el codigo en el IDE Arduino se debera seleccionar los siguientes parametro en la pestaña __Tools__:
- Board: "AI Thinker ESP32-CAM"
- CPU Frequency: "240MHz (Wifi/BT)"
- Flash Frequency: "80MHz"
- Flash Mode: "QIO"
- Partition Scheme: "Huge APP (3MB No OTA/1MB SPIFFS)"

[![image.png](https://i.postimg.cc/fbzscvX4/image.png)](https://postimg.cc/tsM8yW2D)

Estos pasos se pueden hacer en un rasberry pi como en cualquier otro sistema operativo.
Despues de haber cargado  el codigo debemos subir el programa a la ESP32-CAM

[![image.png](https://i.postimg.cc/SKghDz7n/image.png)](https://postimg.cc/B8Pkvbz3)

Una vez se termine de subir el programa desconectaremos el puente del modo programacion y reiniciaremos la ESP32 para que ejecute el programa.

# Resultados Node-Red
Una vez se haya subirdo el codigo podremos visualizar de forma visual en Node-Red.

Antes de verlo necesitaremos iniciar noder red en una terminal:
```python
node-red
```
Tambien debremos configurar los nodos de la sig manera (sugerencia):

[![image.png](https://i.postimg.cc/9QzKCNGQ/image.png)](https://postimg.cc/0zRcCnWT)

Esto lo puenden importar con el archivo "flows.json".

Una vez realizado todo lo anterior podremos visualizar el proyecto Node-Red UI

http://192.168.1.105:1880/ui (Esto dependera de la ip donde se este ejecutando el Node-Red)

[![image.png](https://i.postimg.cc/Yq0bWLjX/image.png)](https://postimg.cc/4mC6Cd6t)

# Referencias externas
* https://nodered.org/docs/getting-started/raspberrypi
* https://www.mysql.com/
* https://www.codigoiot.com/
* https://fritzing.org/download/
