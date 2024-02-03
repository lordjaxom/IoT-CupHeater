#ifndef ESP8266_IOT_HEATERCONTROL_HPP
#define ESP8266_IOT_HEATERCONTROL_HPP

#include <functional>

#include "Event.hpp"
#include "Output.hpp"
#include "Timer.hpp"

class Heater
{
    static constexpr double minimumSetpoint = 40.0;
    static constexpr double maximumSetpoint = 100.0;
    static constexpr double setpointStep = 10.0;

    static constexpr uint32_t updateDelay = 100;
    static constexpr uint32_t telemetryDelay = 1000;

    using Input = std::function<double()>;
    using Output = std::function<void(bool value)>;

public:
    Heater(Input input, Output output) noexcept;
    Heater(Heater const&) = delete;

    double get() const { return setpoint_; }
    double read() const { return temperature_; }
    double preset() const { return preset_; }

    void set(double value);
    void toggle() { set(setpoint_ > 0 ? 0 : preset_); }
    void preset(double value);
    void increasePreset();

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
    double setpoint_{};
    double temperature_{};
    double preset_{60.0};
};

#endif // ESP8266_IOT_HEATERCONTROL_HPP