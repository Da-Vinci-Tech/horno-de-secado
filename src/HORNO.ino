//-----------------------------------Declaracion de librerias--------------------------------

#include "AlltimeESP32.h"
//#include "Wire.h"
#include <Wire.h>
#include <OLED_I2C.h>//libreria necesaria para el control de la pantala OLED .  web recomendada para descargar http://www.rinkydinkelectronics.com/library.php?id=79
#include <Preferences.h> //LIBRERIAS NECESARIAS PARA GUARDAR EN LA MEMORIA FLASH DEL ESP32
#include <WiFi.h>
Preferences preferences;

//LIBRERIAS NECESARIAS PARA ADQUIRIRDATOS DE LOS SENSORES DE TEMPERATURA
#include <OneWire.h>
#include <DallasTemperature.h>

//INSTRUCCIONES INICIALES DE CONFIGURACION DEL PUERTO ONE WIRE
const int oneWireBus = 32; //Selecciona el pin al que se conectaran los sensores de temperatura en el lado D del ORIGAMI
OneWire oneWire(oneWireBus); //Comunicar que vamos a utilizar la interfaz oneWire
DallasTemperature sensors (&oneWire); //Indica que el sensor utilizará la interfaz OneWire

//INDICAMOS LAS DIRECCIONES ROM DE CADA SENSOR DS18B20 LAS CUALES SE HAN ADQUIRIDO PREVIAMENTE
DeviceAddress temphorno = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0x89, 0x6C, 0x4F }; //direccion ROM de sensor de temperatura ambiente del horno (al sensor fisico se le debe colocar una etiqueta de identificacion)
DeviceAddress tempmotor = { 0x28, 0xFF, 0x64, 0x1E, 0x15, 0x9D, 0x5C, 0x26 }; //direccion ROM de sensor que se debe insertar dentro del motor (al sensor fisico se le debe colocar una etiqueta de identificacion)

ESP32_B Alltime;//instacia de la libreria

//-----------------------------------Declaracion de variables--------------------------------

const char* key = "I1RIj"; const char* usuario = "P8zLn"; const char* pasw = "P8zLn_iotalltime"; // datos de conexion con la plataforma
//char ssid[] = "RED"; char pass[] = "12345678"; // datos de conexion WIFI
char ssid[] = "Pacific-Logging"; char pass[] = "P@c1f1c-L0gg1ng"; // datos de conexion WIFI


//SE CREAN FUNCIONES
void calentamiento();
void secado();
void enfriamiento();

//VARIABLES DEL SISTEMA
int luz;                //variable donde se debe guardar la intensidad de la luz si se desea utilizar
float temp;             //variable donde se debe guardar la temperatura de la placa
float tempenf;          //variable donde se guarda el setpoint de enfriamiento lento
float tempcal;          //variable donde se guarda el setpoint de calentamiento lento
float tempset2;         //setpoint que se incremente o decrementa de forma lenta
//double tempReading = 0;
unsigned int banderaorg = 0;      //esta variable le indica al loop principal si debe ir a la funcion de calentamiento lento, secado o enfriamiento.
unsigned long conteo = 0;     //aqui se guarda el contador en todas las funciones
float temp2;            //aqui se guarda la temperatura inicial al empezar el calentamiento lento
float temperatureC;     //variable de la temperatura real con la que trabaja el control, el usuario o programador puede signarle temphorno o tempmotor segun prefiera.
float TempHorno;        //aqui se guarda el dato del sensor que mide la temp ambiente del horno
float TempMotor;        //aqui se guarda el dato del sensor que mide la temp dentro del motor.
int Start = 0;
byte estado = 0; //esta variable se usa para elegir 

extern uint8_t logo[]; //variable de la imagen del logo de DA VINCI TECH

//DEFINICION PINES DE SENSORES Y ACTUADORES

#define tempPin 39  //pin de sensor de temp de la placa
#define luzPin 36   //pin de sensor de nivel de luz
#define soplador 15
#define lamp 14     //pin de salida de activacion de lamparas del modulo del lado B
#define res 19      //pin de activasion de resistencia del modulo del lado E
#define res2 23     //pin de activasion de resistencia del modulo del lado C
#define B 26
#define BotonRojo 33
#define BotonVerde 25

//VARIABLES DEL USUARIO

