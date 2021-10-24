#include <Servo.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int relay[] = {29, 28, 27, 26, 25, 24, 23, 22}; //pinnen van het relay-board
int ledstrip[] = {4, 5, 6}; //pinnen van de ledstrip RGB: ROOD, GROEN, BLAUW
byte button = 2; //schakelaar aangesloten op pin 2
byte aantal_state = 5; //momenteel niet gebruikt

Servo servo_rookmachine; //pin 9

int counter = 0;
unsigned long time;
bool state[] = {1, 1, 1, 1, 1};
int step = 0;

void setup() {
pinMode(13, OUTPUT); //led buildin
pinMode(button, INPUT_PULLUP);
for(byte i=0; i<8; i++){
  pinMode(relay[i], OUTPUT);
  digitalWrite(relay[i], HIGH);
}
for(byte i=0; i<3; i++){
  pinMode(ledstrip[i], OUTPUT);
  digitalWrite(ledstrip[i], LOW);
}
servo_rookmachine.attach(9); //servo aangesloten op pin 9
servo_rookmachine.write(0); //ga naar positie nul
time = millis();

Serial.begin(9600);
}

void loop() {

byte schakelaar_status = digitalRead(button);

if(digitalRead(button) == 0){ 
  digitalWrite(13, HIGH); //controle led aan (op arduino zelf)
  if(schakelaar_status != digitalRead(button)){
    Serial.println("Schakelaar is 1");}

  if(millis()-time<12000 && state[0]==1){
    digitalWrite(relay[5], HIGH); //spot uit, begin van de scene
    digitalWrite(relay[0], LOW); //laat perslucht cilinders omhoog gaan
    state[0] = !state[0];
    Serial.println("Loop 1");
  }
  
  if(millis()-time<12000){ //hier wordt de ledstrip geregeld
    if(step<256){
      if(millis()-time>5*step && millis()-time<=5*(step+1)){
        analogWrite(ledstrip[0], step);
        step++;
      }
    }
  }

  if(millis()-time>8000 && millis()-time<=14000 && state[3]==1){
    digitalWrite(relay[1], LOW); //rookmachine aan
    servo_rookmachine.write(270); //draai servo tot tegen de knop van de rookmachine
    state[3] = !state[3];
    Serial.println("Rookmachine Aan");
  }
  if(millis()-time>14000 && state[4]==1){
    digitalWrite(relay[1], HIGH); //rookmachine uit
    servo_rookmachine.write(0); //draai rookmachine terug weg
    state[4] = !state[4];
    Serial.println("Rookmachine uit");
  }

  if(millis()-time>= 15000 && state[1]==1){ //cilinders zakken na 15 seconden
    digitalWrite(relay[0], HIGH);
    //digitalWrite(relay[1], HIGH); //rookmachine uit
    state[1] = !state[1];
    Serial.println("Loop 2");
  }
  //na 10 seconden moet het rode licht terug uit
  if(millis()-time>=25000 && state[2]==1){
    digitalWrite(relay[5], HIGH); //spot uit
    digitalWrite(ledstrip[0], LOW); //ledstrip uit
    state[2] = !state[2];
    Serial.println("Loop3");
  }

  if(millis()-time>=30000){
    for(byte i=0; i<5; i++){
      state[i]=1;
    }
    step = 0;
    time = millis();
  }
}
if(digitalRead(button)==1){
  if(schakelaar_status!=digitalRead(button)){ //dit zodat de loop maar 1 keer wordt uitgevoerd
    digitalWrite(13, LOW); //controle led uit
    digitalWrite(relay[5], LOW); //spot aan voor een beetje licht
    digitalWrite(relay[0], HIGH);
    digitalWrite(relay[1], HIGH);
    for(byte i=0; i<3; i++){
     digitalWrite(ledstrip[i], LOW);
    }
  }
  time=millis();
  step=0;
  for(byte i=0; i<3; i++){
    state[i]=1;
  }
}
}

