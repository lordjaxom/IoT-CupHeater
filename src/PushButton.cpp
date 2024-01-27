#include <utility>

#include "IoT.hpp"
#include "PushButton.hpp"

PushButton::PushButton(Handler input) noexcept
        : input_(std::move(input)),
          looped_(IoT.loopEvent.subscribe([this] { loop(); })),
          timer_([this] { expired(); })
{
}

void PushButton::loop()
{
    bool value = input_();
    if (value_ != value) {
        value_ = value;

        if (value_) {
            timer_.start(1000);
        } else if (!finished_) {
            ++clicks_;
            timer_.start(200);
        } else {
            finished_ = false;
        }
    }
}

void PushButton::expired()
{
    if (value_) {
        finished_ = true;
        if (clicks_ == 0) {
            clickedEvent(0);
        }
    } else {
        clickedEvent(clicks_);
    }
    clicks_ = 0;
}
