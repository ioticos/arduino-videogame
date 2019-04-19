/*
_____    ____    _______   _____    _____    ____     _____
|_   _|  / __ \  |__   __| |_   _|  / ____|  / __ \   / ____|
 | |   | |  | |    | |      | |   | |      | |  | | | (___
 | |   | |  | |    | |      | |   | |      | |  | |  \___ \
_| |_  | |__| |    | |     _| |_  | |____  | |__| |  ____) |
|_____|  \____/     |_|    |_____|  \_____|  \____/  |_____/

*/
//Desarrollado por Pablo Sánchez para Curso Master arduino
// TREMENDO CUPON DE DESCUENTO EN:
// https://www.udemy.com/master-en-arduino/?couponCode=IOTICOS_ARDUINO_10
// si te gustó el Proyecto del Video juego no borres este link GRACIAS!

/* EXPLICACIÓN pines
DISPLAY:      ARDUINO UNO
VCC       ->      5V
GND       ->      GND
SCL       ->      A5
SDA       ->      A4

POTENCIOMETRO LINEAL DE 5 A 10K   ||    ARDUINO UNO
PATA IZQUIERDA                    ->      GND
PATA CENTRAL                      ->      A0
PATA DERECHA                      ->      +5V

(LED)
PATA POSITIVA LED MEDIANTE RESISTENCIA APROPIADA A PIN 2 DEL ARDUINO UNO
NEGATIVO DEL LED A GND DEL ARDUINO UNO

(PULSADOR)
PATA PULSADOR A PIN 3 DEL ARDUINO UNO
PATA PULSADOR A GND

(BUZZER)
A PIN 2 DEL ARDUINO UNO
*/

//includes para usar display ssd1306 (recuerda corregir en el .h el tamaño correcto de tu display en el caso que estés importando la librería desde cero)
#include <SPI.h>
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//si tu display no usa pin reset, aquí pon -1 para no desperdiciar un pin de tu arduino
Adafruit_SSD1306 display(-1);

//definicion de pines
byte pote_pin = A0;
byte led = 2;
byte boton_inicia_juego = 3;

//variables globales
byte tamano_cursor = 30;
byte posicion = 30;
byte bala_x = 64;
byte bala_y = 0;
byte ant_bala_x = 64;
byte ant_bala_y = 0;
byte puntaje = 0;
byte vidas = 5;

byte cuenta_puntos = 0;
byte nivel = 1;
bool bala_creada= true;
bool inicia_juego= false;

void setup() {

  Serial.begin(9600);
  //Inicio el display para la AxAA dirección
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  pinMode(boton_inicia_juego, INPUT_PULLUP);
  pinMode(led,OUTPUT);
  attachInterrupt(digitalPinToInterrupt(boton_inicia_juego), cambia_estado_inicia_juego, FALLING);
  display.display();
  display.clearDisplay();
  display.display();
  display.setCursor(0,28);
  display.println("P R E S S  S T A R T");
  display.display();
}

void loop() {

  if (inicia_juego == false){
      delay(1000);
      display.clearDisplay();
      display.setCursor(0,28);
      display.setTextColor(WHITE,BLACK);
      display.println("P R E S S  S T A R T");
      display.display();
      tone(led, 1000, 100);
      delay(100);
      tone(led, 2000, 100);
      delay(100);
      tone(led, 4000, 100);
      delay(100);
      tone(led, 8000, 100);
      delay(100);
      tone(led, 16000, 100);

      while(inicia_juego==false){
        delay(100);

      }
      // Borrar display
      display.clearDisplay();
      randomSeed(analogRead(A2));
      dibuja_puntaje();
      dibuja_vidas();
      dibuja_nivel();
  }

  digitalWrite(led,!digitalRead(led));
  randomSeed(analogRead(A2));
  byte valor = map (analogRead(pote_pin), 0, 1024, 0, 128 - tamano_cursor);

  if (bala_creada) {
    mueve_bala(valor);
  }

  dibuja_cursor(valor);
}

void mueve_bala(byte posicion) {
  display.drawPixel(ant_bala_x, ant_bala_y, BLACK);

  bala_x += random(-5, 5);

  if (bala_x > 127) {
    bala_x = 127;
  }

  if (bala_x < 1) {
    bala_x = 0;
  }

  bala_y ++;

  if (bala_y == 63 && bala_x > posicion && bala_x < posicion + tamano_cursor ) {
    puntaje ++;
    bala_y = 0;
    bala_x = 30;
    dibuja_puntaje();

  }

  if (bala_y > 64 ) {
    vidas--;
    bala_y = 0;
    bala_x = 30;
    dibuja_vidas();
  }

  display.drawPixel(bala_x, bala_y, WHITE);
  display.display();

  ant_bala_x = bala_x;
  ant_bala_y = bala_y;
}

void dibuja_cursor(byte pos) {
  display.drawLine(0, 63,  127, 63, BLACK);
  display.drawLine(pos, 63,  pos + tamano_cursor, 63, WHITE);
  display.display();
}

void dibuja_puntaje(){
  tamano_cursor--;
  display.setCursor(0,0);
  display.setTextColor(WHITE,BLACK);
  display.println("P " + String(puntaje));
  display.display();
  tone(led, 5000, 100);
  cuenta_puntos ++;
  if (cuenta_puntos > 5){
    nivel ++;

  tone(led, 5000, 100);
  delay(300);
  tone(led, 7000, 100);
  delay(300);
  tone(led, 10000, 100);
  delay(300);
  tone(led, 12000, 100);
  delay(300);
  tone(led, 16000, 100);
  cuenta_puntos = 0;
  dibuja_nivel();
  }
}

void dibuja_vidas(){
  display.setCursor(104,0);
  display.setTextColor(WHITE,BLACK);
  display.println("V "+ String(vidas));
  display.display();
  tone(led, 800, 500);
  if (vidas == 0){
    vidas = 5;
    nivel = 0;
    tamano_cursor = 30;
    display.clearDisplay();
    display.setCursor(0,28);
    display.println("  G A M E  O V E R");
    display.display();
    tone(led, 300, 2500);
    delay(3000);
    inicia_juego = false;
  }
}

void dibuja_nivel(){
  display.setCursor(58,0);
  display.setTextColor(WHITE,BLACK);
  display.println("N "+String(nivel));
  display.display();
  tone(led, 800, 500);
  tone(led, 12000, 500);
  tone(led, 15000, 500);
  tone(led, 800, 500);
}

void cambia_estado_inicia_juego(){
  inicia_juego = true;
  Serial.println(inicia_juego);
}
