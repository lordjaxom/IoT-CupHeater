#include "Algorithm.hpp"
#include "ControllerNormal.hpp"
#include "ControllerSetup.hpp"
#include "Heater.hpp"
#include "Pixels.hpp"
#include "PushButton.hpp"

ControllerModeSetup::ControllerModeSetup(Controller& c) noexcept
        : c_(c),
          powerAnimation_(Color::blue(), Color::black(), 2000)
{
}

void ControllerModeSetup::update(uint32_t elapsed)
{
    // pixel 1: blinking setup indicator
    c_.pixels_.SetPixelColor(0, powerAnimation_.animate(elapsed));

    // pixel 2: setpoint indicator
    auto setpoint = clamp(c_.heater_.defaultSetpoint(), 40.0, 100.0);
    auto index = static_cast<unsigned>((setpoint - 20.0) * 8 / 80.0) - 1;
    c_.pixels_.SetPixelColor(2 + index, Color::red());
}

void ControllerModeSetup::clicked(unsigned int clicks)
{
    if (clicks == PushButton::singleClick) {
        auto setpoint = c_.heater_.defaultSetpoint() + 10.0;
        if (setpoint > 100.0) {
            setpoint = 40.0;
        }
        c_.heater_.defaultSetpoint(setpoint);
    }
}

std::unique_ptr<ControllerMode> ControllerModeSetup::next()
{
    return std::unique_ptr<ControllerMode>(new ControllerModeNormal(c_));
}
