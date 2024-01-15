#pragma once
#include <SFML/Graphics.hpp>
#include "RoundedRectangle.h"
#include "Visible.h"
#include "Scheduler.h"
#include <iostream>

class DynamicObject : public RoundedRectangle, virtual public Visible
{
public:

    enum class ParentObjectType
    {
        Dynamic,
        Shape,
        Sprite,
        Window,
        None
    };

    DynamicObject(const sf::Vector2f& size = { 0.f, 0.f });
    void setSize(const sf::Vector2f& size);
    void setRelativePosition(float percentageX, float percentageY);
    void setRelativePosition(const sf::Vector2f& percentage);
    void setPosition(float x, float y);
    void setPosition(const sf::Vector2f& position);
    void setScale(float factorX, float factorY);
    void scale(float factorX, float factorY);
    void setRotation(float angle);
    void rotate(float rotation);
    void setParent(DynamicObject& parent);
    void setParent(sf::Shape& parent);
    void setParent(sf::Sprite& parent);
    void setParent(sf::RenderWindow& parent);
    void removeParent();
    void* getParent() const;
    ParentObjectType getParentObjectType() const;
    sf::Vector2f getRelativePosition() const;
    sf::Vector2f getGlobalPosition() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getScale() const;
    float getRotation() const;
    sf::FloatRect getGlobalBounds() const;
    sf::Transform getTransform() const;

protected:

    void applyChanges(sf::Transform& transform) const;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    void* parentObject;
    ParentObjectType objectType;
    sf::Vector2f relativePosition;
    sf::Vector2f scaleFactor;
    float rotation;
    bool dynamicallyPositioned;
};