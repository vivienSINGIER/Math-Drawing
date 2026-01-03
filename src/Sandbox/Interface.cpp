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
                    currentStep = SelectMirors;
                    std::cout << "Select if you want a miror version from the origin (0 : no, 1 : yes), from X and from Y (all atttach)" << std::endl;
                    //m_pGraph->TraceCourbe(currentType, points);
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
            std::cout << "Select if you want a miror version from the origin (0 : no, 1 : yes), from X and from Y (all atttach)" << std::endl;
            currentStep = SelectMirors;
            //m_pGraph->TraceCourbe(currentType, points, derivPoints);
        }
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

void Interface::SelectMiror(const sf::Event& event)
{
    /*char temp = ConvertKeyCode::Convert(event.key);

    if (temp == '0' || temp == '1')
    {
        bool isMiror = true;

        if (temp == '0')
            isMiror = false;

        system("cls");

        m_pGraph->TraceCourbe(currentType, points, derivPoints, isMiror);
    }*/
    if (event.key.code == sf::Keyboard::Enter)
    {
        bool mirorO = true;
        bool mirorX = true;
        bool mirorY = true;

        if (currentValue[0] == 0)
            mirorO = false;

        if (currentValue[1] == 0)
            mirorX = false;

        if (currentValue[2] == 0)
            mirorY = false;

        system("cls");

        m_pGraph->TraceCourbe(currentType, points, derivPoints, mirorO, mirorX, mirorY);
    }
    else
    {
        currentValue += ConvertKeyCode::Convert(event.key);
    }
}

#endif
