#include "Animation.hpp"
#include "ControllerNormal.hpp"
#include "ControllerSetup.hpp"
#include "Heater.hpp"
#include "Pixels.hpp"
#include "Algorithm.hpp"

ControllerModeNormal::ControllerModeNormal(Controller& c) noexcept
        : c_(c),
          heatingAnimation_(Color::faintred(), Color::red(), 2000),
          coolingAnimation_(Color::faintorange(), Color::orange(), 4000)
{
}

void ControllerModeNormal::update(uint32_t elapsed)
{
    auto setpoint = c_.heater_.get();
    auto temperature = c_.heater_.read();

    // pixel 1: power indicator
    c_.pixels_.SetPixelColor(0, Color::blue());

    // pixel 2: heating indicator
    if (setpoint > 0) {
        if (temperature < setpoint - Controller::hysteresis) {
            c_.pixels_.SetPixelColor(1, heatingAnimation_.animate(elapsed));
        } else {
            c_.pixels_.SetPixelColor(1, Color::green());
        }
    } else if (temperature > 50.0) {
        c_.pixels_.SetPixelColor(1, coolingAnimation_.animate(elapsed));
    }

    // pixels 3-10: temperature indicator
    auto value = map(temperature, 20.0, 100.0, 0u, (c_.pixels_.PixelsCount() - 2) * 255u);
    for (size_t i = 2; i < c_.pixels_.PixelsCount(); ++i) {
        c_.pixels_.SetPixelColor(i, RgbColor(std::min(value, 255u), 0, 0));
        value = value > 255 ? value - 255 : 0;
    }
}

void ControllerModeNormal::clicked(unsigned int clicks)
{
    if (clicks == 1) {
        c_.heater_.toggle();
    }
}

std::unique_ptr<ControllerMode> ControllerModeNormal::next()
{
    c_.heater_.set(0.0);
    return std::unique_ptr<ControllerMode>(new ControllerModeSetup(c_));
}
