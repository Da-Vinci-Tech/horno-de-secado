# HORNO DE SECADO DE MOTORES INDUSTRIALES
sistema de control para horno de secado de motores industriales usando el ecosistema de IoT **DA VINCI ORIGAMI** cuyo core es el ESP32. Realiza un perfil de ascenso y descenso de temperatura controlado por el usuario desde la plataforma **ALL TIME IOT** https://alltimeiot.com/

![render](https://user-images.githubusercontent.com/124691721/217300607-d55de268-cec6-4cef-8b26-d1125db9c5a8.jpg)

En el siguiente grafico podemos observar como la linea morada corresponde al setpoint o temperatura deseada la cual se incremente y decrementa de forma controlada, la linea azul corresponde a un control de temperatura convencional y la linea roja al perfil de temperatura que deseamos lograr con este proyecto.

![grafica horno](https://user-images.githubusercontent.com/124691721/217304398-dfd6ad70-7194-4faf-a9e0-cb6807d4a88a.png)

la siguiente grafica es del serial plotter de arduino, en la cua la linea roja es la temperatura deseada y la linea azul la temperatura real. Vemos que se va adaptando a al setpoint deseado en todo momento. 

![photo5010494498484103561](https://user-images.githubusercontent.com/124691721/217306627-352c0367-5c7a-40dd-967e-6f038800b1c9.jpg)

la siguiente grafica es la que vemos en la plataforma **ALL TIME IOT** esta no nos permite imprimir dos variables en la misma grafica, sin embargo corroboramos como el perfil de temperatura corresponde  a los tiempos establecidos

![grafica de alltimeiot](https://user-images.githubusercontent.com/124691721/217309493-511763d6-ffb5-434b-85a2-e100d97718ec.png)

¡instalamos todo dentro del horno!

![foto horno abierto](https://user-images.githubusercontent.com/124691721/217313599-d90418f3-0083-4338-80a8-f7314ed9907e.png)

¡instalamos todo en la caja de control! ¡Y LISTO!

![caja de horno](https://user-images.githubusercontent.com/124691721/217314915-55f03ad7-78f9-4e11-866a-c74f5754fc39.png)

# LISTA DE PARTES

- 1 modulos principal Da Vinci Origami: core ESP32.
- 1 display Oled: SSD1306 cuyos pines I2C son SDA(21) SCL(22).
- 2 modulos relays: cada uno contiene 2 relays cuyos pines son, turbina(15) lamparas(14) resistencias(19,23).
- 1 modulo de conexiones basicas: usado para exponer los GPIOs y conectar los sensores de temperatura.
- 2 sensores de temperatura DS18B20 al pin (32).