float tempset = 0;           //aca se guarda la temperatura deseada por el usuario
unsigned long tiemposet = 0;  //aca se guarda por cuanto tiempo quiere mantener la temperatura deseada
unsigned long tiempocal = 0;  //aca se guarda el tiempo en que desea que se alcance la temperatura deseada
unsigned long tiempoenf = 0; //aca se guarda el tiempo en que se desea que descienda la temperatura
unsigned long stoptemp = 30;  //aca se guarda la temperatura final a la que desea permanecer despues del enfriamiento, debe ser una temperatura menor a tempset
bool parada = 0;              //aca se guarda la parada de emergencia




//------variable de envio hacia la plataforma-------------------

String temperatura = ""; //varible de envio de datos del sensor
#define iniystop "ST4" //lectura del dato en la plataforma
#define t_set "f1Z" //lectura del dato de la intensidad en la plataforma
#define t_enfr "maP" //lectura del dato de la intensidad en la plataforma
#define tiempo_set "ZFa" //lectura del dato de la intensidad en la plataforma
#define tiempo_cal "4bi" //lectura del dato de la intensidad en la plataforma
#define temp_motor "cqY" // apuntador de conexion a widget en la plataforma
#define temp_horno "p4" // apuntador de conexion a widget en la plataforma
OLED  myOLED(21, 22);// inicializamos la pantalla OLED
extern uint8_t SmallFont[];// Selecciono el tamaño de fuente para los textos
extern uint8_t MediumNumbers[];//variable para tamaño de fuente medio para los numeros
extern uint8_t BigNumbers[];//variable para tamaño de fuente grande para los numeros
//-----------------------------------Configuracion del setup--------------------------------

void setup() {

Wire.begin(21, 22); //pines I2C 21->SDA 22->SCL
myOLED.begin(SSD1306_128X64);//inicializa el display OLED
myOLED.setBrightness(255);
  // Ahora damos un mensaje de inicio del programa
myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
myOLED.print("Sistema de control", CENTER, 0);//imprime la frase entre comillas
myOLED.print("de horno de secado", CENTER, 15);//imprime la frase entre comillas
myOLED.print("powered by", CENTER, 40);//imprime la frase entre comillas
myOLED.print("DA VINCI TECH", CENTER, 55);//imprime la frase entre comillas
myOLED.update();// actualiza la pantalla
delay(3000);
myOLED.clrScr(); // borra la pantalla
  //web recomendada para convertir png a bitmap http://www.rinkydinkelectronics.com/t_imageconverter_mono.php
myOLED.drawBitmap(33, 0, logo, 55, 64); //imprimimos el bitmap del logo
myOLED.update();
delay(3000);
myOLED.clrScr(); // borra la pantalla

  //Serial.begin(115200);
  //iniciamos y configuramos la resolucion de ambos sensores
  sensors.begin();
  sensors.setResolution(temphorno, 10);
  sensors.setResolution(tempmotor, 10);

  delay(1000);

  pinMode(B, OUTPUT);
  pinMode(soplador, OUTPUT);
  pinMode(lamp, OUTPUT);
  pinMode(res, OUTPUT);
  pinMode(res2, OUTPUT);
  pinMode(BotonVerde, INPUT);
  pinMode(BotonRojo, INPUT_PULLUP);
    
  preferences.begin("horno", false); //se crea un epacio dentro de la memoria NVS del ESP32
  //SE CREAN LAS SIGUIENTES KEYS, SI NO EXISTEN, Y SE GUARDAN LAS VARIABLES CORRESPONDIENTES, SI EXISTEN SE ADQUIEREN LOS VALORES ALMACENADOS EN MEMORIA.
  conteo = preferences.getULong("conteo", conteo);
  banderaorg = preferences.getUInt("banderaorg", banderaorg);
  tempset = preferences.getFloat("tempset", tempset);
  tiemposet = preferences.getULong("tiemposet", tiemposet);
  tiempocal = preferences.getULong("tiempocal", tiempocal);
  tiempoenf = preferences.getULong("tiempoenf", tiempoenf);
  temp2 = preferences.getFloat("temp2", temp2);
  stoptemp = preferences.getULong("stoptemp", stoptemp);
  


  Alltime.setup (ssid, pass, key, usuario, pasw);
  myOLED.clrScr(); // borra la pantalla
  myOLED.print("Conectado al WiFi:", CENTER, 0);//imprime la frase entre comillas
  myOLED.print((WiFi.SSID()), CENTER, 15);//imprime la frase entre comillas
  myOLED.print("Asignada la IP:", CENTER, 30);//imprime la frase entre comillas
  myOLED.print((WiFi.localIP()).toString(), CENTER, 45);//imprime la frase entre comillas
  myOLED.update();
  delay(3000);
  myOLED.clrScr(); // borra la pantalla



  Alltime.suscripcion("4bi", 0);
  Alltime.suscripcion("ZFa", 0);
  Alltime.suscripcion("maP", 0);
  Alltime.suscripcion("f1Z", 0);
  Alltime.suscripcion("ST4", 10);

digitalWrite(B, HIGH);
delay(300);
digitalWrite(B, LOW);
delay(300);
digitalWrite(B, HIGH);
delay(300);
digitalWrite(B, LOW);
delay(300);
digitalWrite(B, HIGH);
delay(300);
digitalWrite(B, LOW);

if (banderaorg == 5){
      banderaorg = 0;
      tempset = 0;
      tempset2 = 0;           
      tiemposet = 0;  
      tiempocal = 0;  
      tiempoenf = 0;
      conteo = 0;
      estado = 0;
      preferences.clear();
  }

if (banderaorg > 0 && banderaorg < 5){
    estado = 1;
    digitalWrite(B, LOW);
    }
}

