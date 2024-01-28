#include "Animation.hpp"
#include "ControllerNormal.hpp"
#include "ControllerSetup.hpp"
#include "Heater.hpp"
#include "Pixels.hpp"

static unsigned calc_temp_8px(double temperature)
{
    static constexpr double minimum = 20.0;
    static constexpr double maximum = 100.0;
    static constexpr uint16_t pixels = 8;

    if (temperature < minimum) {
        return 0;
    }
    if (temperature >= maximum) {
        return pixels;
    }
    return static_cast<unsigned>((temperature - minimum) * pixels * 256 / (maximum - minimum));
}

ControllerModeNormal::ControllerModeNormal(Controller &c) noexcept
        : c_(c),
          heatingAnimation_(PulseAnimation(Color::faintred, Color::red, 1000))
{
}

void ControllerModeNormal::update(uint32_t elapsed)
{
    auto setpoint = c_.heater_.get();
    auto temperature = c_.heater_.read();

    // pixel 1: power indicator
    c_.pixels_.SetPixelColor(0, Color::blue);

    // pixel 2: heating indicator
    if (setpoint > 0) {
        if (temperature < setpoint - Controller::hysteresis) {
            c_.pixels_.SetPixelColor(1, heatingAnimation_(elapsed));
        } else {
            c_.pixels_.SetPixelColor(1, Color::green);
        }
    }

    // pixels 3-10: temperature indicator
    auto value = calc_temp_8px(temperature);
    for (size_t i = 0; i < 8; ++i) {
        c_.pixels_.SetPixelColor(2 + i, RgbColor(std::min(value, 255u), 0, 0));
        value = value > 255 ? value - 255 : 0;
    }
}

void ControllerModeNormal::clicked(unsigned int clicks)
{
    if (clicks == 1) {
        c_.heater_.toggle();
    }
}

ControllerMode *ControllerModeNormal::next()
{
    c_.heater_.set(0.0);
    return new ControllerModeSetup(c_);
}
