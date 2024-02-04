#ifndef ESP8266_IOT_SETTINGS_HPP
#define ESP8266_IOT_SETTINGS_HPP

class Settings
{
    static constexpr size_t eepromSize = 256;
    static constexpr int eepromVersion = 1;

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