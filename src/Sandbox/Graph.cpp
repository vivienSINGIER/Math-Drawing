#include "pch.h"
#ifndef GRAPH_CPP_DEFINED
#define GRAPH_CPP_DEFINED

#include "Graph.h"

#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Debug.h"
#include "Utils.h"
#include "MathFunctions.hpp"
#include "Interface.h"

#include <iostream>

void Graph::OnInitialize()
{
    //InitDiamond();
    //InitHeart();
    //InitSpade();
    //InitClub();
    
    //Zoom
    m_pView = GameManager::Get()->GetView();
    m_pUiView = GameManager::Get()->GetUIView();
    m_windowSize = m_pView->getSize();
    m_pWindow = GameManager::Get()->GetWindow();

    m_pInterface = new Interface;
    m_pInterface->m_pGraph = this;
}

void Graph::OnEvent(const sf::Event& event)
{
    m_pInterface->OnEvent(event);

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            m_isMousePressed = true;
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (m_selectedCurve != nullptr)
            {
                HandleVertexSelection(true);
            }
        }
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            m_isMousePressed = false;
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            if (m_selectedCurve != nullptr)
            {
                HandleVertexSelection(false);
            }
        }
    }

    //Zoom
    if (event.type == sf::Event::MouseWheelScrolled)
    {
        m_currentZoom += event.mouseWheelScroll.delta * -0.1f;

        if (m_currentZoom <= 0.f)
        {
            m_currentZoom = 0.1f;
        }

        m_pView->setSize(m_windowSize * m_currentZoom);
    }

    if (event.type == sf::Event::KeyPressed)
    {
        switch (event.key.code)
        {
        case sf::Keyboard::Num0:
            m_selectedType = LINEAR;
            break;
        case sf::Keyboard::Num1:
            m_selectedType = PARABOLA;
            break;
        case sf::Keyboard::Num2:
            m_selectedType = LAGRANGE;
            break;
        case sf::Keyboard::Num3:
            m_selectedType = HERMITE;
            break;
        case sf::Keyboard::Num4:
            m_selectedType = BEZIER;
            break;
        case sf::Keyboard::Num5:
            m_selectedType = ELLIPSE;
            break;
        case sf::Keyboard::Num6:
            m_selectedType = ARC_CIRCLE;
            break;
        }

        // Type change
        if (event.key.code == sf::Keyboard::Enter)
        {
            if (m_selectedCurve != nullptr)
            {
                m_selectedCurve->SetType(m_selectedType);
                m_selectedCurve->CalculateCurve();
            }
        }

        if (event.key.code == sf::Keyboard::S)
        {
            if (m_selectedCurve != nullptr)
            {
                m_selectedCurve->AddSymetry();
            }
        }

        // Curve Selection
        if (event.key.code == sf::Keyboard::Up)
            m_selectedIndex++;
        if (event.key.code == sf::Keyboard::Down)
            m_selectedIndex--;

        // Curve Creation
        if (event.key.code == sf::Keyboard::Add)
        {
            Curve* curve = new Curve;
            curve->SetType(m_selectedType);
            curve->CalculateCurve();
            m_selectedCurve = curve;
            m_vCurves.push_back(curve);
            m_selectedIndex = m_vCurves.size() - 1;
        }
        if (event.key.code == sf::Keyboard::Subtract)
        {
            if (m_vCurves.size() >= 1)
            {
                delete m_vCurves[m_selectedIndex];
                m_vCurves.erase(m_vCurves.begin() + m_selectedIndex);
                m_selectedIndex--;
            }
        }

        // Curve Selection
        if (m_selectedIndex < 0)
            m_selectedIndex = 0;
        if (m_selectedIndex >= m_vCurves.size())
            m_selectedIndex = m_vCurves.size() - 1;

        if (m_selectedIndex >= 0)
            m_selectedCurve = m_vCurves[m_selectedIndex];
        else
            m_selectedCurve = nullptr;
    }
}

