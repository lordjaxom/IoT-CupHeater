#ifndef ESP8266_IOT_CONTROLLER_HPP
#define ESP8266_IOT_CONTROLLER_HPP

#include <NeoPixelBus.h>

#include "Event.hpp"
#include "GammaTable.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Timer.hpp"

template<typename Pixels>
class Controller
{
    static constexpr uint32_t updateDelay = 100;
    static constexpr double hysteresis = 1.5;

public:
    Controller(Heater& heater, Pixels& pixels) noexcept
            : heater_(heater),
              pixels_(pixels),
              updateTimer_(updateDelay, true, [this] { update(); })
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
        set_pixel(0, RgbColor(0, 0, 255));

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
            if (value > 255) {
                set_pixel(2 + i, RgbColor(255, 0, 0));
                value -= 255;
            } else {
                set_pixel(2 + i, RgbColor(value, 0, 0));
                value = 0;
            }
        }

        pixels_.Show();
    }

    uint16_t calc_temp_8px(double temperature)
    {
        static constexpr double minimum = 25.0;
        static constexpr double maximum = 100.0;
        static constexpr uint16_t pixels = 8;

        if (temperature < minimum) {
            return 0;
        }
        if (temperature >= maximum) {
            return pixels;
        }
        return static_cast<uint16_t>((temperature - minimum) * pixels * 256 / (maximum - minimum));
    }

    void set_pixel(uint16_t index, RgbColor color)
    {
        pixels_.SetPixelColor(index, GammaTable::apply(color));
    }

    Heater& heater_;
    Pixels& pixels_;
    Timer updateTimer_;
};
#endif // ESP8266_IOT_CONTROLLER_HPP