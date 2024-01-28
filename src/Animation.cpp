#include "Animation.hpp"

PulseAnimation::PulseAnimation(RgbColor left, RgbColor right, uint32_t runtime) noexcept
        : left_(left),
          right_(right),
          runtime_(runtime)
{
}

RgbColor PulseAnimation::operator()(uint32_t elapsed)
{
    float factor = position_ < runtime_ / 2 ? position_ / (runtime_ / 2.f) : (runtime_ - position_) / (runtime_ / 2.f);
    position_ = (position_ + elapsed) % runtime_;
    return RgbColor::LinearBlend(left_, right_, factor);
}