void Graph::OnUpdate()
{
    HandleMouseMovement();
    DrawGraph();
    m_pInterface->Update();

    for (Curve* curve : m_vCurves)
    {
        if (curve == m_selectedCurve)
        {
            curve->DrawPoints();
            curve->DrawPath(sf::Color::Blue);
        }
        else
            curve->DrawPath(sf::Color::Red);
    }

    if (m_selectedVertex == nullptr) return;
    if (m_selectedCurve == nullptr) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*GameManager::Get()->GetWindow());
    sf::Vector2f worldPos = m_pWindow->mapPixelToCoords(mousePos, *m_pView);

    worldPos.x /= TILE_SIZE;
    worldPos.y /= TILE_SIZE;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        worldPos.x = Utils::RoundToInt(worldPos.x);
        worldPos.y = Utils::RoundToInt(worldPos.y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    {
        worldPos.x = Utils::RoundToStep(worldPos.x, 0.1f);
        worldPos.y = Utils::RoundToStep(worldPos.y, 0.1f);
    }
    
    m_selectedCurve->m_function->SetVertexPosition(m_selectedVertex, worldPos.x, -worldPos.y);
    m_selectedCurve->CalculateCurve();
}

void Graph::DrawGraph()
{
    float tempX = m_minX;
    while (tempX <= m_maxX)
    {
        float size = 10.0f;
        if (tempX == m_maxX || tempX == m_minX)
            size *= 1.5f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            Debug::DrawLine(tempX * TILE_SIZE, m_minY * TILE_SIZE, tempX * TILE_SIZE, m_maxY * TILE_SIZE, sf::Color(50, 50, 50));
        
        Debug::DrawLine(tempX * TILE_SIZE, -size, tempX * TILE_SIZE, size, sf::Color::White);
        if ((int)tempX != 0)
            Debug::DrawText(tempX * TILE_SIZE - 2.5f, 21.0f, std::to_string((int)tempX), sf::Color::Red);
            
        tempX += m_interval_X;
    }

    float tempY = m_minY;
    while (tempY <= m_maxY)
    {
        float size = 10.0f;
        if (tempY == m_maxY || tempY == m_minY)
            size *= 1.5f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            Debug::DrawLine(m_minX * TILE_SIZE, tempY * TILE_SIZE, m_maxX * TILE_SIZE, tempY * TILE_SIZE, sf::Color(50, 50, 50));
        
        Debug::DrawLine(-size, tempY * TILE_SIZE, size, tempY * TILE_SIZE, sf::Color::White);
        if ((int)tempY != 0)
            Debug::DrawText(-35.f, tempY * TILE_SIZE - 2.5f, std::to_string((int)-tempY), sf::Color::Green);
        
        tempY += m_interval_Y;
    }

    Debug::DrawLine(m_minX * TILE_SIZE, 0.0f, m_maxX * TILE_SIZE, 0.0f, sf::Color::White);
    Debug::DrawLine(0.0f, m_minY * TILE_SIZE, 0.0f, m_maxY * TILE_SIZE, sf::Color::White);
    
    std::string typeStr = std::string(magic_enum::enum_name(m_selectedType));

    sf::Vector2f txtPos = m_pUiView->getCenter() - m_pUiView->getSize() / 2.f;
    Debug::DrawText(txtPos.x, txtPos.y, typeStr, sf::Color::Magenta, 20.0f, true);

    if (m_selectedVertex == nullptr) return;

    std::string coordTxt = "( ";
    coordTxt.append(std::to_string(m_selectedVertex->x));
    coordTxt.append(", ");
    coordTxt.append(std::to_string(m_selectedVertex->y));
    coordTxt.append(" )");

    sf::Vector2f txtPos2 = m_pUiView->getCenter() - m_pUiView->getSize() / 2.f + sf::Vector2f(0, 50);
    Debug::DrawText(txtPos2.x, txtPos2.y, coordTxt, sf::Color::Magenta, 20.0f, true);
}

void Graph::HandleMouseMovement()
{
    m_lastMousePos = m_mousePos;
    m_mousePos = sf::Mouse::getPosition();
    
    if (m_isMousePressed == false) return;

    sf::Vector2f mouseDelta = {(float)(m_lastMousePos.x - m_mousePos.x), -(float)(m_lastMousePos.y - m_mousePos.y)};
    mouseDelta *= m_movementSpeed * GameManager::Get()->GetDeltaTime() * m_currentZoom;

    m_pView->move(mouseDelta.x, -mouseDelta.y);
}

