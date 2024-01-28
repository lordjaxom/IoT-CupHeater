#ifndef ESP8266_IOT_CONTROLLERMODENORMAL_HPP
#define ESP8266_IOT_CONTROLLERMODENORMAL_HPP

#include "Controller.hpp"

class ControllerModeNormal : public ControllerMode
{
public:
    explicit ControllerModeNormal(Controller& c) noexcept;

    void update(uint32_t elapsed) override;
    void clicked(unsigned clicks) override;
    ControllerMode* next() override;

private:
    Controller& c_;
    std::function<RgbColor(uint32_t elapsed)> heatingAnimation_;
};

#endif // ESP8266_IOT_CONTROLLERMODENORMAL_HPP