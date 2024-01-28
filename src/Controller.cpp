#include "Controller.hpp"

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

Controller::Controller(PushButton &button, Heater &heater, Pixels &pixels) noexcept
        : heater_(heater),
          pixels_(pixels),
          updateTimer_(updateDelay, true, [this] { update(); }),
          clicked_(button.clickedEvent.subscribe([this](unsigned clicks) { clicked(clicks); }))
{
    IoT.beginEvent += [this] { begin(); };
}

void Controller::begin()
{
    pixels_.Begin();
    update();
}

void Controller::update()
{
    pixels_.ClearTo(RgbColor(0, 0, 0));

    auto setpoint = heater_.get();
    auto temperature = heater_.read();

    // pixel 1: power indicator
    pixels_.SetPixelColor(0, Color::cyan.Dim(100));

    // pixel 2: heating indicator
    if (setpoint > 0) {
        if (temperature < setpoint - hysteresis) {
            auto next = fadeAnimation_.next();
            log("next color: ", int(next.R));
            pixels_.SetPixelColor(1, next);
        } else {
            pixels_.SetPixelColor(1, Color::green.Dim(128));
        }
    }

    // pixels 3-10: temperature indicator
    auto value = calc_temp_8px(temperature);
    for (size_t i = 0; i < 8; ++i) {
        pixels_.SetPixelColor(2 + i, RgbColor(std::min(value, 255u), 0, 0));
        value = value > 255 ? value - 255 : 0;
    }

    pixels_.Show();
}

void Controller::clicked(unsigned clicks)
{
    if (clicks == 1) {
        heater_.toggle();
    }
}