//*******************************************************inicio del programa**************************************************************


//--------------------------------------------------------Programa principal--------------------------------------------------------------

void loop() {

delay(806);
myOLED.clrScr(); //borra la pantalla para reiniciar el loop

//A CONTINUACION SE RECIBEN LAS VARIABLES DE CONFIGURACION DE LA PLATAFORMA ALLTIME IOT
Start = Alltime.Leer(iniystop).toInt();

if (estado == 1 && Start == 0){ //si se corto la energia antes de terminar el proceso pasa lo siguiente. 
      estado = 2;  
  }
  
if (estado == 2 && Start == 1){
      estado = 3;
  }
  
if (estado == 3 && Start == 0 or digitalRead(BotonVerde) == 1){
      banderaorg = 0;
      tempset = 0;
      tempset2 = 0;           
      tiemposet = 0;  
      tiempocal = 0;  
      tiempoenf = 0;
      conteo = 0;
      estado = 0;
      digitalWrite(B, LOW);
      preferences.clear();
      Serial.println("borrando todo...");
      myOLED.clrScr(); //borra la pantalla para reiniciar el loop
      myOLED.print("PARADA MANUAL", LEFT, 0);//imprime la frase entre comillas
      myOLED.print("A detenido el", CENTER, 10);//imprime la frase entre comillas
      myOLED.print("horno manualmente", CENTER, 20);//imprime la frase entre comillas
      myOLED.update();//actualiza la pantalla
      delay(5000);
      myOLED.clrScr(); //borra la pantalla para reiniciar el loop
      
  }
if (Start == 0 && estado == 0){
      banderaorg = 0;
      tempset = 0;
      tempset2 = 0;           
      tiemposet = 0;  
      tiempocal = 0;  
      tiempoenf = 0;
      conteo = 0;
      estado = 0;
      digitalWrite(B, LOW);
      preferences.clear();
      Serial.println("borrando todo...");
      delay (1000);
  }  
    

while (banderaorg == 0) {
  
    digitalWrite(B, LOW);
    Start = Alltime.Leer(iniystop).toInt();
    tempset = Alltime.Leer(t_set).toInt();
    tiempoenf = Alltime.Leer(t_enfr).toInt();
    tiemposet = Alltime.Leer(tiempo_set).toInt();
    tiempocal= Alltime.Leer(tiempo_cal).toInt();
    digitalWrite(lamp, LOW); //apagamos la salida de la lampara
    digitalWrite(res, LOW);
    digitalWrite(res2, LOW);
    digitalWrite(soplador, LOW);
    Serial.println("esperando datos de All Time IoT");
    
    //ENVIO DE DATOS DE TEMPERATURA A LA PLATAFORMA INCLUSO CUANDO NO ESTA INICIALIZADO EL HORNO

     //Leer la temperatura
    sensors.requestTemperatures();
    TempHorno = sensors.getTempC(temphorno);
    Alltime.Enviar(String (TempHorno), temp_horno); //envio de datos leidos hacia la plataforma
    TempMotor = sensors.getTempC(tempmotor);
    Alltime.Enviar(String (TempMotor), temp_motor); //envio de datos leidos hacia la plataforma

      if (digitalRead(BotonVerde) == 1){
    Start = 1;
    tempset=70;
    tiempocal=2;
    tiemposet=2;
    tiempoenf=2;
    estado=1;
    myOLED.clrScr(); //borra la pantalla para reiniciar el loop
    myOLED.print("INICIO LOCAL", CENTER, 0);//imprime la frase entre comillas
    myOLED.print("Set predeterminados:", CENTER, 10);//imprime la frase entre comillas
    myOLED.print("Setpoint: 70", LEFT, 20);//imprime la frase entre comillas
    myOLED.print("ascendo de temp: 2h", LEFT, 30);//imprime la frase entre comillas
    myOLED.print("Secado: 2h", LEFT, 40);//imprime la frase entre comillas
    myOLED.print("descenso de temp: 2h", LEFT, 50);//imprime la frase entre comillas
    myOLED.update();//actualiza la pantalla
    delay(5000);
    myOLED.clrScr(); //borra la pantalla para reiniciar el loop
  }    

    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
    myOLED.print("Esperando datos", CENTER, 0);//imprime la frase entre comillas
    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(tempset2,1,LEFT,8);// imprime una  variable float 
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o SETPOINT",50, 8);//imprime la frase entre comillas 
    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(TempMotor,1,LEFT,28);// imprime una  variable float  
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o MOTOR",70, 28);//imprime la frase entre comillas
    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(TempHorno,1,LEFT,48);// imprime una  variable float  
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o HORNO",70, 48);//imprime la frase entre comillas  
    myOLED.update();//actualiza la pantalla
        
    if (banderaorg==0 && tempset>0 && tiemposet>0 && tiempocal>0 && tiempoenf>0 && Start == 1){
  
//aca podria ir el codigo que ingresa a la variable bandera el valor de 1, esto dara inicio al proceso siempre y cuando el usuario haya ingresado todos los valores de temperaturas y tiempos deseados. deberia ser  un boton distinto de rojo.  
banderaorg = 1;
digitalWrite(B, HIGH);
delay(300);
digitalWrite(B, LOW);
delay(300);
digitalWrite(B, HIGH);
delay(300);
digitalWrite(B, LOW);
delay(300);
digitalWrite(B, HIGH);

  }

 }
      
    //digitalWrite(lamp, HIGH); //apagamos la salida de la lampara

    //A CONTINUACION SE GUARDAN LAS VARIABLES EN MEMORIA SOLO SI  EL PROCESO YA INICIO Y SE DEJAN DE GUARDAR SI EL PROCESO TERMINO O LLEGO A LA  TEMPERATURA DE stoptemp
    if (banderaorg > 0 && banderaorg < 5) {
      preferences.putULong("conteo", conteo);
      preferences.putUInt("banderaorg", banderaorg);
      preferences.putFloat("tempset", tempset);
      preferences.putULong("tiemposet", tiemposet);
      preferences.putULong("tiempocal", tiempocal);
      preferences.putULong("tiempoenf", tiempoenf);
      preferences.putFloat("temp2", temp2);
      preferences.putULong("stoptemp", stoptemp);
    }


    //Leer la temperatura
    sensors.requestTemperatures();

    //Lectura en grados celsius
    TempHorno = sensors.getTempC(temphorno);
    Alltime.Enviar(String (TempHorno), temp_horno); //envio de datos leidos hacia la plataforma
    TempMotor = sensors.getTempC(tempmotor);
    Alltime.Enviar(String (TempMotor), temp_motor); //envio de datos leidos hacia la plataforma
    temperatureC = TempMotor;

    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(tempset2,1,LEFT,8);// imprime una  variable float 
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o SETPOINT",50, 8);//imprime la frase entre comillas 
    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(temperatureC,1,LEFT,28);// imprime una  variable float  
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o MOTOR",70, 28);//imprime la frase entre comillas
    myOLED.setFont(MediumNumbers);//seteo el tamaño de la fuente para los numeros
    myOLED.printNumF(TempHorno,1,LEFT,48);// imprime una  variable float  
    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente 
    myOLED.print("o HORNO",70, 48);//imprime la frase entre comillas  
    myOLED.update();//actualiza la pantalla 
    

    temp = analogRead(tempPin);
    float temp = (temp*125.0)/4096.0; //convertimos a grados centigrados
    
    luz = analogRead(luzPin);
    luz = luz * 0.244;

    //Serial.print(" temp horno: "); Serial.print(temperatureC); Serial.print(" Start: "); Serial.print(Start); Serial.print(" banderaorg:"); Serial.print(banderaorg); Serial.print(" conteo:"); Serial.print(conteo); Serial.print(" tempset2:"); Serial.print(tempset2); Serial.print(" setpoint:"); Serial.print(tempset); Serial.print(" tiempocal:"); Serial.println(tiempocal);
    Serial.print(temperatureC); Serial.print(" "); Serial.print(TempHorno); Serial.print(" "); Serial.println(tempset2);

    //LLAMADO A FUNCION DE CALENTAMIENTO LENTO
    if (banderaorg == 1 or banderaorg == 2) {
      calentamiento();
      myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
      myOLED.print("Ascenso de temp", CENTER, 0);//imprime la frase entre comillas
      myOLED.update();//actualiza la pantalla
    }

    //LLAMADO A FUNCION DE SECADO
    if (banderaorg == 3) {
      secado();
      myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
      myOLED.print("Secado", CENTER, 0);//imprime la frase entre comillas
      myOLED.update();//actualiza la pantalla
    }
    //LLAMADO A FUNCION DE ENFRIAMIENTO LENTO
    if (banderaorg == 4 or banderaorg == 5) {
      enfriamiento();
      myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
      myOLED.print("Descenso de temp", CENTER, 0);//imprime la frase entre comillas
      myOLED.update();//actualiza la pantalla
    }

}

    void calentamiento() {
        
    if (banderaorg == 1) {

      temp2 = temperatureC;
      banderaorg = 2;
//      estado = 1;
    }

    tempset2 = map(conteo, 0, tiempocal * 3600, temp2 * 100, tempset * 100);
    tempset2 = tempset2 / 100;

    if (tempset2 > tempset) {
      tempset2 = tempset;
    }

    digitalWrite(soplador, HIGH);

    if (temperatureC - tempset2 > 3) {
      
      digitalWrite(lamp, LOW);
      
    }
    else {
      
      digitalWrite(lamp, HIGH);
      
    }

    if (temperatureC - tempset2 > 2) {
      digitalWrite(res, LOW);
      digitalWrite(res2, LOW);
    }
    
    else {
      digitalWrite(res, HIGH);
      digitalWrite(res2, HIGH);
    }

    
    if (temperatureC >= tempset && conteo >= tiempocal * 3600 && tempset2 >= tempset) {
      conteo = 0;
      banderaorg = 3;
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      
         }
    //if (conteo < conteo*tiempocal*3600){     
    conteo++;
    //}
    
}

    void secado() {

    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
    myOLED.print("SECADO", CENTER, 0);//imprime la frase entre comillas
    myOLED.update();//actualiza la pantalla

    digitalWrite(soplador, HIGH);
    if (temperatureC - tempset2 > 3) {
      
      digitalWrite(lamp, HIGH);
      
    }
    else {
      
      digitalWrite(lamp, LOW);
      
    }

    if (temperatureC - tempset2 > 2) {
      digitalWrite(res, LOW);
      digitalWrite(res2, LOW);
    }
    
    else {
      digitalWrite(res, HIGH);
      digitalWrite(res2, HIGH);
    }

    conteo++;
    if (conteo == tiemposet * 3600) {
      banderaorg = 4;
      conteo = 0;
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
    }
}

    void enfriamiento() {

    myOLED.setFont(SmallFont);//seteo el tamaño de la fuente
    myOLED.print("DESCENSO  DE TEMP", CENTER, 0);//imprime la frase entre comillas
    myOLED.update();//actualiza la pantalla

    if (banderaorg == 4){
    tempset2 = map(conteo, 0, tiempoenf * 3600, tempset * 100, stoptemp * 100);
    tempset2 = tempset2 / 100;
    }
    
if (temperatureC - tempset2 > 3) {
      
      digitalWrite(lamp, HIGH);
    }
    else {
      
      digitalWrite(lamp, LOW);
    }

    if (temperatureC - tempset2 > 2) {
      digitalWrite(res, LOW);
      digitalWrite(res2, LOW);
    }
    
    else {
      digitalWrite(res, HIGH);
      digitalWrite(res2, HIGH);
    }

    if (banderaorg == 4){
    conteo++;
    }

    if (tempset2 <= stoptemp && banderaorg == 4) {
      digitalWrite(soplador, LOW);
      tempset2 = stoptemp;
      preferences.clear();
      //preferences.end();
      //estado = 0;
      tempset = 0; 
      //tempset2 = 0;          
      tiemposet = 0;  
      tiempocal = 0;  
      tiempoenf = 0;
      conteo = 0;
      //Start = 0;
      banderaorg = 5;
      
    }

    if (banderaorg == 5){
       digitalWrite(lamp, LOW);
      digitalWrite(soplador, LOW);
      digitalWrite(res, LOW);
      digitalWrite(res2, LOW);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      digitalWrite(B, HIGH);
      delay(100);
      digitalWrite(B, LOW);
      delay(100);
      }
}




//*******************************************************fin del programa**************************************************************
