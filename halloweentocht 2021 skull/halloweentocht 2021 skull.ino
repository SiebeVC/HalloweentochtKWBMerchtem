#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";
boolean button_state = 0;
int led_pin = 3;

Servo servo_skull;

bool status = false;
unsigned long time;

void setup() {
Serial.begin(9600);
radio.begin();
radio.openReadingPipe(0, address);   //Setting the address at which we will receive the data
radio.setPALevel(RF24_PA_MIN);       //You can set this as minimum or maximum depending on the distance between the transmitter and receiver.
radio.startListening();              //This sets the module as receiver

servo_skull.attach(6);
servo_skull.write(60); //open positie

time= millis();

}
void loop()
{
  if (radio.available())              //Looking for the data.
  {
    char text[32] = "";                 //Saving the incoming data
    radio.read(&text, sizeof(text));    //Reading the data
    radio.read(&button_state, sizeof(button_state));    //Reading the data
    Serial.println(text);
    status = String(text) == "HIGH";
    delay(5);
  }
  if (status && Time()>random(500, 750))
  {
    Serial.println(Time());
    beweeg();
    time=millis();
  }
}

void beweeg(){
  byte val = random(5, 60);
  Serial.println(val);
  servo_skull.write(val);
}

unsigned long Time(){
  return millis()-time;
}