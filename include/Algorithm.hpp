#ifndef ESP8266_IOT_ALGORITHM_HPP
#define ESP8266_IOT_ALGORITHM_HPP

#include <algorithm>
#include <type_traits>

template<typename T>
auto clamp(T value, T min, T max)
-> typename std::enable_if<std::is_arithmetic<T>::value, T>::type
{
    return std::max(min, std::min(max, value));
}

template<typename T, typename U>
auto map(T value, T inMin, T inMax, U outMin, U outMax)
-> typename std::enable_if<std::is_arithmetic<T>::value && std::is_arithmetic<U>::value, U>::type
{
    return value < inMin ? outMin :
           value >= inMax ? outMax :
           (value - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

#endif // ESP8266_IOT_ALGORITHM_HPP