#ifndef ESP8266_IOT_ALGORITHM_HPP
#define ESP8266_IOT_ALGORITHM_HPP

#include <algorithm>

template<typename T>
T clamp(T const& value, T const& min, T const& max)
{
    return std::max(min, std::min(max, value));
}

template<typename T, typename U>
U map(T const& value, T const& inMin, T const& inMax, U const& outMin, U const& outMax)
{
    return value < inMin ? outMin
            : value >= inMax ? outMax
            : (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

#endif // ESP8266_IOT_ALGORITHM_HPP