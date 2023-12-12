#include "Visible.h"

Visible::Visible()
	: visibility{ true }
{
}

Visible::~Visible()
{
	autoDraw(false);
}

void Visible::show()
{
	visibility = true;
}

void Visible::hide()
{
	visibility = false;
}

bool Visible::isVisible() const
{
	return visibility;
}

void Visible::autoDraw(bool autoDraw)
{
	for (size_t i{ 0 }; i < visibleObjects.size(); i++)
	{
		if (visibleObjects[i] == this)
		{
			if (!autoDraw)
			{
				visibleObjects.erase(visibleObjects.begin() + i);
			}
			return;
		}
	}
	if (autoDraw)
	{
		visibleObjects.push_back(this);
	}
}

void Visible::drawVisibleObjects(sf::RenderTarget& target)
{
	for (Visible* object : visibleObjects)
	{
		if (object->isVisible())
		{
			object->draw(target, sf::RenderStates::Default);
		}
	}
}

std::vector<Visible*> Visible::visibleObjects;