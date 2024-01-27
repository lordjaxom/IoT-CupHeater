#include <DallasTemperature.h>
#include <OneWire.h>

#include "Controller.hpp"
#include "Debounce.hpp"
#include "Gpio.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Output.hpp"
#include "PushButton.hpp"

static double thermistor_read();

//@formatter:off
static char const*        wifi_ssid        = "VillaKunterbunt";
static char const*        wifi_password    = "sacomoco02047781";
static char const*        mqtt_server      = "openhab";
static char const*        mqtt_base_topic  = "Workshop/Appliance/CupHeater";

static constexpr uint8_t  thermistor_port  = 4;
static constexpr uint8_t  heatbed_port     = 16;
static constexpr uint8_t  button_port      = 14;

using Pixels = NeoPixelBus<NeoGrbwFeature, NeoEsp8266AsyncUart1800KbpsMethod>;

OneWire                   oneWire(thermistor_port);
DallasTemperature         thermistor(&oneWire);
Pixels                    pixels( 10 );

IoTClass                  IoT(mqtt_base_topic, wifi_ssid, wifi_password, mqtt_server);
PushButton                button(debounce(gpioInput(button_port)));
Output                    heatbed("POWER", gpioOutput(heatbed_port));
Heater                    heater(thermistor_read, [](bool value) { heatbed.set(value); });
Controller<Pixels>        controller(button, heater, pixels);
//@formatter:on

static double thermistor_read()
{
    thermistor.requestTemperatures();
    return thermistor.getTempCByIndex(0);
}

void setup()
{
    IoT.begin();
}

void loop()
{
    IoT.loop();
}
