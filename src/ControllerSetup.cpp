#include "Algorithm.hpp"
#include "ControllerNormal.hpp"
#include "ControllerSetup.hpp"
#include "Heater.hpp"
#include "Pixels.hpp"
#include "PushButton.hpp"

ControllerModeSetup::ControllerModeSetup(Controller& c) noexcept
        : c_(c),
          powerAnimation_(Color::blue(), Color::black(), 2000),
          presetSetpoint_(c.heater_.preset()),
          timeoutTimer_(timeoutDelay, [this] { next(); })
{
}

void ControllerModeSetup::update(uint32_t elapsed)
{
    // pixel 1: blinking setup indicator
    c_.pixels_.SetPixelColor(0, powerAnimation_.animate(elapsed));

    // pixel 2: setpoint indicator
    auto index = static_cast<unsigned>((presetSetpoint_ - 20.0) * 8 / 80.0) - 1;
    c_.pixels_.SetPixelColor(2 + index, Color::red());
}

void ControllerModeSetup::clicked(unsigned int clicks)
{
    if (clicks == PushButton::singleClick) {
        presetSetpoint_ = Heater::increasePreset(presetSetpoint_);
        timeoutTimer_.start(timeoutDelay);
    }
}

void ControllerModeSetup::next()
{
    c_.heater_.preset(presetSetpoint_);
    c_.mode_.reset(new ControllerModeNormal(c_));
}
