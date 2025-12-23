#include "pch.h"
#ifndef GRAPH_CPP_DEFINED
#define GRAPH_CPP_DEFINED

#include "Graph.h"

#include "Debug.h"
#include "Utils.h"
#include "MathFunctions.hpp"

void Graph::OnInitialize()
{
    Diamond f;
    f.origin = sf::Vector2f(10, 10);
    f.a = 2;
    f.b = 3;
    curve1.CalculateShape(f);
}

void Graph::OnEvent(const sf::Event& event)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            m_isMousePressed = true;
    }

    if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
            m_isMousePressed = false;
    }
}

void Graph::OnUpdate()
{
    HandleMouseMovement();
    DrawGraph();

    curve1.DrawPath(sf::Color::Blue);
    // curve1.DrawPoints(sf::Color::Blue, 5.0f);
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
}

void Graph::HandleMouseMovement()
{
    m_lastMousePos = m_mousePos;
    m_mousePos = sf::Mouse::getPosition();
    
    if (m_isMousePressed == false) return;

    sf::Vector2f mouseDelta = {(float)(m_lastMousePos.x - m_mousePos.x), -(float)(m_lastMousePos.y - m_mousePos.y)};
    mouseDelta *= m_movementSpeed * GameManager::Get()->GetDeltaTime();

    GameManager::Get()->GetView()->move(mouseDelta.x, -mouseDelta.y);
}


#endif
