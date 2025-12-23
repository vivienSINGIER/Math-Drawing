#include <SFML/System/Vector2.hpp>

#include <cmath>

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
	
	float Cos(float x)
	{
		float result = 0.0f;
		for (int i = 0; i < 10; i++)
		{
			result += pow(-1, i) * (pow(x, 2 * i) / 2) / Factorial(2 * i);
		}
		return result;
	}

	float Sin(float x)
	{
		float result = 0.0f;
		for (int i = 0; i < 10; i++)
		{
			result += pow(-1, i) * (pow(x, 2 * i + 1) / 2) / Factorial(2 * i + 1);
		}
		return result;
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
}