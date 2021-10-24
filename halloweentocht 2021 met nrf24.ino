#include <Servo.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

int relay[] = {29, 28, 27, 26, 25, 24, 23, 22}; //pinnen van het relay-board
int ledstrip[] = {4, 5, 6}; //pinnen van de ledstrip RGB: ROOD, GROEN, BLAUW
byte button = 2; //schakelaar aangesloten op pin 2
byte aantal_state = 5; //momenteel niet gebruikt

Servo servo_rookmachine; //pin 9

unsigned long time;
bool state[] = {true, true, true, true, true};
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

radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
radio.stopListening();

Serial.begin(9600);
}

void loop() {

byte schakelaar_status = digitalRead(button);

if(digitalRead(button) == 0)
{ 
  digitalWrite(13, HIGH); //controle led aan (op arduino zelf)
  if(schakelaar_status != digitalRead(button)){ //debug 'functie'
    Serial.println("Schakelaar is 1");}

  if(Time() < 12000 && state[0])
  {
    digitalWrite(relay[5], HIGH); //spot uit, begin van de scene
    digitalWrite(relay[6], HIGH); //extra spot uit
    digitalWrite(relay[0], LOW); //laat perslucht cilinders omhoog gaan
    servo_rookmachine.write(45);
    state[0] = false;
  }
  
  if(Time() < 12000) //hier wordt de ledstrip geregeld
  {
    if(step<256)
    {
      if(Time() > 5*step && Time() <= 5*(step+1))
      {
        analogWrite(ledstrip[0], step);
        step++;
      }
    }
  }

  if(Time() > 6000 && state[3])
  {
    servo_rookmachine.write(0); //rookmachine uit
    RadioAan();
    digitalWrite(relay[5], LOW); //spot aan
    state[3] = false;
  }

  if(Time() >= 15000 && state[1]) //cilinders zakken na 15 seconden
  {
    digitalWrite(relay[0], HIGH);
    state[1] = false;
    Serial.println("Loop 2");
  }
  //na 10 seconden moet het rode licht terug uit
  if(Time() >= 25000 && state[2])
  {
    digitalWrite(ledstrip[0], LOW); //ledstrip uit
    state[2] = false;
  }

  if(Time() >= 27000){
    for(byte i=0; i<sizeof(state)/sizeof(state[0]); i++)
    {
      state[i] = true;
    }
    RadioUit();
    digitalWrite(relay[5], HIGH); //spot uit
    time = millis();
  }
}

if(digitalRead(button)==1){
  if(schakelaar_status != digitalRead(button))  //dit zodat de loop maar 1 keer wordt uitgevoerd
  {
    NietActiefScene();
  }
  
  time = millis();
  step=0;
  for(byte i=0; i<3; i++)
  {
    state[i]=1;
  }
}
}

unsigned long Time()
{
  return millis() - time;
}

void RadioAan()
{
  const char text[] = "HIGH";
  radio.write(&text, sizeof(text));
}
void RadioUit()
{
  const char text[] = "LOW";
  radio.write(&text, sizeof(text));
}

void NietActiefScene()
{
  digitalWrite(13, LOW); //controle led uit
  digitalWrite(relay[5], LOW); //spot aan voor een beetje licht
  digitalWrite(relay[6], LOW); //eventuele extra lichtspot voor licht in de tunnel
  digitalWrite(relay[0], HIGH); //perslucht cilinders naar beneden
  for(byte i=0; i<3; i++){
   digitalWrite(ledstrip[i], LOW);
  }
  RadioUit();
}