#include "Text.h"

Text::Text(const sf::Vector2f& size, const std::string& text)
    : DynamicObject{ size }
    , relativeCharacterSize{ 50.f }
{
    setTextSize(relativeCharacterSize);
    setTextString(text);
}

void Text::setSize(const sf::Vector2f& size)
{
    DynamicObject::setSize(size);
    setTextSize(relativeCharacterSize);
}

void Text::setTextString(const sf::String& string)
{
    text.setString(string);
    centerText();
}

void Text::setTextFont(const std::string& fontPath)
{
    font.loadFromFile(fontPath);
    text.setFont(font);
    centerText();
}

void Text::setTextSize(float percentage)
{
    text.setCharacterSize(static_cast<unsigned int>(getLocalBounds().height * (percentage / 100.f)));
    relativeCharacterSize = percentage;
    centerText();
}

void Text::setTextFillColor(const sf::Color& color)
{
    text.setFillColor(color);
}

void Text::setTextOutlineColor(const sf::Color& color)
{
    text.setOutlineColor(color);
}

void Text::setTextOutlineThickness(float thickness)
{
    text.setOutlineThickness(thickness);
}

void Text::setTextStyle(sf::Text::Style style)
{
    text.setStyle(style);
    centerText();
}

const sf::String& Text::getTextString() const
{
    return text.getString();
}

const sf::Font* Text::getTextFont() const
{
    return &font;
}

float Text::getTextSize() const
{
    return relativeCharacterSize;
}

const sf::Color& Text::getTextFillColor() const
{
    return text.getFillColor();
}

const sf::Color& Text::getTextOutlineColor() const
{
    return text.getOutlineColor();
}

float Text::getTextOutlineThickness() const
{
    return text.getOutlineThickness();
}

sf::Text::Style Text::getTextStyle() const
{
    return (sf::Text::Style)text.getStyle();
}

sf::FloatRect Text::getTextLocalBounds() const
{
    return text.getLocalBounds();
}

sf::FloatRect Text::getTextGlobalBounds() const
{
    return getTransform().transformRect(getTextLocalBounds());
}

void Text::centerText()
{
    text.setOrigin(text.getLocalBounds().left, text.getLocalBounds().top);

    float offsetX{ (getLocalBounds().width - text.getLocalBounds().width) / 2.f };
    float offsetY{ (getLocalBounds().height - text.getLocalBounds().height) / 2.f };
    text.setPosition(getLocalBounds().left + offsetX, getLocalBounds().top + offsetY);
}

void Text::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
    applyChanges(combinedTransform);

    Shape::draw(target, combinedTransform);
    if (text.getFont()) target.draw(text, combinedTransform);
}