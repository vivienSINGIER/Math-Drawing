#include <SFML/System/Vector2.hpp>

#include <cmath>

#include "Sandbox/pch.h"

namespace Utils 
{
	float Pow(float x, int n)
	{
		float result = 1;
		for (int i = 0; i < n; i++)
			result *= x;
		return result;
	}

	float Sqrt(float x)
	{
		if (x < 0.0f)
			return 0.0f;

		float result = x;
		float lastResult = 0.0f;
		while (result != lastResult)
		{
			lastResult = result;
			result = (result + x / result) * 0.5f;
		}
		return result;
	}
	
	int Factorial(int n)
	{
		int result = 1;
		for (int i = 1; i <= n; i++)
			result *= i;
		return result;
	}

	float BinomialCoeff(int n, int k)
	{
		if (k < 0 || k > n) return 0;
		if (k == 0 || k == n) return 1;

		float result = 1;
		for (int i = 1; i <= k; i++)
		{
			result *= (n - i + 1) / i;
		}
		return result;
	}

	float Abs(float x)
	{
		return (x < 0) ? -x : x;
	}

	int RoundToInt(float value) {
		return (value >= 0.0f)
			? static_cast<int>(value + 0.5f)
			: static_cast<int>(value - 0.5f);
	}

	float RoundToStep(float x, float step)
	{
		return RoundToInt(x / step) * step ;
	}
	
	float Cos(float x)
	{
		float term = 1.0f;
		float sum  = 1.0f;

		for (int i = 1; i < 10; i++)
		{
			term *= -x * x / ((2*i - 1) * (2*i));
			sum += term;
		}
		return sum;
	}

	float Sin(float x)
	{
		float term = x;
		float sum  = x;

		for (int i = 1; i < 10; i++)
		{
			term *= -x * x / ((2*i) * (2*i + 1));
			sum += term;
		}
		return sum;
	}
	
    bool Normalize(sf::Vector2f& vector)
    {
        float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;
		
			return true;
		}

		return false;
    }

	float GetDistance(int x1, int y1, int x2, int y2)
	{
		int x = x2 - x1;
		int y = y2 - y1;

		return std::sqrt(x * x + y * y);
	}

	float GetAngleDegree(const sf::Vector2f& v1, const sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}

	float GetAngle(sf::Vector2f& v1, sf::Vector2f& v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot);
	}

	float GetAngle(sf::Vector2f& v1)
	{
		float dot = v1.x * 1.0f + v1.y * 0.0f;
		float det = v1.x * 0.0f - v1.y * 1.0f;

		return std::atan2(det, dot);
	}

	sf::Vector2f NormalizeVector(sf::Vector2f const& vector)
	{
		float magnitude = Sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude == 0)
			return vector;
		
		sf::Vector2f result;
		result.x = vector.x / magnitude;
		result.y = vector.y / magnitude;

		return result;		
	}

	float Norm(sf::Vector2f const& vector)
	{
		float magnitude = Sqrt(vector.x * vector.x + vector.y * vector.y);
		
		return magnitude;		
	}

	float Dot(sf::Vector2f const& v1, sf::Vector2f const& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
}
