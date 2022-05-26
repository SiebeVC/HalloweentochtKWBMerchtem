#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>

class Relay
{
private:
    int number; // amount of relays
    int *relay; // array with adresses from relays, dynamically allocated
    bool *state;

public:
    Relay();
    Relay(int number, int *adres);
    ~Relay();

    bool *get_state() const;
    bool get_state(int index) const;
    bool operator[](int index) const;
    int get_number_of_relays() const;

    void add_relay(int pin);

    void set_state(bool *state);

    void all_on();
    void all_on(bool condition);
    void all_off();
    void all_off(bool condition);

    void on(int index);
    void on(int index, bool condition);
    void off(int index);
    void off(int index, bool condition);

    void toggle(int index);
    void operator()(int index);

    void emergency();

    void print_state();
};

#endif