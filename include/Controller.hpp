#ifndef ESP8266_IOT_CONTROLLER_HPP
#define ESP8266_IOT_CONTROLLER_HPP

#include <NeoPixelBus.h>

#include "Event.hpp"
#include "Timer.hpp"

class Heater;
class Pixels;
class PushButton;

class ControllerMode
{
public:
    virtual ~ControllerMode() = default;

    virtual void update(uint32_t elapsed) = 0;
    virtual void clicked(unsigned clicks) = 0;
    virtual std::unique_ptr<ControllerMode> next() = 0;
};

class Controller
{
    static constexpr uint32_t updateDelay = 100;
    static constexpr double hysteresis = 1.5;

    friend class ControllerModeNormal;
    friend class ControllerModeSetup;

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
    std::unique_ptr<ControllerMode> mode_;
};
#endif // ESP8266_IOT_CONTROLLER_HPP