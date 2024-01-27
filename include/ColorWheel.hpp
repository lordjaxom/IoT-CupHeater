#ifndef ESP8266_IOT_COLORWHEEL_HPP
#define ESP8266_IOT_COLORWHEEL_HPP

#include <cstdint>
#include <cmath>

#include <NeoPixelBus.h>

#include "ValueTable.hpp"

namespace detail {

    static constexpr double pi = 3.14159265359;

    template< std::size_t N >
    struct ColorWheelGenerator
    {
        static constexpr std::uint32_t apply( std::size_t input )
        {
            return ( r( input ) << 16 ) |
                   ( g( input ) << 8 ) |
                   ( b( input ) << 0 );
        }

    private:
        static constexpr size_t i( size_t input ) { return input * 3 / N; }

        static constexpr double f( size_t input ) { return pi * ( 3.0 * input - i( input ) * N ) / ( 2.0 * N ); }

        static constexpr int s( size_t input ) { return (int) ( 255.0 * std::sin( f( input ))); }

        static constexpr int c( size_t input ) { return (int) ( 255.0 * std::cos( f( input ))); }

        static constexpr uint8_t r( size_t input )
        {
            return (uint8_t) (( i( input ) == 0 ? 1 : 0 ) * s( input ) + ( i( input ) == 1 ? 1 : 0 ) * c( input ));
        }

        static constexpr uint8_t g( size_t input )
        {
            return (uint8_t) (( i( input ) == 1 ? 1 : 0 ) * s( input ) + ( i( input ) == 2 ? 1 : 0 ) * c( input ));
        }

        static constexpr uint8_t b( size_t input )
        {
            return (uint8_t) (( i( input ) == 2 ? 1 : 0 ) * s( input ) + ( i( input ) == 0 ? 1 : 0 ) * c( input ));
        }
    };

} // namespace detail

template< std::size_t N >
struct BasicColorWheel
        : ValueTable< detail::ColorWheelGenerator< N >, 0, N >
{
    using Base = ValueTable< detail::ColorWheelGenerator< N >, 0, N >;

    static RgbColor get( std::size_t index )
    {
        auto value = Base::get(index);
        return {
            static_cast<uint8_t>((value >> 16) & 0xff),
            static_cast<uint8_t>((value >> 8) & 0xff),
            static_cast<uint8_t>((value >> 0) & 0xff)
        };
    }
};

using ColorWheel = BasicColorWheel< 256 >;

#endif // ESP8266_IOT_COLORWHEEL_HPP
