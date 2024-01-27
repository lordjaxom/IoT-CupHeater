#include "Algorithm.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Output.hpp"

Heater::Heater(Input input, Output& output) noexcept
        : input_(std::move(input)),
          output_(output),
          connected_(IoT.connectEvent.subscribe([this] { connected(); })),
          updateTimer_(updateDelay, true, [this] { update(); }),
          telemetryTimer_(telemetryDelay, true, [this] { publishTelemetry(); })
{
    IoT.subscribeCommand("SETPOINT", [this](String const& payload) { set(payload.toDouble()); });
}

void Heater::set(double value)
{
    value = clamp(value, minimumSetpoint, maximumSetpoint);

    setpoint_ = value;
    if (value > 0) {
        defaultSetpoint_ = value;
    }
    publishState();
}

void Heater::toggle()
{
    if (setpoint_ > 0) {
        set(0);
    } else {
        set(defaultSetpoint_);
    }
}

void Heater::connected()
{
    publishState();
    publishTelemetry();
}

void Heater::update()
{
    temperature_ = input_();
    if (setpoint_ > 0 && temperature_ < setpoint_) {
        if (!output_.get()) {
            output_.set(true);
        }
    } else if (output_.get()) {
        output_.set(false);
    }
}

void Heater::publishState() const
{
    IoT.publishState("SETPOINT", String(setpoint_));
}

void Heater::publishTelemetry() const
{
    IoT.publishTelemetry("TEMPERATURE", String(temperature_));
}
