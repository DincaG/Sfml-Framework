#include "RoundedRectangleShape.h"
#include <cmath>

RoundedRectangleShape::RoundedRectangleShape(const sf::Vector2f& size, float radius, unsigned int cornerPointCount)
{
    this->size = size;
    this->radius = radius;
    this->cornerPointCount = cornerPointCount;
    update();
}

void RoundedRectangleShape::setSize(const sf::Vector2f& size)
{
    this->size = size;
    update();
}

void RoundedRectangleShape::setCornerRadius(float radius)
{
    this->radius = radius;
    update();
}

void RoundedRectangleShape::setCornerPointCount(unsigned int count)
{
    this->cornerPointCount = count;
    update();
}

const sf::Vector2f& RoundedRectangleShape::getSize() const
{
    return size;
}

float RoundedRectangleShape::getCornerRadius() const
{
    return radius;
}

std::size_t RoundedRectangleShape::getPointCount() const
{
    return cornerPointCount * 4;
}

sf::Vector2f RoundedRectangleShape::getPoint(std::size_t index) const
{
    if (index >= cornerPointCount * 4)
    {
        return sf::Vector2f(0, 0);
    }

    float deltaAngle{ 90.0f / (cornerPointCount - 1) };
    unsigned int centerIndex{ index / cornerPointCount };
    static const float pi{ 3.141592654f };
    
    sf::Vector2f center;
    switch (centerIndex)
    {
        case 0: 
        {
            center.x = size.x - radius; 
            center.y = radius; 
            break;
        }
        case 1: 
        {
            center.x = radius; 
            center.y = radius; 
            break;
        }
        case 2: 
        {
            center.x = radius; 
            center.y = size.y - radius; 
            break;
        }
        case 3:
        {
            center.x = size.x - radius; 
            center.y = size.y - radius; 
            break;
        }
    }

    float angle{ deltaAngle * (index - centerIndex) * pi / 180 };
    return sf::Vector2f(radius * cos(angle) + center.x, -radius * sin(angle) + center.y);
}
