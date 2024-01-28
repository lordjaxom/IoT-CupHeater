#ifndef ESP8266_IOT_IOT_HPP
#define ESP8266_IOT_IOT_HPP

#include <stdint.h>
#include <functional>
#include <map>

#include <AsyncMqttClient.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#include "Event.hpp"
#include "Timer.hpp"
#include "String.hpp"

class IoTClass
{
    static constexpr uint32_t watchdogDelay = 60000;
    static constexpr uint32_t reconnectDelay = 1000;

    using Subscriber = std::function<void(String payload)>;

public:
    static constexpr uint32_t tick = 10;

    IoTClass(char const* topic, char const* wiFiSsid, char const* wiFiPassword, char const* mqttIp,
             uint16_t mqttPort = 1883) noexcept;
    IoTClass(IoTClass const&) = delete;

    String const& clientId() const { return clientId_; }
    String const& topic() const { return topic_; }

    void begin();
    void loop();

    void publish(String const& topic, String const& payload, bool retain = false);
    void publish(String const& topic, char const* payload, bool retain = false);
    void subscribe(String topic, Subscriber handler);

    template<typename T>
    void publishState(char const* state, T&& payload)
    {
        publish(str("stat/", topic_, "/", state), std::forward<T>(payload));
    }

    template<typename T>
    void publishTelemetry(char const* state, T&& payload)
    {
        publish(str("tele/", topic_, "/", state), std::forward<T>(payload));
    }

    void subscribeCommand(char const* command, Subscriber handler);

    Event<void()> beginEvent;
    Event<void()> connectEvent;
    Event<void()> loopEvent;

private:
    void connectToWiFi();
    void connectToMqtt();

    void wiFiConnected();
    void wiFiDisconnected();

    void mqttConnected();
    void mqttDisconnected();
    void mqttMessage(char const* topic, char const* payload, size_t length);
    void mqttSubscribe(String const& topic);

    String const topic_; // must stay constant
    String const clientId_; // must stay constant
    String const hostname_; // must stay constant
    Timer watchdogTimer_;

    char const* wiFiSsid_;
    char const* wiFiPassword_;
    WiFiEventHandler wiFiConnectHandler_;
    WiFiEventHandler wiFiDisconnectHandler_;
    Timer wiFiReconnectTimer_;

    char const* mqttIp_;
    uint16_t mqttPort_;
    String const mqttWillTopic_; // must stay constant
    Timer mqttReconnectTimer_;
    AsyncMqttClient mqttClient_;
    std::multimap<String, Subscriber> mqttSubscriptions_;

    ESP8266WebServer webServer_;
    ESP8266HTTPUpdateServer updateServer_;

    uint32_t timestamp_{};
};

extern IoTClass IoT;

#endif // ESP8266_IOT_IOT_HPP
