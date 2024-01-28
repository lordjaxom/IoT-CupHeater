#ifndef ESP8266_IOT_CONTROLLER_HPP
#define ESP8266_IOT_CONTROLLER_HPP

#include <NeoPixelBus.h>

#include "ColorWheel.hpp"
#include "Event.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Pixels.hpp"
#include "PushButton.hpp"
#include "Timer.hpp"
#include "Logger.hpp"

namespace detail
{
    struct FadeAnimation
    {
        static constexpr uint8_t minimum = 45;
        static constexpr uint8_t maximum = 255;
        static constexpr uint8_t step = 10;

        static_assert(minimum < maximum, "minimum must be less than maximum");

        RgbColor next()
        {
            auto last = static_cast<uint8_t>(current_);
            current_ += (direction_ ? 1 : -1) * step;
            if (current_ <= minimum || current_ >= maximum) {
                if (current_ < minimum) {
                    current_ = minimum;
                } else if (current_ > maximum) {
                    current_ = maximum;
                }
                direction_ = !direction_;
            }
            return {last, 0, 0};
        }

        int current_{minimum};
        bool direction_{true};
    };
}

class Controller
{
    static constexpr uint32_t updateDelay = 100;
    static constexpr double hysteresis = 1.5;

public:
    Controller(PushButton& button, Heater& heater, Pixels& pixels) noexcept;

private:
    void begin();
    void update();
    void clicked(unsigned clicks);

    Heater& heater_;
    Pixels& pixels_;
    Timer updateTimer_;
    Subscription clicked_;
    detail::FadeAnimation fadeAnimation_;
};
#endif // ESP8266_IOT_CONTROLLER_HPP