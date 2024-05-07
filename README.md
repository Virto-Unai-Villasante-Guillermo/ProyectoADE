# Proyecto ADE Unai Virto y Guillermo Villasante Master SIEAV UPV/EHU

Este proyecto forma parte de la asignatura Análisis y Desarrollo de Estructuras de Software para SoPC.

## Contenido

El grueso del proyecto se concentra en el archivo bme280server.c, el cual incluye:
1. Lectura de la temperatura, presión y humedad relativa del ambiente, debidamente calibrada.
2. Creación de un servidor udp local para envío de comandos a través de un teléfono móvil. Los comandos incluyen:
	- temp: devuelve la temperatura en grados centígrados en la terminal del dispositivo móvil.
	- hum: devuelve la humedad relativa en porcentaje.
	- press: devuelve la presión en hectopascales (hPa).
	- live: almacena los datos y la fecha y hora en una base de datos de MySQL y los muestra en la dirección http://192.168.43.2/datos.php.
	- start (min): almacena y muestra durante los minutos especificados.
	- stop: detiene los comandos live y start.
	- close: cierra la conexión con el servidor.
3. Conexión con la base de datos de MySQL para su actualizacióń.
4. Encendido y apagado de un LED conectado en el PIN 21 para informar al usuario de las acciones que se están llevando a cabo.

El archivo bme280.c contiene las funciones necesarias para acceder al sensor y leer los valores de temperatura, humedad relativa y presión. Ha sido obtenido del siguiente [repositorio](https://github.com/bitbank2/bme280).

En los archivos config.php y bme280.php se concentra la programación en PHP para acceder y añadir valores a la base de datos a través de una URL de la forma http://192.168.43.2/bme280.php?Temperatura=1&Presion=2&Humedad=3.

En el archivo datos.php hay un conjunto de instrucciones en PHP, HTML y JavaScript que permiten que la página http://192.168.43.2/datos.php se actualice cada vez que recibe un dato nuevo del sensor.

Por último, el archivo bme280server.service permite crear un servicio que se arranca al encender la RaspberryPi que arranca el servidor para su uso.

## Estado del proyecto
Finalizado

## Instalación
Compila el archivo ejecutable con el Makefile
Copia los archivos .php en el directorio /var/www/html/
Copia el archivo bme280server.service en /etc/systemd/system/
