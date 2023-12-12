#include "Animation.h"

Animation::Animation()
    : progress{ 0.0 }
    , totalLength { 0.0 }
    , target{ nullptr }
{
}

Animation::Animation(DynamicObject* target) 
    : target(target)
    , progress{ 0.0 }
    , totalLength{ 0.0 }
{
}

void Animation::setTarget(DynamicObject* target)
{
    this->target = target;
}

void Animation::addFrame(Frame frame)
{
    totalLength += frame.duration;
    frames.push_back(frame);
}

void Animation::update(double elapsed)
{
    progress += elapsed;
    double p{ progress };
    for (size_t i{ 0 }; i < frames.size(); i++)
    {
        p -= frames[i].duration;

        if (p > 0.0 && &(frames[i]) == &(frames.back()))
        {
            i = 0;
            continue;
        }

        if (p <= 0.0 || &(frames[i]) == &frames.back())
        {
            target->setTexture(frames[i].texutre);
            break;
        }
    }
}