void Graph::HandleVertexSelection(bool state)
{
    if (state == false && m_selectedVertex == nullptr) return;
    
    if (state == false && m_selectedVertex != nullptr)
    {
        m_selectedVertex->isSelected = false;
        m_selectedVertex = nullptr;
        return;
    }

    if (m_selectedVertex != nullptr) return;
    
    if (m_selectedCurve == nullptr) return;

    sf::Vector2i mousePos = sf::Mouse::getPosition(*GameManager::Get()->GetWindow());
    sf::Vector2f worldPos = m_pWindow->mapPixelToCoords(mousePos, *m_pView);
    
    vertex* newSelected = m_selectedCurve->HandleSelection(worldPos.x, worldPos.y);

    if (newSelected != nullptr)
    {
        m_selectedVertex = newSelected;
        m_selectedVertex->isSelected = true;
        return;
    }

    worldPos.x /= TILE_SIZE;
    worldPos.y /= TILE_SIZE;
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    {
        worldPos.x = Utils::RoundToInt(worldPos.x);
        worldPos.y = Utils::RoundToInt(worldPos.y);
    }

    vertex* v = m_selectedCurve->m_function->AddControlPoint(worldPos.x, -worldPos.y);

    if (v != nullptr)
    {
        if (m_selectedVertex != nullptr)
            m_selectedVertex->isSelected = false;
        m_selectedVertex = v;
        m_selectedVertex->isSelected = true;
    }
    
    m_selectedCurve->CalculateCurve();
}

void Graph::InitDiamond()
{
    Curve* curve = new Curve();
    curve->SetType(BEZIER);
    curve->m_function->ClearControlPoints();
    curve->m_function->controlPoints.push_back(new vertex(0.0f, 0.0f));
    curve->m_function->controlPoints.push_back(new vertex(1.0f, 3.0f));
    curve->m_function->controlPoints.push_back(new vertex(3.0f, 4.0f));
    curve->CalculateCurve();
    m_vCurves.push_back(curve);

    Curve* curve1 = new Curve();
    curve1->SetType(BEZIER);
    curve1->m_function->ClearControlPoints();
    curve1->m_function->controlPoints.push_back(new vertex(0.0f, 0.0f));
    curve1->m_function->controlPoints.push_back(new vertex(-1.0f, 3.0f));
    curve1->m_function->controlPoints.push_back(new vertex(-3.0f, 4.0f));
    curve1->CalculateCurve();
    m_vCurves.push_back(curve1);

    Curve* curve2 = new Curve();
    curve2->SetType(BEZIER);
    curve2->m_function->ClearControlPoints();
    curve2->m_function->controlPoints.push_back(new vertex(0.0f, 8.0f));
    curve2->m_function->controlPoints.push_back(new vertex(1.0f, 5.0f));
    curve2->m_function->controlPoints.push_back(new vertex(3.0f, 4.0f));
    curve2->CalculateCurve();
    m_vCurves.push_back(curve2);

    Curve* curve3 = new Curve();
    curve3->SetType(BEZIER);
    curve3->m_function->ClearControlPoints();
    curve3->m_function->controlPoints.push_back(new vertex(0.0f, 8.0f));
    curve3->m_function->controlPoints.push_back(new vertex(-1.0f, 5.0f));
    curve3->m_function->controlPoints.push_back(new vertex(-3.0f, 4.0f));
    curve3->CalculateCurve();
    m_vCurves.push_back(curve3);
}

