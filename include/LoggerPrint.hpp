#ifndef ESP8266_IOT_LOGGERPRINT_HPP
#define ESP8266_IOT_LOGGERPRINT_HPP

#include <type_traits>

#include <Print.h>

#include "Logger.hpp"
#include "LoggerBase.hpp"

class LoggerPrint
        : public LoggerBase<LoggerPrint>
{
public:
    explicit LoggerPrint(Print& print) : print_(print) {}

    template<typename Arg>
    auto append(Arg&& arg)
    -> typename std::enable_if<std::is_same<decltype(std::declval<Print>().print(
            std::forward<Arg>(arg))), size_t>::value>::type
    {
        print_.print(std::forward<Arg>(arg));
    }

    template<typename Arg>
    auto append(Arg&& arg)
    -> typename std::enable_if<std::is_same<decltype(this->append(
            toString(std::forward<Arg>(arg)))), void>::value>::type
    {
        append(toString(std::forward<Arg>(arg)));
    }

    void append(detail::Timestamp const& ts)
    {
        print_.printf("%02lu:%02lu:%02lu.%03lu ", ts.h, ts.m, ts.s, ts.ms);
    }

    void flush()
    {
        print_.println();
    }

private:
    Print& print_;
};

#endif //ESP8266_IOT_LOGGERPRINT_HPP
