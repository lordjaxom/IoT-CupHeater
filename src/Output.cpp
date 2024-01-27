#include "IoT.hpp"
#include "Output.hpp"
#include "String.hpp"

Output::Output(const char* state, Handler output) noexcept
        : Output(state, "OFF", "ON", std::move(output))
{
}

Output::Output(const char* state, const char* falseValue, const char* trueValue, Handler output) noexcept
        : state_(state),
          falseValue_(falseValue),
          trueValue_(trueValue),
          output_(std::move(output)),
          connected_(IoT.connectEvent.subscribe([this] { publishState(); }))
{
}

void Output::set(bool value)
{
    if (value_ == value) {
        return;
    }
    value_ = value;
    if (output_) {
        output_(value_);
    }
    publishState();
}

void Output::publishState()
{
    IoT.publishState(state_, value_ ? trueValue_ : falseValue_);
}
