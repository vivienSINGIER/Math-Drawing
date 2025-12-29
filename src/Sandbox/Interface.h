#ifndef INTERFACE_H_DEFINED
#define INTERFACE_H_DEFINED

#include "Graph.h"
#include <iostream>

enum CreationStep {
	NotStart,
	SelectType,
	SelectNumberOfPoints,
	CreatePoints,
};

struct ConvertKeyCode
{
	static char Convert(sf::Event::KeyEvent key)
	{
		switch (key.code)
		{
		case sf::Keyboard::Num0:
		case sf::Keyboard::Numpad0:
		{
			std::cout << "0";
			return '0';
			break;
		}
		case sf::Keyboard::Num1:
		case sf::Keyboard::Numpad1:
		{
			std::cout << "1";
			return '1';
			break;
		}
		case sf::Keyboard::Num2:
		case sf::Keyboard::Numpad2:
		{
			std::cout << "2";
			return '2';
			break;
		}
		case sf::Keyboard::Num3:
		case sf::Keyboard::Numpad3:
		{
			std::cout << "3";
			return '3';
			break;
		}
		case sf::Keyboard::Num4:
		case sf::Keyboard::Numpad4:
		{
			std::cout << "4";
			return '4';
			break;
		}
		case sf::Keyboard::Num5:
		case sf::Keyboard::Numpad5:
		{
			std::cout << "5";
			return '5';
			break;
		}
		case sf::Keyboard::Num6:
		case sf::Keyboard::Numpad6:
		{
			std::cout << "6";
			return '6';
			break;
		}
		case sf::Keyboard::Num7:
		case sf::Keyboard::Numpad7:
		{
			std::cout << "7";
			return '7';
			break;
		}
		case sf::Keyboard::Num8:
		case sf::Keyboard::Numpad8:
		{
			std::cout << "8";
			return '8';
			break;
		}
		case sf::Keyboard::Num9:
		case sf::Keyboard::Numpad9:
		{
			std::cout << "9";
			return '9';
			break;
		}
		case sf::Keyboard::Period:
		{
			std::cout << ".";
			return '.';
			break;
		}
		case sf::Keyboard::Subtract:
		case sf::Keyboard::Hyphen:
		{
			std::cout << "-";
			return '-';
			break;
		}
		default:
		{
			return 'N';
			break;
		}
		}
	}
};

class Interface
{
public:
	Graph* m_pGraph = nullptr;

	void Update();

	void OnEvent(const sf::Event& event);

private:
	CreationStep currentStep = NotStart;

	Type currentType;

	int numPoints = 0;
	std::vector<vertex> points;
	int currentPos = 0;
	bool setX = true;

	std::string currentValue = "";

	void StartCreation();
	void SelectParameters(Type type);
	void SelectNumberOfPoint(const sf::Event& event);
	void CreatePoint(const sf::Event& event);
};


#endif
