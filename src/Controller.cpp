#include "Controller.hpp"
#include "ControllerNormal.hpp"
#include "Heater.hpp"
#include "IoT.hpp"
#include "Pixels.hpp"
#include "PushButton.hpp"

Controller::Controller(PushButton &button, Heater &heater, Pixels &pixels) noexcept
        : heater_(heater),
          pixels_(pixels),
          updateTimer_(updateDelay, true, [this] { update(); }),
          clicked_(button.clickedEvent.subscribe([this](unsigned clicks) { clicked(clicks); })),
          mode_(new ControllerModeNormal(*this))
{
    IoT.beginEvent += [this] { begin(); };
}

void Controller::begin()
{
    pixels_.SetBrightness(128);
    pixels_.Begin();
    update();
}

void Controller::update()
{
    pixels_.ClearTo(RgbColor(0, 0, 0));
    mode_->update(updateDelay);
    pixels_.Show();
}

void Controller::clicked(unsigned clicks)
{
    if (clicks == 0) {
        mode_.reset(mode_->next());
    }
    mode_->clicked(clicks);
}
