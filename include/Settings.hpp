#ifndef ESP8266_IOT_SETTINGS_HPP
#define ESP8266_IOT_SETTINGS_HPP

class Settings
{
public:
    Settings() = default;
    Settings(Settings const&) = delete;

    double presetSetpoint() const { return presetSetpoint_; }
    void presetSetpoint(double value);

    void begin() { loadEeprom(); }

private:
    void loadEeprom();
    void saveEeprom();

    double presetSetpoint_{60.0};
};

#endif // ESP8266_IOT_SETTINGS_HPP