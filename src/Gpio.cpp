#include "Gpio.hpp"
#include "IoT.hpp"

using namespace std;

std::function<bool()> gpioInput(uint8_t pin, bool pullUp)
{
    IoT.beginEvent += [=] { pinMode(pin, static_cast< uint8_t >( pullUp ? INPUT_PULLUP : INPUT )); };
    return [=] { return digitalRead(pin) == (pullUp ? LOW : HIGH); };
}

std::function<void(bool value)> gpioOutput(uint8_t pin, bool invert)
{
    IoT.beginEvent += [=] { pinMode(pin, OUTPUT); };
    return [=](bool value) { digitalWrite(pin, static_cast< uint8_t >( value == invert ? LOW : HIGH )); };
}
