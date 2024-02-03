#include "Algorithm.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Output.hpp"

Heater::Heater(Input input, Output output) noexcept
        : input_(std::move(input)),
          output_(std::move(output)),
          connected_(IoT.connectEvent.subscribe([this] { connected(); })),
          updateTimer_(updateDelay, true, [this] { update(); }),
          telemetryTimer_(telemetryDelay, true, [this] { publishTelemetry(); })
{
    IoT.subscribeCommand("SETPOINT", [this](String const& payload) { set(payload.toDouble()); });
}

void Heater::set(double value)
{
    if (value > 0.0) {
        value = clamp(value, minimumSetpoint, maximumSetpoint);
    }

    setpoint_ = value;
    if (value > 0) {
        preset_ = value;
    }
    publishState();
}

void Heater::preset(double value)
{
    preset_ = clamp(value, minimumSetpoint, maximumSetpoint);
}

void Heater::increasePreset()
{
    preset_ = std::floor(preset_ / setpointStep) * setpointStep + setpointStep;
    if (preset_ > maximumSetpoint) {
        preset_ = minimumSetpoint;
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
    output_(setpoint_ > 0 && temperature_ < setpoint_);
}

void Heater::publishState() const
{
    IoT.publishState("SETPOINT", String(setpoint_));
}

void Heater::publishTelemetry() const
{
    IoT.publishTelemetry("TEMPERATURE", String(temperature_));
}
