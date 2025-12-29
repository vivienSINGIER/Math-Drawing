#pragma once

#include <SFML/System/Vector2.hpp>

namespace Utils
{
	float Pow(float x, int n);
	float Sqrt(float x);
	int Factorial(int n);
	float BinomialCoeff(int n, int k);
	float Abs(float x);
	
	float Cos(float x);
	float Sin(float x);
	
	bool Normalize(sf::Vector2f& vector);
	float GetDistance(int x1, int y1, int x2, int y2);
	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2);
	float GetAngle(sf::Vector2f& v1, sf::Vector2f& v2);
	float GetAngle(sf::Vector2f& v1);

	sf::Vector2f NormalizeVector(sf::Vector2f const& vector);
	float Norm(sf::Vector2f const& vector);
}