#include <EEPROM.h>

#include "Settings.hpp"

void Settings::loadEeprom()
{
    EEPROM.begin(eepromSize);

    int address = 0;
    int version;
    EEPROM.get(address, version);
    address += sizeof(version);
    if (version >= 1) {
        EEPROM.get(address, presetSetpoint_);
        address += sizeof(presetSetpoint_);
    }
    EEPROM.end();
}

void Settings::saveEeprom()
{
    EEPROM.begin(eepromSize);

    int address = 0;
    int version = eepromVersion;
    EEPROM.put(address, version);
    address += sizeof(version);
    EEPROM.put(address, presetSetpoint_);
    address += sizeof(presetSetpoint_);
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
