#ifndef ESP8266_IOT_CONTROLLER_HPP
#define ESP8266_IOT_CONTROLLER_HPP

#include <NeoPixelBus.h>

#include "ColorWheel.hpp"
#include "Event.hpp"
#include "GammaTable.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "PushButton.hpp"
#include "Timer.hpp"

template<typename Pixels>
class Controller
{
    static constexpr uint32_t updateDelay = 100;
    static constexpr double hysteresis = 1.5;

public:
    Controller(PushButton& button, Heater& heater, Pixels& pixels) noexcept
            : heater_(heater),
              pixels_(pixels),
              updateTimer_(updateDelay, true, [this] { update(); }),
              clicked_(button.clickedEvent.subscribe([this](unsigned clicks) { clicked(clicks); }))
    {
        IoT.beginEvent += [this] { begin(); };
    }

private:
    void begin()
    {
        pixels_.Begin();
        update();
    }

    void update()
    {
        pixels_.ClearTo(RgbColor(0, 0, 0));

        auto setpoint = heater_.get();
        auto temperature = heater_.read();

        // pixel 1: power indicator
        set_pixel(0, ColorWheel::get(powerIndex_));
        ++powerIndex_;

        // pixel 2: heating indicator
        if (setpoint <= 0) {
            set_pixel(1, RgbColor(0, 0, 0));
        } else if (temperature < setpoint - hysteresis) {
            set_pixel(1, RgbColor(255, 0, 0));
        } else {
            set_pixel(1, RgbColor(0, 255, 0));
        }

        // pixels 3-10: temperature indicator
        auto value = calc_temp_8px(temperature);
        for (size_t i = 0; i < 8; ++i) {
            set_pixel(2 + i, RgbColor(std::min(value, 255u), 0, 0));
            value = value > 255 ? value - 255 : 0;
        }

        pixels_.Show();
    }

    void clicked(unsigned clicks)
    {
        if (clicks == 1) {
            heater_.toggle();
        }
    }

    unsigned calc_temp_8px(double temperature)
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

    void set_pixel(uint16_t index, RgbColor color)
    {
        pixels_.SetPixelColor(index, GammaTable::apply(color));
    }

    Heater& heater_;
    Pixels& pixels_;
    Timer updateTimer_;
    Subscription clicked_;
    uint8_t powerIndex_{};
};
#endif // ESP8266_IOT_CONTROLLER_HPP