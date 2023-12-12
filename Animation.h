#pragma once
#include <SFML/Graphics.hpp>
#include "DynamicObject.h"

struct Frame 
{
	sf::Texture* texutre;
	double duration; // in seconds
};

class Animation 
{
	std::vector<Frame> frames;
	double totalLength;
	double progress;
	DynamicObject* target;

public:
	Animation();
	Animation(DynamicObject* target);
	void setTarget(DynamicObject* target);
	void addFrame(Frame frame);
	void update(double elapsed);
	const double getLength() const { return totalLength; }
};