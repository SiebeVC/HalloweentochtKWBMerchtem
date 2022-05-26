#include "relay.h"

////////////////////// Helpfull methods for scope:///////////////////////////////////

/**
 * @brief Not yet implemented
 */
void Relay::emergency() {} // emergency method, choose which relays that need to be on
                           // in case of emergency

///////////////////// Constructors and Destructors:////////////////////////////////

Relay::Relay()
{
    number = 0;
    relay = NULL;
    state = NULL;
}

Relay::Relay(int number, int *adres)
{
    this->number = number;
    relay = new int[number]; // dynamically allocated!!
    state = new bool[number];
    for (int i = 0; i < number; i++)
    {
        relay[i] = adres[i];
        state[i] = 0;
        pinMode(relay[i], OUTPUT);
    }
}

Relay::~Relay()
{
    all_off();      // turn of all high signals
    delete[] relay; // delete dynamically allocated arrays.
    delete[] state;
}

/////////////////////// Getters and Setters: //////////////////////////////////////////////

/**
 * @brief Returns pointer to state. (not quite useful)
 */
bool *Relay::get_state() const
{
    return state;
}

/**
 * @brief Give status from relay with index.
 * @param index from relay.
 * @return state from relay, or false when index out of range.
 */
bool Relay::get_state(int index) const
{
    if (index >= 0 && index < number)
    {
        return state[index];
    }
    return false;
}

/**
 * @brief Give status from relay with index.
 * @param index from relay.
 * @return state from relay, or false when index out of range.
 */
bool Relay::operator[](int index) const
{
    return get_state(index);
}

/**
 * @brief Returns number of relays.
 * @return int
 */
int Relay::get_number_of_relays() const
{
    return number;
}

/**
 * @brief Adds a relay to the object, with the given pin number.
 * @param pin
 */
void Relay::add_relay(int pin)
{
    pinMode(pin, OUTPUT); // make sure the pin is set as output!
    // some reallocs are needed, not possible in C++ so we need to make some copies on the stack.
    number++;
    bool *next_state = new bool[number];
    int *next_relay = new int[number];

    for (int i = 0; i < number - 1; i++)
    {
        next_state[i] = state[i];
        next_relay[i] = relay[i];
    }
    next_state[number - 1] = 0;
    next_relay[number - 1] = pin;

    delete[] state; // delete allocated memory
    delete[] relay;

    state = next_state;
    relay = next_relay;
}

////////////////////////////////////// Set State //////////////////////////////////////////

/**
 * @brief Set de relay state. MAKE SURE YOUR ARRAY HAS THE RIGHT LENGHT!
 * Use this function on your own risk. The on/off functions are prefered.
 * @param state
 */
void Relay::set_state(bool *state)
{
    for (int i = 0; i < number; i++)
    {
        this->state[i] = state[i];
        digitalWrite(relay[i], state[i]);
    }
}

/////////////////////////////// On off methods //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Turns al relays on if condition is true.
 * @param condition
 */
void Relay::all_on(bool condition)
{
    if (condition)
    {
        for (int i = 0; i < number; i++)
        {
            state[i] = true;
            digitalWrite(relay[i], HIGH);
        }
    }
}

/**
 * @brief Turns all relays on.
 */
void Relay::all_on() { return all_on(true); }

/**
 * @brief Turns all relays off if condition is true.
 * @param condition
 */
void Relay::all_off(bool condition)
{
    if (condition)
    {
        for (int i = 0; i < number; i++)
        {
            state[i] = false;
            digitalWrite(relay[i], LOW);
        }
    }
}

/**
 * @brief Turns all relays off.
 */
void Relay::all_off() { return all_off(true); }

/////////////////////// Single On/Off /////////////////////////////

/**
 * @brief Turn on relay with index if condition is true.
 * @param index
 * @param condition
 */
void Relay::on(int index, bool condition)
{
    if (condition)
    {
        if (index >= 0 && index < number)
        {
            state[index] = true;
            digitalWrite(relay[index], HIGH);
        }
    }
}

/**
 * @brief Turn on relay with index.
 * @param index
 */
void Relay::on(int index) { return on(index, true); }

/**
 * @brief Turn off relay with index if condition is true.
 * @param index
 * @param condition
 */
void Relay::off(int index, bool condition)
{
    if (condition)
    {
        if (index >= 0 && index < number)
        {
            state[index] = false;
            digitalWrite(relay[index], LOW);
        }
    }
}

/**
 * @brief Turn off relay with index.
 * @param index
 */
void Relay::off(int index) { return off(index, true); }

////////////////////////  Togle methods ////////////////////////////
/**
 * @brief Toggle status from relay.
 * @param index
 */
void Relay::toggle(int index)
{
    if (index >= 0 && index < number)
    {
        state[index] = !state[index];
        digitalWrite(relay[index], state[index]);
    }
}

/**
 * @brief Toggle status from relay.
 * @param index
 */
void Relay::operator()(int index)
{
    return toggle(index);
}
// print method

/**
 * @brief Prints status from relays, for debug purposes. Dont forget to add Serial.begin() in your code.
 */
void Relay::print_state()
{
    Serial.println("Status van relais: ");
    for (int i = 0; i < number; i++)
    {
        Serial.print("\tRelais ");
        Serial.print(i);
        Serial.print(": ");
        if (state[i])
        {
            Serial.println("Aan");
        }
        else
        {
            Serial.println("Uit");
        }
    }
}

////////////////////// Private methods///////////////////////////////////
// none for the moment