#include "Algorithm.hpp"
#include "Animation.hpp"
#include "ControllerNormal.hpp"
#include "ControllerSetup.hpp"
#include "Heater.hpp"

ControllerModeSetup::ControllerModeSetup(Controller &c) noexcept
        : c_(c),
          powerAnimation_(PulseAnimation(Color::blue, Color::black, 1000))
{
}

void ControllerModeSetup::update(uint32_t elapsed)
{
    // pixel 1: blinking setup indicator
    c_.pixels_.SetPixelColor(0, powerAnimation_(elapsed));

    // pixel 2: setpoint indicator
    auto setpoint = clamp(c_.heater_.defaultSetpoint(), 40.0, 100.0);
    auto index = static_cast<unsigned>((setpoint - 20.0) * 8 / 80.0) - 1;
    c_.pixels_.SetPixelColor(2 + index, Color::red);
}

void ControllerModeSetup::clicked(unsigned int clicks)
{
    if (clicks == 1) {
        auto setpoint = c_.heater_.defaultSetpoint() + 10.0;
        if (setpoint > 100.0) {
            setpoint = 40.0;
        }
        c_.heater_.defaultSetpoint(setpoint);
    }
}

ControllerMode *ControllerModeSetup::next()
{
    return new ControllerModeNormal(c_);
}
