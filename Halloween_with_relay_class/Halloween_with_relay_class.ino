#include "relay.h"

int adres[] = {12, 13}; // pinnen van de relays
Relay relay(2, adres);  // hoeveel relays an adres van de pinnen

bool state1[] = {true, false, true};
bool state2[] = {false, true, false};

void setup()
{
    Serial.begin(9600); // for print shit

    int a = relay.get_number_of_relays();
    Serial.println(a);

    relay.add_relay(8);
    a = relay.get_number_of_relays();
    Serial.println(a);

    relay.all_on();
    delay(2000);

    relay.print_state();

    relay.all_off();
    delay(500);
}

void loop()
{

    // for (int i = 0, count = relay.get_number_of_relays(); i < count; i++)
    // {
    //     relay.on(i);
    //     // Serial.print("Relais ");
    //     // Serial.print(i);
    //     // Serial.print(": ");
    //     Serial.println(relay.get_state(i));
    //     delay(500);
    // }

    // for (int i = 0, count = relay.get_number_of_relays(); i < count; i++)
    // {
    //     relay.off(i);
    //     // Serial.print("Relais ");
    //     // Serial.print(i);
    //     // Serial.print(": ");
    //     // Serial.println(relay.get_state(i));
    //     delay(500);
    // }

    relay.set_state(state1);
    delay(1000);
    relay.set_state(state2);
    delay(1000);
}
