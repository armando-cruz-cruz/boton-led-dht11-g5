/*
 * Bonton-led
 * Por: Armando Cruz
 * Fecha: 20 de abril de 2022
 * 
 * Programa que recibe de entrada 3 botones y la temperatura del sensor DHT11
 * y enciende los leds dependiendo de las condiciones
 */
// Bibliotecas
#include "DHT.h"

#define DHTPIN 14    //Pin donde se conecta el DHT11
#define DHTTYPE DHT11 
#define MAX_TEMP 26

// Constantes
const int LED1=2; //Refrigeracion manual
const int LED2=4; //Refrigeración automática
const int BOTON1=13; //Manual. Activa la refrigeración al ser presionado
const int BOTON2=12; //Alta demanda
const int BOTON3=15; //Sobre carga de funcionamiento
// Variables
int dato1,dato2,dato3; //Botones
float t;  //Temperatura
// Definición de objetos
DHT dht(DHTPIN, DHTTYPE);
// Condiciones iniciales - Se ejecuta sólo una vez al energizar
void setup() {// Inicio de void setup ()
  // Aquí va tu código
  pinMode(BOTON1,INPUT_PULLUP); //Configurar el pin del boton como entrada
  pinMode(BOTON2,INPUT_PULLUP); //Configurar el pin del boton como entrada
  pinMode(BOTON3,INPUT_PULLUP); //Configurar el pin del boton como entrada
  pinMode(LED1,OUTPUT); //Configurar el pin de led como salida
  pinMode(LED2,OUTPUT); //Configurar el pin de led como salida
  digitalWrite(LED1,LOW); //empieza el led apagado
  digitalWrite(LED2,LOW); //empieza el led apagado
  Serial.begin(115200);
  Serial.println(F("Conexion iniciada."));
  dht.begin();
}// Fin de void setup

// Cuerpo del programa - Se ejecuta constamente
void loop() {// Inicio de void loop
  delay(200);
  lecturaSensor();
  lecturaBotones();
  impresionDatos();
  logica();
}// Fin de void loop

// Funciones del usuario
//Funcion que toma la lectura del sensor DHT11
void lecturaSensor() {
  t = dht.readTemperature(); // Leer la temp en celsius
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
}

//Funcion que toma la lecturas de los 3 botones
void lecturaBotones() {
  dato1=!digitalRead(BOTON1); //Leer el pin del boton1 (Manual)
  dato2=!digitalRead(BOTON2); //Leer el pin del boton1 (alta demanda)
  dato3=!digitalRead(BOTON3); //Leer el pin del boton1 (sobrecarga)
}

//Funcion que evalua las entradas para decidir que leds encender
void logica() {
  if ((dato2 || dato3) && t>MAX_TEMP) { //Alta demanda o sobrecarga y la temperatura es alta
    digitalWrite(LED1, HIGH);     //Enciende los dos refrigeraciones
    digitalWrite(LED2,HIGH);
  }
  else if (dato1) {  //Si esta aplanado el boton manual
    digitalWrite(LED1, HIGH);  //Solo enciende la refrigeracion manual
    digitalWrite(LED2,LOW);
  }
  else if (dato2 || dato3 || t>MAX_TEMP) { // alta demanda o sobrecarga o la temperatura es alta
    digitalWrite(LED1, LOW);  //Solo enciende la refrigeracion automatica
    digitalWrite(LED2,HIGH);  
  }
  else {                            // Si no se cumple nada de las condiciones anterios
    digitalWrite(LED1, LOW);        //ambas refrigeraciones apagadas
    digitalWrite(LED2,LOW);
  }
}

//Funcion que imprime los datos en monitor serial
void impresionDatos() {
  Serial.print(F("Temp: "));
  Serial.print(t);
  Serial.print(F("°C, boton1: "));
  Serial.print(dato1);
  Serial.print(F(", boton2: "));
  Serial.print(dato2);
  Serial.print(F(", boton3: "));
  Serial.println(dato3);
}
