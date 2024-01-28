#include <utility>

#include "IoT.hpp"
#include "PushButton.hpp"

PushButton::PushButton(Handler input) noexcept
        : input_(std::move(input)),
          updateTimer_(updateDelay, true, [this] { update(); }),
          expiredTimer_([this] { expired(); })
{
}

void PushButton::update()
{
    bool value = input_();
    if (value_ != value) {
        value_ = value;

        if (value_) {
            expiredTimer_.start(1000);
        } else if (!finished_) {
            ++clicks_;
            expiredTimer_.start(200);
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
