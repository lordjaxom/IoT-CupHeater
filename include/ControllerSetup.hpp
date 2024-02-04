#ifndef ESP8266_IOT_CONTROLLERMODESETUP_HPP
#define ESP8266_IOT_CONTROLLERMODESETUP_HPP

#include "Animation.hpp"
#include "Controller.hpp"
#include "Timer.hpp"

class ControllerModeSetup : public ControllerMode
{
    static constexpr uint32_t timeoutDelay = 10000;

public:
    explicit ControllerModeSetup(Controller& c) noexcept;

    void update(uint32_t elapsed) override;
    void clicked(unsigned clicks) override;
    void next() override;

private:
    Controller& c_;
    PulseAnimation powerAnimation_;
    double presetSetpoint_;
    Timer timeoutTimer_;
};

#endif // ESP8266_IOT_CONTROLLERMODESETUP_HPP