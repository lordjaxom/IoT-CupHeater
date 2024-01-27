#ifndef ESP8266_IOT_OUTPUT_HPP
#define ESP8266_IOT_OUTPUT_HPP

#include <Arduino.h>

#include <functional>

#include "Event.hpp"

class Output
{
    using Handler = std::function<void(bool value)>;

public:
    explicit Output(char const* state, Handler output = nullptr) noexcept;
    Output(char const* state, char const* falseValue, char const* trueValue, Handler output = nullptr) noexcept;
    Output(Output const&) = delete;

    bool get() const { return value_; }

    void set(bool value);

    void toggle() { set(!value_); }

private:
    void publishState();

    char const* state_;
    char const* falseValue_;
    char const* trueValue_;
    Handler output_;
    Subscription connected_;
    bool value_{};
};

#endif // ESP8266_IOT_OUTPUT_HPP