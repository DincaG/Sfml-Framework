#include "Shape.h"
#include <cmath>

namespace
{
    sf::Vector2f computeNormal(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        sf::Vector2f normal(p1.y - p2.y, p2.x - p1.x);
        float length = std::sqrt(normal.x * normal.x + normal.y * normal.y);
        if (length != 0.f)
            normal /= length;
        return normal;
    }

    float dotProduct(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        return p1.x * p2.x + p1.y * p2.y;
    }
}

Shape::~Shape()
{
}

void Shape::setTexture(const sf::Texture* texture, bool resetRect)
{
    if (texture)
    {
        if (resetRect || (!m_texture && (m_textureRect == sf::IntRect())))
            setTextureRect(sf::IntRect(0, 0, texture->getSize().x, texture->getSize().y));
    }

    m_texture = texture;
}

const sf::Texture* Shape::getTexture() const
{
    return m_texture;
}

void Shape::setTextureRect(const sf::IntRect& rect)
{
    m_textureRect = rect;
    updateTexCoords();
}

const sf::IntRect& Shape::getTextureRect() const
{
    return m_textureRect;
}

void Shape::setFillColor(const sf::Color& color)
{
    m_fillColor = color;
    updateFillColors();
}

const sf::Color& Shape::getFillColor() const
{
    return m_fillColor;
}

void Shape::setOutlineColor(const sf::Color& color)
{
    m_outlineColor = color;
    updateOutlineColors();
}

const sf::Color & Shape::getOutlineColor() const
{
    return m_outlineColor;
}

void Shape::setOutlineThickness(float thickness)
{
    m_outlineThickness = thickness;
    update();
}

float Shape::getOutlineThickness() const
{
    return m_outlineThickness;
}

sf::FloatRect Shape::getLocalBounds() const
{
    return m_bounds;
}

sf::FloatRect Shape::getGlobalBounds() const
{
    return getTransform().transformRect(getLocalBounds());
}

Shape::Shape() :
m_texture         (NULL),
m_textureRect     (),
m_fillColor       (255, 255, 255),
m_outlineColor    (255, 255, 255),
m_outlineThickness(0),
m_vertices        (sf::TriangleFan),
m_outlineVertices (sf::TriangleStrip),
m_insideBounds    (),
m_bounds          ()
{
}

void Shape::update()
{
    std::size_t count = getPointCount();
    if (count < 3)
    {
        m_vertices.resize(0);
        m_outlineVertices.resize(0);
        return;
    }

    m_vertices.resize(count + 2);

    for (std::size_t i = 0; i < count; ++i)
        m_vertices[i + 1].position = getPoint(i);
    m_vertices[count + 1].position = m_vertices[1].position;

    m_vertices[0] = m_vertices[1];
    m_insideBounds = m_vertices.getBounds();

    m_vertices[0].position.x = m_insideBounds.left + m_insideBounds.width / 2;
    m_vertices[0].position.y = m_insideBounds.top + m_insideBounds.height / 2;

    updateFillColors();
    updateTexCoords();
    updateOutline();
}

void Shape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    //states.transform *= getTransform();

    states.texture = m_texture;
    target.draw(m_vertices, states);

    if (m_outlineThickness != 0)
    {
        states.texture = NULL;
        target.draw(m_outlineVertices, states);
    }
}

void Shape::updateFillColors()
{
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = m_fillColor;
}

void Shape::updateTexCoords()
{
    for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i)
    {
        float xratio = m_insideBounds.width > 0 ? (m_vertices[i].position.x - m_insideBounds.left) / m_insideBounds.width : 0;
        float yratio = m_insideBounds.height > 0 ? (m_vertices[i].position.y - m_insideBounds.top) / m_insideBounds.height : 0;
        m_vertices[i].texCoords.x = m_textureRect.left + m_textureRect.width * xratio;
        m_vertices[i].texCoords.y = m_textureRect.top + m_textureRect.height * yratio;
    }
}

void Shape::updateOutline()
{
    if (m_outlineThickness == 0.f)
    {
        m_outlineVertices.clear();
        m_bounds = m_insideBounds;
        return;
    }

    std::size_t count = m_vertices.getVertexCount() - 2;
    m_outlineVertices.resize((count + 1) * 2);

    for (std::size_t i = 0; i < count; ++i)
    {
        std::size_t index = i + 1;

        sf::Vector2f p0 = (i == 0) ? m_vertices[count].position : m_vertices[index - 1].position;
        sf::Vector2f p1 = m_vertices[index].position;
        sf::Vector2f p2 = m_vertices[index + 1].position;

        sf::Vector2f n1 = computeNormal(p0, p1);
        sf::Vector2f n2 = computeNormal(p1, p2);

        if (dotProduct(n1, m_vertices[0].position - p1) > 0) n1 = -n1;
        if (dotProduct(n2, m_vertices[0].position - p1) > 0) n2 = -n2;

        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        sf::Vector2f normal = (n1 + n2) / factor;

        m_outlineVertices[i * 2 + 0].position = p1;
        m_outlineVertices[i * 2 + 1].position = p1 + normal * m_outlineThickness;
    }

    m_outlineVertices[count * 2 + 0].position = m_outlineVertices[0].position;
    m_outlineVertices[count * 2 + 1].position = m_outlineVertices[1].position;

    updateOutlineColors();

    m_bounds = m_outlineVertices.getBounds();
}

void Shape::updateOutlineColors()
{
    for (std::size_t i = 0; i < m_outlineVertices.getVertexCount(); ++i)
        m_outlineVertices[i].color = m_outlineColor;
}
