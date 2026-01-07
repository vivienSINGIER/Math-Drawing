#pragma once

#include <SFML/System/Vector2.hpp>

class sf::Color;

namespace Utils
{
	float Pow(float x, int n);
	float Sqrt(float x);
	int Factorial(int n);
	float BinomialCoeff(int n, int k);
	float Abs(float x);

	int RoundToInt(float x);
	float RoundToStep(float x, float step);
	
	float Cos(float x);
	float Sin(float x);
	
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);
	float GetAngle(sf::Vector2f& v1, sf::Vector2f& v2);
	float GetAngle(sf::Vector2f& v1);

	sf::Vector2f NormalizeVector(sf::Vector2f const& vector);
	float Norm(sf::Vector2f const& vector);
	float Dot(sf::Vector2f const& v1, sf::Vector2f const& v2);

	float RadToDeg(float rad);

	sf::Color HSVtoRGB(float h, float s, float v);
}