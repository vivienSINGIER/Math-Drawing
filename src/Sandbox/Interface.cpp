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
            SelectParameters(HERMITE);
        }

        else if (event.key.code == sf::Keyboard::L && currentStep == SelectType)
        {
            SelectParameters(LAGRANGE);
        }

        else if (event.key.code == sf::Keyboard::B && currentStep == SelectType)
        {
            SelectParameters(BEZIER);
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
            CreatePoint(event);
        }

        else if (currentStep == SelectMirors)
        {
            SelectMiror(event);
        }
    }
}

void Interface::StartCreation()
{
    std::cout << "Please select the type of the new curve" << std::endl;
    std::cout << "H : Hermite, L : Lagrange, B : Bezier" << std::endl;

    currentStep = SelectType;
}

void Interface::SelectParameters(FunctionType type)
{
    currentType = type;

    switch (type)
    {
    case HERMITE:
    {
        system("cls");

        std::cout << "Select X for point 1" << std::endl;

        currentStep = CreatePoints;

        numPoints = 4;

        for (int i = 0; i < numPoints; i++)
        {
            points.push_back(new vertex( 0.f, 0.f ));
        }

        break;
    }
    case BEZIER:
    {
        system("cls");

        std::cout << "Select the number of point" << std::endl;

        currentStep = SelectNumberOfPoints;

        break;
    }
    case LAGRANGE:
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
    if (event.key.code == sf::Keyboard::Backspace)
    {
        if (currentValue.size() != 0)
            currentValue.pop_back();

        std::cout << std::endl;

        std::cout << currentValue;
        return;
    }

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
            points.push_back(new vertex(0.f, 0.f));
        }
    }
    else if (temp != '.' && temp != 'N' && temp != '-')
        currentValue += temp;
}

void Interface::CreatePoint(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Backspace)
    {
        if (currentValue.size() != 0)
            currentValue.pop_back();

        std::cout << std::endl;

        std::cout << currentValue;
        return;
    }

    if (event.key.code == sf::Keyboard::Enter)
    {
        float temp = std::stof(currentValue);
        currentValue = "";
        std::cout << std::endl;
        
        if (setX)
        {
            points[currentPos]->x = temp;
            std::cout << "Select Y for point " << currentPos + 1 << std::endl;
        }
        else
        {
            points[currentPos]->y = temp;
            currentPos++;
            std::cout << "Select X for point " << currentPos + 1 << std::endl;

            if (currentPos == numPoints)
            {
                system("cls");
                currentPos = 0;

                currentStep = SelectMirors;
                std::cout << "Select if you want a miror version from the origin (0 : no, 1 : yes), from X and from Y (all atttach)" << std::endl;                
            }
        }

        setX = !setX;
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

void Interface::SelectMiror(const sf::Event& event)
{
    if (event.key.code == sf::Keyboard::Backspace)
    {
        if (currentValue.size() != 0)
            currentValue.pop_back();

        std::cout << std::endl;

        std::cout << currentValue;
        return;
    }

    if (event.key.code == sf::Keyboard::Enter)
    {
        bool mirorO = false;
        bool mirorX = false;
        bool mirorY = false;

        if (currentValue.size() >= 1)
            if (currentValue[0] == '1')
                mirorO = true;

        if (currentValue.size() >= 2)
            if (currentValue[1] == '1')
                mirorX = true;

        if (currentValue.size() >= 3)
            if (currentValue[2] == '1')
                mirorY = true;

        system("cls");

        m_pGraph->TraceCourbe(currentType, points, mirorO, mirorX, mirorY);

        currentStep = NotStart;
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

#endif
