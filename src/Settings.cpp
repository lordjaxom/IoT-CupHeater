#include <EEPROM.h>

#include "Settings.hpp"

namespace
{
    constexpr size_t eepromSize = 256;
    constexpr int eepromVersion = 1;

    template<typename T>
    T readEeprom(int& address)
    {
        T value;
        EEPROM.get(address, value);
        address += sizeof(value);
        return value;
    }

    template<typename T>
    void writeEeprom(int& address, T value)
    {
        EEPROM.put(address, value);
        address += sizeof(value);
    }
}

void Settings::loadEeprom()
{
    EEPROM.begin(eepromSize);

    int address = 0;
    int version = readEeprom<int>(address);
    if (version >= 1) {
        presetSetpoint_ = readEeprom<double>(address);
    }
    EEPROM.end();
}

void Settings::saveEeprom() const
{
    EEPROM.begin(eepromSize);

    int address = 0;
    writeEeprom(address, eepromVersion);
    writeEeprom(address, presetSetpoint_);

    EEPROM.commit();
    EEPROM.end();
}

void Settings::presetSetpoint(double value)
{
    if (presetSetpoint_ != value) {
        presetSetpoint_ = value;
        saveEeprom();
    }
}
