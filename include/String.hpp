#ifndef ESP8266_IOT_STRING_HPP
#define ESP8266_IOT_STRING_HPP

namespace detail {

    inline void strAppend(String& result, char const* arg)
    {
        result += arg;
    }

    inline void strAppend(String& result, __FlashStringHelper const* arg)
    {
        result += arg;
    }

    inline void strAppend(String& result, String const& arg)
    {
        result += arg;
    }

    inline void str(String& result) {}

    template<typename Arg0, typename ...Args>
    void str(String& result, Arg0&& arg0, Args&& ...args)
    {
        strAppend(result, std::forward<Arg0>(arg0));
        str(result, std::forward<Args>(args)...);
    }

} // namespace detail

template<typename ...Args>
String str(Args&& ... args)
{
    String result;
    detail::str(result, std::forward<Args>(args)...);
    return result;
}

#endif //ESP8266_IOT_STRING_HPP
