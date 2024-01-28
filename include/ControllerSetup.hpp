#ifndef ESP8266_IOT_CONTROLLERMODESETUP_HPP
#define ESP8266_IOT_CONTROLLERMODESETUP_HPP

#include "Animation.hpp"
#include "Controller.hpp"

class ControllerModeSetup : public ControllerMode
{
public:
    explicit ControllerModeSetup(Controller& c) noexcept;

    void update(uint32_t elapsed) override;
    void clicked(unsigned clicks) override;
    ControllerMode* next() override;

private:
    Controller& c_;
    PulseAnimation powerAnimation_;
};

#endif // ESP8266_IOT_CONTROLLERMODESETUP_HPP