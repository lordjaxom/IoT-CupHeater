#include "Algorithm.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Output.hpp"

double Heater::increasePreset(double value)
{
    value = std::floor(value / setpointStep) * setpointStep + setpointStep;
    return value > maximumSetpoint ? minimumSetpoint : value;
}

Heater::Heater(Input input, Output output) noexcept
        : input_(std::move(input)),
          output_(std::move(output)),
          connected_(IoT.connectEvent.subscribe([this] { connected(); })),
          updateTimer_(updateDelay, true, [this] { update(); }),
          telemetryTimer_(telemetryDelay, true, [this] { publishTelemetry(); })
{
    IoT.subscribeCommand("SETPOINT", [this](String const& payload) { set(payload.toDouble()); });
}

double Heater::preset() const
{
    return IoT.settings().presetSetpoint();
}

void Heater::set(double value)
{
    if (value > 0.0) {
        value = clamp(value, minimumSetpoint, maximumSetpoint);
    }

    setpoint_ = value;
    if (value > 0) {
        IoT.settings().presetSetpoint(value);
    }
    publishState();
}

void Heater::toggle()
{
    set(setpoint_ > 0 ? 0 : IoT.settings().presetSetpoint());
}

void Heater::preset(double value)
{
    value = clamp(value, minimumSetpoint, maximumSetpoint);
    IoT.settings().presetSetpoint(value);
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
