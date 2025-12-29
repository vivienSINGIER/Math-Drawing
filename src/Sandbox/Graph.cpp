#include "pch.h"
#ifndef GRAPH_CPP_DEFINED
#define GRAPH_CPP_DEFINED

#include "Graph.h"

#include <SFML/Window/Window.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Debug.h"
#include "Utils.h"
#include "MathFunctions.hpp"

void Graph::OnInitialize()
{
    //InitDiamond();
    //InitHeart();
    // InitEllipse();
    // InitBezier();

    Curve* curve = new Curve;
    curve->SetType(ARC_CIRCLE);
    curve->CalculateCurve();

    m_selectedCurve = curve;
    
    m_vCurves.push_back(curve);
    
    //Zoom
    m_pView = GameManager::Get()->GetView();
    m_pUiView = GameManager::Get()->GetUIView();
    m_windowSize = m_pView->getSize();
    m_pWindow = GameManager::Get()->GetWindow();
}

void Graph::OnEvent(const sf::Event& event)
{
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

    //test zoom
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
    }
}

void Graph::OnUpdate()
{
    HandleMouseMovement();
    DrawGraph();

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

    m_selectedCurve->m_function->SetVertexPosition(m_selectedVertex, worldPos.x / TILE_SIZE, -worldPos.y / TILE_SIZE);
    m_selectedCurve->CalculateCurve();
}

void Graph::DrawGraph()
{
    Debug::DrawLine(m_minX * TILE_SIZE, 0.0f, m_maxX * TILE_SIZE, 0.0f, sf::Color::White);
    Debug::DrawLine(0.0f, m_minY * TILE_SIZE, 0.0f, m_maxY * TILE_SIZE, sf::Color::White);
    
    float tempX = m_minX;
    while (tempX <= m_maxX)
    {
        float size = 10.0f;
        if (tempX == m_maxX || tempX == m_minX)
            size *= 1.5f;
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
        Debug::DrawLine(-size, tempY * TILE_SIZE, size, tempY * TILE_SIZE, sf::Color::White);
        if ((int)tempY != 0)
            Debug::DrawText(-35.f, tempY * TILE_SIZE - 2.5f, std::to_string((int)-tempY), sf::Color::Green);
        tempY += m_interval_Y;
    }

    std::string typeStr = std::string(magic_enum::enum_name(m_selectedType));

    sf::Vector2f txtPos = m_pUiView->getCenter() - m_pUiView->getSize() / 2.f;
    Debug::DrawText(txtPos.x, txtPos.y, typeStr, sf::Color::Magenta, 20.0f, true);
}

void Graph::HandleMouseMovement()
{
    m_lastMousePos = m_mousePos;
    m_mousePos = sf::Mouse::getPosition();
    
    if (m_isMousePressed == false) return;

    sf::Vector2f mouseDelta = {(float)(m_lastMousePos.x - m_mousePos.x), -(float)(m_lastMousePos.y - m_mousePos.y)};
    mouseDelta *= m_movementSpeed * GameManager::Get()->GetDeltaTime();

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
    
    vertex* newSelected = m_selectedCurve->m_function->HandleSelection(worldPos.x, worldPos.y);

    if (newSelected != nullptr)
    {
        m_selectedVertex = newSelected;
        m_selectedVertex->isSelected = true;
    }
    
}

