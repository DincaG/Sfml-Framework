#include "Utility.h"

sf::Color LinearInterpolation(const sf::Color& beginColor, const sf::Color& endColor, float ratio)
{
	sf::Color color;
	color.r = (unsigned int)LinearInterpolation(beginColor.r, endColor.r, ratio);
	color.g = (unsigned int)LinearInterpolation(beginColor.g, endColor.g, ratio);
	color.b = (unsigned int)LinearInterpolation(beginColor.b, endColor.b, ratio);
	color.a = (unsigned int)LinearInterpolation(beginColor.a, endColor.a, ratio);

	return color;
}

float LinearInterpolation(float beginValue, float endValue, float ratio)
{
	return beginValue + (endValue - beginValue) * ratio;
}

float CubicInterpolation(float beginValue, float controlPoint1, float controlPoint2, float endValue, float ratio)
{
	float q0{ LinearInterpolation(beginValue, controlPoint1, ratio) };
	float q1{ LinearInterpolation(controlPoint1, controlPoint2, ratio) };
	float q2{ LinearInterpolation(controlPoint2, endValue, ratio) };
	
	float r0{ LinearInterpolation(q0, q1, ratio) };
	float r1{ LinearInterpolation(q1, q2, ratio) };

	return LinearInterpolation(r0, r1, ratio);
}