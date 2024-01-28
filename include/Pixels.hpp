#ifndef ESP8266_IOT_PIXELS_HPP
#define ESP8266_IOT_PIXELS_HPP

#include <memory>

#include <NeoPixelBus.h>

#include "GammaTable.hpp"

namespace Color
{
    static RgbColor red{255, 0, 0};
    static RgbColor lightred{255, 64, 64};
    static RgbColor darkred{100, 0, 0};
    static RgbColor faintred{45, 0, 0};
    static RgbColor green{0, 255, 0};
    static RgbColor blue{0, 0, 255};
    static RgbColor yellow{255, 255, 0};
    static RgbColor magenta{255, 0, 255};
    static RgbColor cyan{0, 255, 255};
    static RgbColor white{255, 255, 255};
    static RgbColor black{0, 0, 0};
}

namespace detail
{
    class Pixels
    {
    public:
        virtual ~Pixels() = default;

        virtual void Begin() = 0;
        virtual void ClearTo(RgbColor const& color) = 0;
        virtual void Show() = 0;

        /**
         * Set the brighness of all pixels
         */
        virtual void SetBrightness(uint8_t brightness) = 0;

        /**
         * Set the pixel color with gamma and brightness correction
         */
        virtual void SetPixelColor(uint16_t index, RgbColor const& color) = 0;
    };

    template<typename Feature, typename Method>
    class PixelsImpl : public Pixels {
    public:
        explicit PixelsImpl(uint16_t count) noexcept
                : pixels_(count) {}

        void Begin() override { pixels_.Begin(); }
        void ClearTo(RgbColor const& color) override { pixels_.ClearTo(color); }
        void Show() override { pixels_.Show(); }

        void SetBrightness(uint8_t brightness) override
        {
            brightness_ = brightness;
        }

        /**
         * Set the pixel color with gamma and brightness correction
         */
        void SetPixelColor(uint16_t index, RgbColor const& color) override
        {
            pixels_.SetPixelColor(index, GammaTable::apply(color).Dim(brightness_));
        }

    private:
        NeoPixelBus<Feature, Method> pixels_;
        uint8_t brightness_{255};
    };
}

template<typename Feature, typename Method>
struct NeoPixels {};

class Pixels
{
public:
    template<typename Feature, typename Method>
    Pixels(NeoPixels<Feature, Method>, uint16_t countPixels) noexcept
            : impl_(new detail::PixelsImpl<Feature, Method>(countPixels)) {}

    void Begin() { impl_->Begin(); }
    void ClearTo(RgbColor const& color) { impl_->ClearTo(color); }
    void Show() { impl_->Show(); }
    void SetPixelColor(uint16_t index, RgbColor const& color) { impl_->SetPixelColor(index, color); }
    void SetBrightness(uint8_t brightness) { impl_->SetBrightness(brightness); }

private:
    std::unique_ptr<detail::Pixels> impl_;
};

#endif // ESP8266_IOT_PIXELS_HPP