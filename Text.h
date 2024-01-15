#pragma once
#include "DynamicObject.h"

class Text: public DynamicObject
{
public:

    Text(const sf::Vector2f& size = { 0.f, 0.f }, const std::string& text = "");
    void setSize(const sf::Vector2f& size);
    void setTextString(const sf::String& string);
    void setTextFont(const std::string& fontPath);
    void setTextSize(float percentage);
    void setTextFillColor(const sf::Color& color);
    void setTextOutlineColor(const sf::Color& color);
    void setTextOutlineThickness(float thickness);
    void setTextStyle(sf::Text::Style style);
    const sf::String& getTextString() const;
    const sf::Font* getTextFont() const;
    float getTextSize() const;
    const sf::Color& getTextFillColor() const;
    const sf::Color& getTextOutlineColor() const;
    float getTextOutlineThickness() const;
    sf::Text::Style getTextStyle() const;
    sf::FloatRect getTextLocalBounds() const;
    sf::FloatRect getTextGlobalBounds() const;

    Scheduler taskManager;

protected:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    void centerText();

    sf::Text text;
    sf::Font font;
    float relativeCharacterSize;
};
