#include <utility>

#include "IoT.hpp"
#include "Timer.hpp"

using namespace std;

Timer::Timer(Handler handler) noexcept
        : Timer(0, std::move(handler))
{
}

Timer::Timer(uint32_t timeout, Handler handler) noexcept
        : Timer(timeout, false, std::move(handler))
{
}

Timer::Timer(uint32_t timeout, bool repeat, Handler handler) noexcept
        : handler_(std::move(handler)),
          looped_(IoT.loopEvent.subscribe([this] { loop(); }))
{
    start(timeout, repeat);
}

void Timer::start(uint32_t timeout, bool repeat)
{
    remaining_ = timeout;
    nextTimeout_ = repeat ? timeout : 0;
}

void Timer::stop()
{
    remaining_ = 0;
}

void Timer::loop()
{
    if (remaining_ > IoTClass::tick) {
        remaining_ -= IoTClass::tick;
    } else if (remaining_ > 0) {
        remaining_ = nextTimeout_;
        handler_();
    }
}
