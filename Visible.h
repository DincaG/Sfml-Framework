#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Visible
{
public:

	Visible();
	~Visible();
	void autoDraw(bool autoDraw);
	void show();
	void hide();
	bool isVisible() const;
	static void drawVisibleObjects(sf::RenderTarget& target);

private:

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;

	bool visibility;
	static std::vector<Visible*> visibleObjects;
};