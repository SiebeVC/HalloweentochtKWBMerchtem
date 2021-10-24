#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;
const byte aantalTijden = 3; //hoeveel tijden worden er in het programma bijgehouden

Servo servo_skull;

bool status[] = {false, false};
unsigned long time[aantalTijden];
byte vorige_positie;
byte hoek;

void setup() {
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.startListening();              //This sets the module as receiver

servo_skull.attach(6);
servo_skull.write(60); //gesloten positie

for(byte i; i<aantalTijden; i++){
  time[i] = millis();
}
vorige_positie = 60; //vorige positie van de servo

}
void loop()
{
  if (radio.available())              //Looking for the data.
  {
    time[2] = millis(); //mark last radio contact
    char text[32] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    radio.read(&button_state, sizeof(button_state));    //Reading the data
    Serial.println(text);
    status[0] = String(text) == "HIGH";
    delay(5); //stability delay
  }
  if(status[0] && (Time(0)>random(500, 750) || !status[1])) //delay of de hoek is nog niet gelijk aan de vorige positie, soort van bypass
  {
    hoek = random(5, 60)*status[1] + hoek*(!status[1]); //random hoek die de servo moet maken
    if(Time(1)>random(20, 60) && hoek-vorige_positie!=0){ //random(20, 60) staat voor een delay tussen elke stap van de servo waardoor servo trager beweegt 
      vorige_positie = vorige_positie + 1*(hoek-vorige_positie>0) - 1*(hoek-vorige_positie<0); //moet ik optellen of aftrekken check
      servo_skull.write(vorige_positie);
      time[1]=millis();
    }
    status[1] = hoek==vorige_positie; //check dat hoek en vorige positie gelijk zijn
    time[0]=millis();
  }
  if(Time(2)>180000){ //als er na 3 minuten geen radiocontact is geweest, moet je terug beginnen te bewegen.
    status[0] = true;
  }
}

unsigned long Time(byte getal){
  return millis()-time[getal];
}