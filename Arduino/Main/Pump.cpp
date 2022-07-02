#include "Debug.h"
#include "Pump.h"

Pump::Pump(byte address)
{
  _address = address;
}

Pump::Pump() {}

/*
  Will set every pump to OUTPUT
*/
void Pump::init()
{
  debug("Output on address '" + String(_address) + "' configured");
  pinMode(_address, OUTPUT);
  // The relayboard (P2H15880) is off on HIGH
  digitalWrite(_address, HIGH);
}

/*
  Activates the pump with the given address for the given ammount of time
*/
void Pump::pump(float ms)
{
  digitalWrite(_address, LOW);
  delay(ms);
  digitalWrite(_address, HIGH);
}