#ifndef ESP8266_IOT_PIXELS_HPP
#define ESP8266_IOT_PIXELS_HPP

#include <memory>

#include <NeoPixelBus.h>

#include "GammaTable.hpp"

namespace Color
{
    inline RgbColor red() { return {255, 0, 0}; }
    inline RgbColor lightred() { return {255, 64, 64}; }
    inline RgbColor darkred() { return {100, 0, 0}; }
    inline RgbColor faintred() { return {45, 0, 0}; }
    inline RgbColor green() { return {0, 255, 0}; }
    inline RgbColor blue() { return {0, 0, 255}; }
    inline RgbColor yellow() { return {255, 255, 0}; }
    inline RgbColor magenta() { return {255, 0, 255}; }
    inline RgbColor cyan() { return {0, 255, 255}; }
    inline RgbColor white() { return {255, 255, 255}; }
    inline RgbColor black() { return {0, 0, 0}; }
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
        virtual uint16_t PixelsCount() const = 0;

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
    class PixelsImpl : public Pixels
    {
    public:
        explicit PixelsImpl(NeoPixelBus<Feature, Method>& neoPixelBus) noexcept
                : pixels_(neoPixelBus) {}

        void Begin() override { pixels_.Begin(); }
        void ClearTo(RgbColor const& color) override { pixels_.ClearTo(color); }
        void Show() override { pixels_.Show(); }
        uint16_t PixelsCount() const override { return pixels_.PixelCount(); }

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
        NeoPixelBus<Feature, Method>& pixels_;
        uint8_t brightness_{255};
    };
}

class Pixels
{
public:
    template<typename Feature, typename Method>
    explicit Pixels(NeoPixelBus<Feature, Method>& neoPixelBus) noexcept
            : impl_(new detail::PixelsImpl<Feature, Method>(neoPixelBus)) {}

    void Begin() { impl_->Begin(); }
    void ClearTo(RgbColor const& color) { impl_->ClearTo(color); }
    void Show() { impl_->Show(); }
    uint16_t PixelsCount() const { return impl_->PixelsCount(); }
    void SetPixelColor(uint16_t index, RgbColor const& color) { impl_->SetPixelColor(index, color); }
    void SetBrightness(uint8_t brightness) { impl_->SetBrightness(brightness); }

private:
    std::unique_ptr<detail::Pixels> impl_;
};

#endif // ESP8266_IOT_PIXELS_HPP