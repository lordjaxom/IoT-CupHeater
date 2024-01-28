#ifndef ESP8266_IOT_GAMMATABLE_HPP
#define ESP8266_IOT_GAMMATABLE_HPP

#include <cmath>
#include <cstdint>
#include <limits>
#include <ratio>

#include <NeoPixelBus.h>

#include "ValueTable.hpp"

namespace detail
{

    template<typename Gamma>
    struct GammaGenerator
    {
        static constexpr std::uint8_t max = std::numeric_limits<uint8_t>::max();

        static constexpr std::uint8_t apply(std::uint8_t input)
        {
            return std::pow((double) input / max, (1.0 * Gamma::den / Gamma::num)) * max;
        }
    };

} // namespace detail

template<typename Gamma>
struct BasicGammaTable
        : ValueTable<detail::GammaGenerator<Gamma> >
{
    using Base = ValueTable<detail::GammaGenerator<Gamma> >;

    static RgbColor apply(RgbColor color)
    {
        return {Base::get(color.R), Base::get(color.G), Base::get(color.B)};
    }

    static constexpr std::uint8_t apply(std::uint8_t input)
    {
        return Base::get(input);
    }
};

using GammaTable = BasicGammaTable<std::ratio<4, 10> >;

#endif // ESP8266_IOT_GAMMATABLE_HPP
