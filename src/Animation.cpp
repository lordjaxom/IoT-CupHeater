#include "Animation.hpp"

PulseAnimation::PulseAnimation(RgbColor left, RgbColor right, uint32_t runtime) noexcept
        : left_(left),
          right_(right),
          runtime_(runtime)
{
}

RgbColor PulseAnimation::animate(uint32_t elapsed)
{
    auto position = static_cast<float>(position_);
    auto runtime = static_cast<float>(runtime_);
    float factor = position < runtime / 2.f ? position / (runtime / 2.f) : (runtime - position) / (runtime / 2.f);
    position_ = (position_ + elapsed) % runtime_;
    return RgbColor::LinearBlend(left_, right_, factor);
}
