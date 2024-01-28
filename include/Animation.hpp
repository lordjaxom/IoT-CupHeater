#ifndef ESP8266_IOT_ANIMATION_HPP
#define ESP8266_IOT_ANIMATION_HPP

#include <NeoPixelBus.h>

class PulseAnimation
{
public:
    PulseAnimation(RgbColor left, RgbColor right, uint32_t runtime) noexcept;

    RgbColor animate(uint32_t elapsed);

private:
    RgbColor left_;
    RgbColor right_;
    uint32_t runtime_;
    uint32_t position_{};
};

#endif // ESP8266_IOT_ANIMATION_HPP