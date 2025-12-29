#include "pch.h"
#ifndef INTERFACE_CPP_DEFINED
#define INTERFACE_CPP_DEFINED

#include "Interface.h"

void Interface::Update()
{

}

void Interface::OnEvent(const sf::Event& event)
{
    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::A && currentStep == NotStart)
        {
            StartCreation();
        }

        else if (event.key.code == sf::Keyboard::H && currentStep == SelectType)
        {
            SelectParameters(TypeHermite);
        }

        else if (event.key.code == sf::Keyboard::L && currentStep == SelectType)
        {
            SelectParameters(TypeLagrange);
        }

        else if (event.key.code == sf::Keyboard::B && currentStep == SelectType)
        {
            SelectParameters(TypeBezier);
        }

        else if (currentStep == SelectNumberOfPoints)
        {
            SelectNumberOfPoint(event);
        }

        else if (currentStep == CreatePoints)
        {
            CreatePoint(event);
        }

        else if (currentStep == CreateDerivPoints)
        {
            CreateDerivPoint(event);
        }
    }
}

void Interface::StartCreation()
{
    std::cout << "Please select the type of the new curve" << std::endl;
    std::cout << "H : Hermite, L : Lagrange, B : Bezier" << std::endl;

    currentStep = SelectType;
}

void Interface::SelectParameters(Type type)
{
    currentType = type;

    switch (type)
    {
    case TypeHermite:
    {
        system("cls");

        std::cout << "Select X for point 1" << std::endl;

        currentStep = CreatePoints;

        numPoints = 2;

        for (int i = 0; i < numPoints; i++)
        {
            points.push_back({ 0.f, 0.f });
        }

        break;
    }
    case TypeBezier:
    {
        system("cls");

        std::cout << "Select the number of point" << std::endl;

        currentStep = SelectNumberOfPoints;

        break;
    }
    case TypeLagrange:
    {
        system("cls");

        std::cout << "Select the number of point" << std::endl;

        currentStep = SelectNumberOfPoints;

        break;
    }
    }
}

void Interface::SelectNumberOfPoint(const sf::Event& event)
{
    char temp = ConvertKeyCode::Convert(event.key);

    if (event.key.code == sf::Keyboard::Enter)
    {
        currentStep = CreatePoints;
        numPoints = std::stoi(currentValue);
        currentValue = "";
        system("cls");
        std::cout << "Select X for point 1" << std::endl;

        //set list
        for (int i = 0; i < numPoints; i++)
        {
            points.push_back({0.f, 0.f});
        }
    }
    else if (temp != '.' && temp != 'N' && temp != '-')
        currentValue += temp;
}

void Interface::CreatePoint(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Enter)
    {
        float temp = std::stof(currentValue);
        currentValue = "";
        std::cout << std::endl;
        
        if (setX)
        {
            points[currentPos].x = temp;
            std::cout << "Select Y for point " << currentPos + 1 << std::endl;
        }
        else
        {
            points[currentPos].y = temp;
            currentPos++;
            std::cout << "Select X for point " << currentPos + 1 << std::endl;

            if (currentPos == numPoints)
            {
                system("cls");
                currentPos = 0;

                if (currentType == TypeBezier || currentType == TypeLagrange)
                {
                    currentStep = NotStart;
                    m_pGraph->TraceCourbe(currentType, points);
                }
                else
                {
                    currentStep = CreateDerivPoints;
                    std::cout << "Now the deriv point" << std::endl;
                    std::cout << "Select Y for point 1" << std::endl;
                }
                
            }
        }

        setX = !setX;
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

void Interface::CreateDerivPoint(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Enter)
    {
        float temp = std::stof(currentValue);
        currentValue = "";
        std::cout << std::endl;

        derivPoints.push_back({points[currentPos].x,temp});

        std::cout << "Select Y for point " << currentPos + 1 << std::endl;

        currentPos++;

        if (derivPoints.size() == numPoints)
        {
            system("cls");
            currentPos = 0;

            currentStep = NotStart;
            m_pGraph->TraceCourbe(currentType, points, derivPoints);
        }
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

#endif
