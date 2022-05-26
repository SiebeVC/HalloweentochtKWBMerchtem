const byte StoelOmhoog = 2; //pin 2
const byte StoelOmlaag = 3; //pin 3
const byte Doodskisten = 4; //pin 4

const long doodskistTijd = 5000; //5000 miliseconden
const long stoelTijd = 50000; //50000 miliseconden

const byte aantalTijden = 3;

bool doodskistState = true; 
bool stoelState = true;
unsigned long time[aantalTijden] = {};

void setup() {
  pinMode(StoelOmhoog, OUTPUT);
  pinMode(StoelOmlaag, OUTPUT); 
  pinMode(Doodskisten, OUTPUT);
  for(byte i=0; i<aantalTijden; i++){
    time[i] = millis();
  }
  digitalWrite(Doodskisten, doodskistState);
  digitalWrite(StoelOmhoog, stoelState);
  digitalWrite(StoelOmlaag, !stoelState);
}

void loop() {
  if(Time(1)>doodskistTijd){ // om de 5 seconden 
    digitalWrite(Doodskisten, doodskistState);
    doodskistState = !doodskistState;
    time[1] = millis();
  }

  if(Time(2)>stoelTijd){ //om de 50 seconden 
    digitalWrite(StoelOmlaag, stoelState);
    digitalWrite(StoelOmhoog, !stoelState);
    stoelState = !stoelState;
    time[2] = millis();
  }
}

unsigned long Time(byte i){
  return millis() - time[i];
}