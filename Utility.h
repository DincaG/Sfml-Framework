#pragma once
#include <SFML/Graphics/Color.hpp>

// Linearly get intermediate values between (beginColor) and (endColor)
// (ratio) of 0.f corresponds to (beginColor)
// (ratio) of 1.f corresponds to (endColor)
sf::Color LinearInterpolation(const sf::Color& beginColor, const sf::Color& endColor, float ratio);

// Linearly get intermediate values between (beginValue) and (endValue)
// (ratio) of 0.f corresponds to (beginValue)
// (ratio) of 1.f corresponds to (endValue)
float LinearInterpolation(float beginValue, float endValue, float ratio);

// Get intermediate values between (beginValue) and (endValue) that go through control points
float CubicInterpolation(float beginValue, float controlPoint1, float controlPoint2, float endValue, float ratio);