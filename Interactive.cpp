#include "Interactive.h"

Interactive::Interactive()
    : hovered{ false }
    , pressed{ false }
    , onHoverBehavior{ nullptr }
    , onPressBehavior { nullptr }
    , trigger{ nullptr }
    , behaviorContextData{ nullptr }
{
    interactives.push_back(this);
}

Interactive::~Interactive()
{
    for (size_t i{ 0 }; i < interactives.size(); i++)
    {
        if (interactives[i] == this)
        {
            interactives.erase(interactives.begin() + i);
            break;
        }
    }
}

void Interactive::setHoverBehavior(void (*hoverBehavior)(Direction, void*))
{
    onHoverBehavior = hoverBehavior;
}

void Interactive::setPressBehavior(void (*pressBehavior)(Direction, void*))
{
    onPressBehavior = pressBehavior;
}

void Interactive::setAction(void (*action)(void*))
{
    trigger = action;
}

void Interactive::setBehaviorContextData(void* behaviorContextData)
{
    this->behaviorContextData = behaviorContextData;
}

void* Interactive::getBehaviorContextData()
{
    return behaviorContextData;
}

void Interactive::onHover(int x, int y)
{
    if (!hovered && getGlobalBounds().contains((float)x, (float)y))
    {
        if (onHoverBehavior) onHoverBehavior(Direction::In, behaviorContextData);
        hovered = true;
    }
    else if (hovered && !getGlobalBounds().contains((float)x, (float)y))
    {
        if (onHoverBehavior) onHoverBehavior(Direction::Out, behaviorContextData);
        hovered = false;
    }
}

void Interactive::onPress()
{
    if (!pressed && hovered)
    {
        if (onPressBehavior) onPressBehavior(Direction::In, behaviorContextData);
        pressed = true;
    }
}

void Interactive::onRelease()
{
    if (pressed)
    {
        if (onPressBehavior) onPressBehavior(Direction::Out, behaviorContextData);
        pressed = false;
        if (hovered)
        {
            if (trigger) trigger(behaviorContextData);
        }
    }
}

bool Interactive::isHovered() const
{
    return hovered;
}

bool Interactive::isPressed() const
{
    return pressed;
}

void Interactive::updateInteractives(Event event, int mouseX, int mouseY)
{
    for (Interactive* interactive : interactives)
    {
        if (!interactive->isVisible()) continue;

        switch (event)
        {
            case Interactive::Event::Hover:
            {
                interactive->onHover(mouseX, mouseY);
                break;
            }
            case Interactive::Event::Press:
            {
                interactive->onPress();
                break;
            }
            case Interactive::Event::Release:
            {
                interactive->onRelease();
                break;
            }
        }
    }
}

std::vector<Interactive*> Interactive::interactives;