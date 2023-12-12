#pragma once
#include "Shape.h"


class RoundedRectangleShape : public Shape
{
public:

    explicit RoundedRectangleShape(const sf::Vector2f & size = sf::Vector2f(0, 0), float radius = 0, unsigned int cornerPointCount = 10);
    void setSize(const sf::Vector2f& size);
    void setCornerRadius(float radius);
    void setCornerPointCount(unsigned int count);
    const sf::Vector2f& getSize() const;
    float getCornerRadius() const;
    virtual std::size_t getPointCount() const;
    virtual sf::Vector2f getPoint(std::size_t index) const;

private:

    sf::Vector2f size;
    float radius;
    unsigned int cornerPointCount;
};