void Graph::InitHeart()
{
    Curve* curve = new Curve();
    curve->SetType(ARC_CIRCLE);
    curve->m_function->ClearControlPoints();
    curve->m_function->controlPoints.push_back(new vertex(2.0f, 6.0f));
    curve->m_function->controlPoints.push_back(new vertex(4.0f, 6.0f));
    curve->m_function->controlPoints.push_back(new vertex(0.0f, 6.0f));
    curve->CalculateCurve();
    m_vCurves.push_back(curve);

    Curve* curve1 = new Curve();
    curve1->SetType(ARC_CIRCLE);
    curve1->m_function->ClearControlPoints();
    curve1->m_function->controlPoints.push_back(new vertex(-2.0f, 6.0f));
    curve1->m_function->controlPoints.push_back(new vertex(0.0f, 6.0f));
    curve1->m_function->controlPoints.push_back(new vertex(-4.0f, 6.0f));
    curve1->CalculateCurve();
    m_vCurves.push_back(curve1);

    Curve* curve2 = new Curve();
    curve2->SetType(BEZIER);
    curve2->m_function->ClearControlPoints();
    curve2->m_function->controlPoints.push_back(new vertex(0.0f, 0.0f));
    curve2->m_function->controlPoints.push_back(new vertex(2.0f, 2.0f));
    curve2->m_function->controlPoints.push_back(new vertex(4.0f, 4.0f));
    curve2->m_function->controlPoints.push_back(new vertex(4.0f, 6.0f));
    curve2->CalculateCurve();
    m_vCurves.push_back(curve2);

    Curve* curve3 = new Curve();
    curve3->SetType(BEZIER);
    curve3->m_function->ClearControlPoints();
    curve3->m_function->controlPoints.push_back(new vertex(0.0f, 0.0f));
    curve3->m_function->controlPoints.push_back(new vertex(-2.0f, 2.0f));
    curve3->m_function->controlPoints.push_back(new vertex(-4.0f, 4.0f));
    curve3->m_function->controlPoints.push_back(new vertex(-4.0f, 6.0f));
    curve3->CalculateCurve();
    m_vCurves.push_back(curve3);
}

void Graph::InitSpade()
{
    Curve* curve = new Curve();
    curve->SetType(LINEAR);
    curve->m_function->ClearControlPoints();
    curve->m_function->controlPoints.push_back(new vertex(-0.6f, 0.0f));
    curve->m_function->controlPoints.push_back(new vertex(0.6f, 0.0f));
    curve->CalculateCurve();
    m_vCurves.push_back(curve);

    Curve* curve1 = new Curve();
    curve1->SetType(BEZIER);
    curve1->m_function->ClearControlPoints();
    curve1->m_function->controlPoints.push_back(new vertex(0.6f, 0.0f));
    curve1->m_function->controlPoints.push_back(new vertex(0.3f, 0.7f));
    curve1->m_function->controlPoints.push_back(new vertex(0.4f, 1.3f));
    curve1->CalculateCurve();
    m_vCurves.push_back(curve1);

    Curve* curve2 = new Curve();
    curve2->SetType(BEZIER);
    curve2->m_function->ClearControlPoints();
    curve2->m_function->controlPoints.push_back(new vertex(-0.6f, 0.0f));
    curve2->m_function->controlPoints.push_back(new vertex(-0.3f, 0.7f));
    curve2->m_function->controlPoints.push_back(new vertex(-0.4f, 1.3f));
    curve2->CalculateCurve();
    m_vCurves.push_back(curve2);

    Curve* curve3 = new Curve();
    curve3->SetType(ARC_CIRCLE);
    curve3->m_function->ClearControlPoints();
    curve3->m_function->controlPoints.push_back(new vertex(1.0f, 2.0f));
    curve3->m_function->controlPoints.push_back(new vertex(0.4f, 1.3f));
    curve3->m_function->controlPoints.push_back(new vertex(1.6f, 2.7f));
    curve3->CalculateCurve();
    m_vCurves.push_back(curve3);

    Curve* curve4 = new Curve();
    curve4->SetType(ARC_CIRCLE);
    curve4->m_function->ClearControlPoints();
    curve4->m_function->controlPoints.push_back(new vertex(-1.0f, 2.0f));
    curve4->m_function->controlPoints.push_back(new vertex(-1.6f, 2.7f));
    curve4->m_function->controlPoints.push_back(new vertex(-0.4f, 1.3f));
    curve4->CalculateCurve();
    m_vCurves.push_back(curve4);

    Curve* curve5 = new Curve();
    curve5->SetType(BEZIER);
    curve5->m_function->ClearControlPoints();
    curve5->m_function->controlPoints.push_back(new vertex(1.6f, 2.7f));
    curve5->m_function->controlPoints.push_back(new vertex(0.6f, 3.5f));
    curve5->m_function->controlPoints.push_back(new vertex(0.0f, 4.5f));
    curve5->m_function->controlPoints.push_back(new vertex(0.0f, 5.4f));
    curve5->CalculateCurve();
    m_vCurves.push_back(curve5);

    Curve* curve6 = new Curve();
    curve6->SetType(BEZIER);
    curve6->m_function->ClearControlPoints();
    curve6->m_function->controlPoints.push_back(new vertex(-1.6f, 2.7f));
    curve6->m_function->controlPoints.push_back(new vertex(-0.6f, 3.5f));
    curve6->m_function->controlPoints.push_back(new vertex(-0.0f, 4.5f));
    curve6->m_function->controlPoints.push_back(new vertex(-0.0f, 5.4f));
    curve6->CalculateCurve();
    m_vCurves.push_back(curve6);
}

