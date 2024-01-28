#ifndef ESP8266_IOT_HEATERCONTROL_HPP
#define ESP8266_IOT_HEATERCONTROL_HPP

#include <functional>

#include "Event.hpp"
#include "Timer.hpp"

class Output;

class Heater
{
    static constexpr double minimumSetpoint = 0.0;
    static constexpr double maximumSetpoint = 100.0;

    static constexpr uint32_t updateDelay = 100;
    static constexpr uint32_t telemetryDelay = 1000;

    using Input = std::function<double()>;
    using Output = std::function<void(bool value)>;

public:
    Heater(Input input, Output output) noexcept;
    Heater(Heater const&) = delete;

    double get() const { return setpoint_; }
    double read() const { return temperature_; }
    double defaultSetpoint() const { return defaultSetpoint_; }

    void set(double value);
    void toggle();

    void defaultSetpoint(double value) { defaultSetpoint_ = value; }

private:
    void connected();
    void update();
    void publishState() const;
    void publishTelemetry() const;

    Input input_;
    Output output_;
    Subscription connected_;
    Timer updateTimer_;
    Timer telemetryTimer_;
    double setpoint_ {};
    double temperature_ {};
    double defaultSetpoint_ {60.0};
};

#endif // ESP8266_IOT_HEATERCONTROL_HPP