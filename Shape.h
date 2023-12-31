#pragma once
#include <SFML/Graphics.hpp>

class Shape : public sf::Drawable, public sf::Transformable
{
public:

    virtual ~Shape();
    void setTexture(const sf::Texture* texture, bool resetRect = false);
    void setTextureRect(const sf::IntRect& rect);
    void setFillColor(const sf::Color& color);
    void setOutlineColor(const sf::Color& color);
    void setOutlineThickness(float thickness);
    const sf::Texture* getTexture() const;
    const sf::IntRect& getTextureRect() const;
    const sf::Color& getFillColor() const;
    const sf::Color& getOutlineColor() const;
    float getOutlineThickness() const;
    virtual std::size_t getPointCount() const = 0;
    virtual sf::Vector2f getPoint(std::size_t index) const = 0;
    sf::FloatRect getLocalBounds() const;
    sf::FloatRect getGlobalBounds() const;

protected:

    Shape();
    void update();
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:

    void updateFillColors();
    void updateTexCoords();
    void updateOutline();
    void updateOutlineColors();

private:

    const sf::Texture* m_texture;         
    sf::IntRect        m_textureRect;     
    sf::Color          m_fillColor;       
    sf::Color          m_outlineColor;    
    float              m_outlineThickness;
    sf::VertexArray    m_vertices;        
    sf::VertexArray    m_outlineVertices; 
    sf::FloatRect      m_insideBounds;    
    sf::FloatRect      m_bounds;          
};