void Graph::InitClub()
{
    Curve* curve = new Curve();
    curve->SetType(LINEAR);
    curve->m_function->ClearControlPoints();
    curve->m_function->controlPoints.push_back(new vertex(-1.0f, 0.0f));
    curve->m_function->controlPoints.push_back(new vertex(1.0f, 0.0f));
    curve->CalculateCurve();
    m_vCurves.push_back(curve);

    Curve* curve1 = new Curve();
    curve1->SetType(BEZIER);
    curve1->m_function->ClearControlPoints();
    curve1->m_function->controlPoints.push_back(new vertex(1.0f, 0.0f));
    curve1->m_function->controlPoints.push_back(new vertex(0.6f, 0.3f));
    curve1->m_function->controlPoints.push_back(new vertex(0.4f, 2.0f));
    curve1->CalculateCurve();
    m_vCurves.push_back(curve1);
    
    Curve* curve2 = new Curve();
    curve2->SetType(BEZIER);
    curve2->m_function->ClearControlPoints();
    curve2->m_function->controlPoints.push_back(new vertex(-1.0f, 0.0f));
    curve2->m_function->controlPoints.push_back(new vertex(-0.6f, 0.3f));
    curve2->m_function->controlPoints.push_back(new vertex(-0.4f, 2.0f));
    curve2->CalculateCurve();
    m_vCurves.push_back(curve2);

    Curve* curve3 = new Curve();
    curve3->SetType(HERMITE);
    curve3->m_function->ClearControlPoints();
    curve3->m_function->controlPoints.push_back(new vertex(0.4f, 2.0f));
    curve3->m_function->controlPoints.push_back(new vertex(1.0f, 1.7f));
    curve3->m_function->controlPoints.push_back(new vertex(1.1f, 2.0f));
    curve3->m_function->controlPoints.push_back(new vertex(0.0f, 2.3f));
    curve3->CalculateCurve();
    m_vCurves.push_back(curve3);

    Curve* curve4 = new Curve();
    curve4->SetType(HERMITE);
    curve4->m_function->ClearControlPoints();
    curve4->m_function->controlPoints.push_back(new vertex(-1.0f, 1.7f));
    curve4->m_function->controlPoints.push_back(new vertex(-0.4f, 2.0f));
    curve4->m_function->controlPoints.push_back(new vertex(-0.0f, 2.3f));
    curve4->m_function->controlPoints.push_back(new vertex(-1.1f, 2.0f));
    curve4->CalculateCurve();
    m_vCurves.push_back(curve4);

    Curve* curve5 = new Curve();
    curve5->SetType(ARC_CIRCLE);
    curve5->m_function->ClearControlPoints();
    curve5->m_function->controlPoints.push_back(new vertex(1.5f, 2.5f));
    curve5->m_function->controlPoints.push_back(new vertex(1.0f, 1.7f));
    curve5->m_function->controlPoints.push_back(new vertex(1.0f, 3.3f));
    curve5->CalculateCurve();
    m_vCurves.push_back(curve5);

    Curve* curve6 = new Curve();
    curve6->SetType(ARC_CIRCLE);
    curve6->m_function->ClearControlPoints();
    curve6->m_function->controlPoints.push_back(new vertex(-1.5f, 2.5f));
    curve6->m_function->controlPoints.push_back(new vertex(-1.0f, 3.3f));
    curve6->m_function->controlPoints.push_back(new vertex(-1.0f, 1.7f));
    curve6->CalculateCurve();
    m_vCurves.push_back(curve6);

    Curve* curve7 = new Curve();
    curve7->SetType(HERMITE);
    curve7->m_function->ClearControlPoints();
    curve7->m_function->controlPoints.push_back(new vertex(0.4f, 2.9f));
    curve7->m_function->controlPoints.push_back(new vertex(1.0f, 3.3f));
    curve7->m_function->controlPoints.push_back(new vertex(1.1f, 2.9f));
    curve7->m_function->controlPoints.push_back(new vertex(0.0f, 2.7f));
    curve7->CalculateCurve();
    m_vCurves.push_back(curve7);

    Curve* curve8 = new Curve();
    curve8->SetType(HERMITE);
    curve8->m_function->ClearControlPoints();
    curve8->m_function->controlPoints.push_back(new vertex(-1.0f, 3.3f));
    curve8->m_function->controlPoints.push_back(new vertex(-0.4f, 2.9f));
    curve8->m_function->controlPoints.push_back(new vertex(-0.0f, 2.7f));
    curve8->m_function->controlPoints.push_back(new vertex(-1.1f, 2.9f));
    curve8->CalculateCurve();
    m_vCurves.push_back(curve8);

    Curve* curve9 = new Curve();
    curve9->SetType(ARC_CIRCLE);
    curve9->m_function->ClearControlPoints();
    curve9->m_function->controlPoints.push_back(new vertex(0.0f, 4.3f));
    curve9->m_function->controlPoints.push_back(new vertex(0.6f, 3.6f));
    curve9->m_function->controlPoints.push_back(new vertex(-0.6f, 3.6f));
    curve9->CalculateCurve();
    m_vCurves.push_back(curve9);

    Curve* curve10 = new Curve();
    curve10->SetType(BEZIER);
    curve10->m_function->ClearControlPoints();
    curve10->m_function->controlPoints.push_back(new vertex(0.4f, 2.9f));
    curve10->m_function->controlPoints.push_back(new vertex(0.3f, 3.3f));
    curve10->m_function->controlPoints.push_back(new vertex(0.6f, 3.6f));
    curve10->CalculateCurve();
    m_vCurves.push_back(curve10);

    Curve* curve11 = new Curve();
    curve11->SetType(BEZIER);
    curve11->m_function->ClearControlPoints();
    curve11->m_function->controlPoints.push_back(new vertex(-0.4f, 2.9f));
    curve11->m_function->controlPoints.push_back(new vertex(-0.3f, 3.3f));
    curve11->m_function->controlPoints.push_back(new vertex(-0.6f, 3.6f));
    curve11->CalculateCurve();
    m_vCurves.push_back(curve11);
}