void Graph::InitDiamond()
{
    // sf::Vector2f origin = sf::Vector2f(2.5f, 0.5f);
    //
    // Curve* curve = new Curve();
    // curve->origin = origin;
    // Hermite* hermite = new Hermite();
    // hermite->v1 = sf::Vector2f(-2.0f, 3.0f);
    // hermite->fp1 = 0.3f;
    // hermite->v2 = sf::Vector2f(0.0f, 6.0f);
    // hermite->fp2 = 2.0f;
    // curve->CalculateCurve(-2.0f, 0.0f, 50, hermite);
    // m_vCurves.push_back(curve);
    //
    // Curve* curve2 = new Curve();
    // curve2->origin = origin;
    // Hermite* hermite2 = new Hermite();
    // hermite2->v1 = sf::Vector2f(0.0f, 6.0f);
    // hermite2->fp1 = -2.0f;
    // hermite2->v2 = sf::Vector2f(2.0f, 3.0f);
    // hermite2->fp2 = -0.3f;
    // curve2->CalculateCurve(0.0f, 2.0f, 50, hermite2);
    // m_vCurves.push_back(curve2);
    //
    // Curve* curve3 = new Curve();
    // curve3->origin = origin;
    // Hermite* hermite3 = new Hermite();
    // hermite3->v1 = sf::Vector2f(-2.0f, 3.0f);
    // hermite3->fp1 = -0.3f;
    // hermite3->v2 = sf::Vector2f(0.0f, 0.0f);
    // hermite3->fp2 = -2.0f;
    // curve3->CalculateCurve(-2.0f, 0.0f, 50, hermite3);
    // m_vCurves.push_back(curve3);
    //
    // Curve* curve4 = new Curve();
    // curve4->origin = origin;
    // Hermite* hermite4 = new Hermite();
    // hermite4->v1 = sf::Vector2f(0.0f, 0.0f);
    // hermite4->fp1 = 2.0f;
    // hermite4->v2 = sf::Vector2f(2.0f, 3.0f);
    // hermite4->fp2 = 0.3f;
    // curve4->CalculateCurve(0.0f, 2.0f, 50, hermite4);
    // m_vCurves.push_back(curve4);
}

void Graph::InitHeart()
{
    // Curve* curve = new Curve();
    // ArcOfCircle* arcOfCircle = new ArcOfCircle();
    // arcOfCircle->origin = sf::Vector2f(-1.0f, 4.0f);
    // arcOfCircle->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // arcOfCircle->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // arcOfCircle->minAngle = PI / 4.0f;
    // arcOfCircle->maxAngle = PI;
    // curve->CalculateShape(arcOfCircle);
    // m_vCurves.push_back(curve);
    //
    // Curve* curve1 = new Curve;
    // ArcOfCircle* arcOfCircle1 = new ArcOfCircle();
    // arcOfCircle1->origin = sf::Vector2f(1.0f, 4.0f);
    // arcOfCircle1->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // arcOfCircle1->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // arcOfCircle1->minAngle = 0.0f;
    // arcOfCircle1->maxAngle = PI - PI / 4.0f;
    // curve1->CalculateShape(arcOfCircle1);
    // m_vCurves.push_back(curve1);
    //
    // Curve* curve2 = new Curve();
    // Hermite* hermite1 = new Hermite();
    // hermite1->v1 = sf::Vector2f(-1.0f - Utils::Sqrt(2), 4.0f);
    // hermite1->fp1 = -2.0f;
    // hermite1->v2 = sf::Vector2f(0.0f, 0.0f);
    // hermite1->fp2 = -0.9f;
    // curve2->CalculateCurve(-1.0f - Utils::Sqrt(2), 0.0f, 50, hermite1);
    // m_vCurves.push_back(curve2);
    //
    // Curve* curve3 = new Curve();
    // Hermite* hermite2 = new Hermite();
    // hermite2->v1 = sf::Vector2f(1.0f + Utils::Sqrt(2), 4.0f);
    // hermite2->fp1 = 2.0f;
    // hermite2->v2 = sf::Vector2f(0.0f, 0.0f);
    // hermite2->fp2 = 0.9f;
    // curve3->CalculateCurve(1.0f + Utils::Sqrt(2), 0.0f, 50, hermite2);
    // m_vCurves.push_back(curve3);
}

void Graph::InitEllipse()
{
    // Ellipse* ellipse = new Ellipse();
    // ellipse->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // ellipse->controlPoints.push_back(new vertex(Utils::Sqrt(2), Utils::Sqrt(2)));
    // ellipse->origin = { 0.f, 2.f };
    //
    // Curve* curve = new Curve();
    // curve->CalculateShape(ellipse);
    //
    // m_vCurves.push_back(curve);
}

void Graph::InitSpade()
{

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


#endif
