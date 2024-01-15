#pragma once
#include "Text.h"
#include "Interactive.h"
#include "Scheduler.h"

class Button: public Text, public Interactive
{
public:

    Button(const sf::Vector2f& size = { 0.f, 0.f });
    sf::FloatRect getGlobalBounds() const;

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};