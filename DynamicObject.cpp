#include "DynamicObject.h"

DynamicObject::DynamicObject(const sf::Vector2f& size)
    : RoundedRectangle{ size }
    , parentObject{ nullptr }
    , objectType{ ParentObjectType::None }
    , relativePosition{ 0.f, 0.f }
    , scaleFactor{ 1.f, 1.f }
    , rotation { 0.f }
    , dynamicallyPositioned{ false }
{
    setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
}

void DynamicObject::setSize(const sf::Vector2f& size)
{
	RoundedRectangle::setSize(size);
	setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
}

void DynamicObject::setRelativePosition(float percentageX, float percentageY)
{
	setRelativePosition(sf::Vector2f(percentageX, percentageY));
}

void DynamicObject::setRelativePosition(const sf::Vector2f& percentage)
{
	RoundedRectangle::setPosition(0.f, 0.f);
	relativePosition = percentage;
	dynamicallyPositioned = true;
}

void DynamicObject::setPosition(float x, float y)
{
	setPosition(sf::Vector2f(x, y));
}

void DynamicObject::setPosition(const sf::Vector2f& position)
{
	RoundedRectangle::setPosition(position);
	dynamicallyPositioned = false;
}

void DynamicObject::setScale(float factorX, float factorY)
{
	scaleFactor = { factorX, factorY };
}

void DynamicObject::scale(float factorX, float factorY)
{
    scaleFactor.x *= factorX;
    scaleFactor.y *= factorY;
}

void DynamicObject::setRotation(float angle)
{
    rotation = angle;
}

void DynamicObject::rotate(float angle)
{
    rotation += angle;
}

void DynamicObject::setParent(DynamicObject& parent)
{
    parentObject = &parent;
    objectType = ParentObjectType::Dynamic;
}

void DynamicObject::setParent(sf::Shape& parent)
{
    parentObject = &parent;
    objectType = ParentObjectType::Shape;
}

void DynamicObject::setParent(sf::Sprite& parent)
{
    parentObject = &parent;
    objectType = ParentObjectType::Sprite;
}

void DynamicObject::setParent(sf::RenderWindow& parent)
{
    parentObject = &parent;
    objectType = ParentObjectType::Window;
}

void DynamicObject::removeParent()
{
    parentObject = nullptr;
    objectType = ParentObjectType::None;
}

void* DynamicObject::getParent() const
{
    return parentObject;
}

DynamicObject::ParentObjectType DynamicObject::getParentObjectType() const
{
    return objectType;
}

sf::Vector2f DynamicObject::getRelativePosition() const
{
    return relativePosition;
}

sf::Vector2f DynamicObject::getGlobalPosition() const
{
	return getTransform().transformPoint(getOrigin());
}

sf::Vector2f DynamicObject::getPosition() const
{
    return RoundedRectangle::getPosition();
}

sf::Vector2f DynamicObject::getScale() const
{
    return scaleFactor;
}

float DynamicObject::getRotation() const
{
    return rotation;
}

sf::FloatRect DynamicObject::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

sf::Transform DynamicObject::getTransform() const
{
	sf::Transform transform{ RoundedRectangle::getTransform() };
	applyChanges(transform);
	return transform;
}

void DynamicObject::applyChanges(sf::Transform& transform) const
{
    sf::Vector2f parentSize{ -1.f, -1.f };
    if (parentObject)
    {
        sf::FloatRect parentBounds{};
        switch (objectType)
        {
            case ParentObjectType::Dynamic:
            {
                transform = ((DynamicObject*)parentObject)->getTransform() * transform;
                parentBounds = ((DynamicObject*)parentObject)->getLocalBounds();
                break;
            }
            case ParentObjectType::Shape:
            {
                transform = ((sf::Shape*)parentObject)->getTransform() * transform;
                parentBounds = ((sf::Shape*)parentObject)->getLocalBounds();
                break;
            }
            case ParentObjectType::Sprite:
            {
                transform = ((sf::Sprite*)parentObject)->getTransform() * transform;
                parentBounds = ((sf::Sprite*)parentObject)->getLocalBounds();
                break;
            }
            case ParentObjectType::Window:
            {
                sf::Vector2u size{ ((sf::RenderWindow*)parentObject)->getSize() };
                parentBounds = sf::FloatRect(0, 0, (float)size.x, (float)size.y);
                break;
            }
        }
        parentSize = sf::Vector2f(parentBounds.width, parentBounds.height);
    }

    if (dynamicallyPositioned && parentSize != sf::Vector2f{ -1.f, -1.f })
    {
        transform.translate(relativePosition.x / 100.f * parentSize.x, relativePosition.y / 100.f * parentSize.y);
    }

    transform.rotate(rotation, getOrigin());
    transform.scale(scaleFactor, getOrigin());
}

void DynamicObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::Transform combinedTransform{ states.transform * sf::Transformable::getTransform() };
    applyChanges(combinedTransform);

    Shape::draw(target, combinedTransform);
}