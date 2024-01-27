#ifndef ESP8266_IOT_ALGORITHM_HPP
#define ESP8266_IOT_ALGORITHM_HPP

#include <algorithm>

template<typename T>
T clamp(T const& value, T const& min, T const& max)
{
    return std::max(min, std::min(max, value));
}

#endif // ESP8266_IOT_ALGORITHM_HPP