void Graph::InitBezier()
{
    BezierCurve* bezierCurve = new BezierCurve();
    bezierCurve->controlPoints.push_back(new vertex(2.0f, 17.0f));
    bezierCurve->controlPoints.push_back(new vertex(6.6f, 1.8f));
    bezierCurve->controlPoints.push_back(new vertex(61.0f, 7.4f));
    bezierCurve->controlPoints.push_back(new vertex(12.0f, 37.0f));
    bezierCurve->controlPoints.push_back(new vertex(62.0f, 17.0f));

    Curve* curve = new Curve();
    curve->CalculateCurve(bezierCurve);
    m_vCurves.push_back(curve);
}

void Graph::TraceCourbe(FunctionType type, std::vector<vertex*> points, bool isMirorO, bool isMirorX, bool isMirorY)
{
    Curve* curve = new Curve();
    curve->SetType(type);
    curve->m_function->ClearControlPoints();
    curve->m_function->controlPoints = points;
    
    m_vCurves.push_back(curve);

    if (isMirorO)
    {
        SymetryAxis* sa = curve->AddSymetry();
        sa->controlPoints[0]->x = -0.5f;
        sa->controlPoints[0]->y = 0.5f;
        sa->controlPoints[1]->x = 0.5f;
        sa->controlPoints[1]->y = -0.5f;
    }

    if (isMirorX)
    {
        SymetryAxis* sa = curve->AddSymetry();
        sa->controlPoints[0]->x = 0.f;
        sa->controlPoints[0]->y = 0.f;
        sa->controlPoints[1]->x = 1.f;
        sa->controlPoints[1]->y = 0.f;
    }

    if (isMirorY)
    {
        SymetryAxis* sa = curve->AddSymetry();
        sa->controlPoints[0]->x = 0.f;
        sa->controlPoints[0]->y = 0.f;
        sa->controlPoints[1]->x = 0.f;
        sa->controlPoints[1]->y = 1.f;
    }

    curve->CalculateCurve();
}


#endif
