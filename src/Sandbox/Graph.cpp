#include "pch.h"
#ifndef GRAPH_CPP_DEFINED
#define GRAPH_CPP_DEFINED

#include "Graph.h"

#include "Debug.h"
#include "Utils.h"
#include "MathFunctions.hpp"
#include "Interface.h"

#include <iostream>

void Graph::OnInitialize()
{
    //InitDiamond();
    //InitHeart();
    //InitEllipse();
    //InitSpade();
    //InitClover();

    //InitBezier();
    
    //Zoom
    m_pView = GameManager::Get()->GetView();
    m_windowSize = m_pView->getSize();

    //Test trace courbe
    std::vector<vertex> test = {
        {2.0f, 17.0f},
        {6.6f, 1.8f},
        {61.0f, 7.4f},
        {12.0f, 37.0f},
        {62.0f, 17.0f}
    };

    m_pInterface = new Interface;
    m_pInterface->m_pGraph = this;

    //TraceCourbe(TypeBezier, test, {}, true);

    //TraceCourbe(TypeLagrange, { {0.f, 0.f}, {5.f, 5.f} }, {}, true);

    //TraceCourbe(TypeHermite, { {0.f, 0.f}, {5.f, 5.f} }, { {0.f, 2.f}, {5.f, 8.f} }, true);
}

void Graph::OnEvent(const sf::Event& event)
{
    m_pInterface->OnEvent(event);

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
}

void Graph::OnUpdate()
{
    HandleMouseMovement();
    DrawGraph();
    m_pInterface->Update();

    for (Curve &curve : vCurves)
    {
        curve.DrawPath();
    }
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
    mouseDelta *= m_movementSpeed * GameManager::Get()->GetDeltaTime() * m_currentZoom;

    m_pView->move(mouseDelta.x, -mouseDelta.y);
}

void Graph::InitDiamond()
{
    sf::Vector2f origin = sf::Vector2f(2.5f, 0.5f);

    Curve curve;
    curve.origin = origin;
    Hermite hermite;
    hermite.v1 = sf::Vector2f(-2.0f, 3.0f);
    hermite.fp1 = 0.3f;
    hermite.v2 = sf::Vector2f(0.0f, 6.0f);
    hermite.fp2 = 2.0f;
    curve.CalculateCurve(-2.0f, 0.0f, 50, hermite);
    vCurves.push_back(curve);

    Curve curve2;
    curve2.origin = origin;
    Hermite hermite2;
    hermite2.v1 = sf::Vector2f(0.0f, 6.0f);
    hermite2.fp1 = -2.0f;
    hermite2.v2 = sf::Vector2f(2.0f, 3.0f);
    hermite2.fp2 = -0.3f;
    curve2.CalculateCurve(0.0f, 2.0f, 50, hermite2);
    vCurves.push_back(curve2);

    Curve curve3;
    curve3.origin = origin;
    Hermite hermite3;
    hermite3.v1 = sf::Vector2f(-2.0f, 3.0f);
    hermite3.fp1 = -0.3f;
    hermite3.v2 = sf::Vector2f(0.0f, 0.0f);
    hermite3.fp2 = -2.0f;
    curve3.CalculateCurve(-2.0f, 0.0f, 50, hermite3);
    vCurves.push_back(curve3);

    Curve curve4;
    curve4.origin = origin;
    Hermite hermite4;
    hermite4.v1 = sf::Vector2f(0.0f, 0.0f);
    hermite4.fp1 = 2.0f;
    hermite4.v2 = sf::Vector2f(2.0f, 3.0f);
    hermite4.fp2 = 0.3f;
    curve4.CalculateCurve(0.0f, 2.0f, 50, hermite4);
    vCurves.push_back(curve4);
}

void Graph::InitHeart()
{
    Curve curve;
    ArcOfCircle arcOfCircle;
    arcOfCircle.origin = sf::Vector2f(-1.0f, 4.0f);
    arcOfCircle.a = Utils::Sqrt(2);
    arcOfCircle.b = Utils::Sqrt(2);
    arcOfCircle.minAngle = PI / 4.0f;
    arcOfCircle.maxAngle = PI;
    curve.CalculateShape(arcOfCircle);
    vCurves.push_back(curve);

    Curve curve1;
    ArcOfCircle arcOfCircle1;
    arcOfCircle1.origin = sf::Vector2f(1.0f, 4.0f);
    arcOfCircle1.a = Utils::Sqrt(2);
    arcOfCircle1.b = Utils::Sqrt(2);
    arcOfCircle1.minAngle = 0.0f;
    arcOfCircle1.maxAngle = PI - PI / 4.0f;
    curve1.CalculateShape(arcOfCircle1);
    vCurves.push_back(curve1);

    Curve curve2;
    Hermite hermite1;
    hermite1.v1 = sf::Vector2f(-1.0f - Utils::Sqrt(2), 4.0f);
    hermite1.fp1 = -2.0f;
    hermite1.v2 = sf::Vector2f(0.0f, 0.0f);
    hermite1.fp2 = -0.9f;
    curve2.CalculateCurve(-1.0f - Utils::Sqrt(2), 0.0f, 50, hermite1);
    vCurves.push_back(curve2);

    Curve curve3;
    Hermite hermite2;
    hermite2.v1 = sf::Vector2f(1.0f + Utils::Sqrt(2), 4.0f);
    hermite2.fp1 = 2.0f;
    hermite2.v2 = sf::Vector2f(0.0f, 0.0f);
    hermite2.fp2 = 0.9f;
    curve3.CalculateCurve(1.0f + Utils::Sqrt(2), 0.0f, 50, hermite2);
    vCurves.push_back(curve3);
}

void Graph::InitEllipse()
{
    Ellipse ellipse;
    ellipse.a = 5.f;
    ellipse.b = 2.f;
    ellipse.origin = { 0.f, 2.f };

    Curve curve;
    curve.CalculateShape(ellipse);

    vCurves.push_back(curve);
}

void Graph::InitSpade()
{
    sf::Vector2f offset = sf::Vector2f(2.f, 1.f);

    Curve curveBase;
    Hermite hermiteBase;
    hermiteBase.v1 = sf::Vector2f(-0.5f, 0.f) + offset;
    hermiteBase.fp1 = 0.f;
    hermiteBase.v2 = sf::Vector2f(0.5f, 0.f) + offset;
    hermiteBase.fp2 = 0.f;
    curveBase.CalculateCurve(-0.5f + offset.x, 0.5f + offset.x, 10, hermiteBase);
    vCurves.push_back(curveBase);

    Curve curve1;
    ArcOfCircle arcOfCircle1;
    arcOfCircle1.origin = sf::Vector2f(-1.0f, 2.0f) + offset;
    arcOfCircle1.minAngle = 3 * PI / 4; //135
    arcOfCircle1.maxAngle = 7 * PI / 4; //315
    curve1.CalculateShape(arcOfCircle1);
    vCurves.push_back(curve1);

    Curve curve2;
    ArcOfCircle arcOfCircle2;
    arcOfCircle2.origin = sf::Vector2f(1.0f, 2.0f) + offset;
    arcOfCircle2.minAngle = PI + PI / 4;
    arcOfCircle2.maxAngle = 2 * PI + PI / 4;
    curve2.CalculateShape(arcOfCircle2);
    vCurves.push_back(curve2);

    Curve curve3;
    curve3.origin = offset;
    BezierCurve bezierCurve1;
    bezierCurve1.controlPoints = {
        {-0.5f, 0.f},
        {-0.40f, 0.5f},
        {-0.26f, 1.32f}
    };
    curve3.CalculateShape(bezierCurve1);
    vCurves.push_back(curve3);

    Curve curve4;
    curve4.origin = offset;
    BezierCurve bezierCurve2;
    bezierCurve2.controlPoints = {
        {0.5f, 0.f},
        {0.40f, 0.5f},
        {0.26f, 1.32f}
    };
    curve4.CalculateShape(bezierCurve2);
    vCurves.push_back(curve4);

    Curve curve5;
    curve5.origin = offset;
    BezierCurve bezierCurve3;
    bezierCurve3.controlPoints = {
        {-1.71f, 2.71f},
        {-0.1f, 4.5f},
        {0.f, 5.f}
    };
    curve5.CalculateShape(bezierCurve3);
    vCurves.push_back(curve5);

    Curve curve6;
    curve6.origin = offset;
    BezierCurve bezierCurve4;
    bezierCurve4.controlPoints = {
        {1.71f, 2.71f},
        {0.1f, 4.5f},
        {0.f, 5.f}
    };
    curve6.CalculateShape(bezierCurve4);
    vCurves.push_back(curve6);
}

void Graph::InitClover()
{
    sf::Vector2f offset = sf::Vector2f(5.f, 3.f);

    Curve curveBase;
    Hermite hermiteBase;
    hermiteBase.v1 = sf::Vector2f(-1.f, 0.f);
    hermiteBase.fp1 = 0.f;
    hermiteBase.v2 = sf::Vector2f(1.f, 0.f);
    hermiteBase.fp2 = 0.f;
    curveBase.origin = offset;
    curveBase.CalculateCurve(-1.f, 1.f, 20, hermiteBase);
    vCurves.push_back(curveBase);

    Curve curveHead;
    curveHead.origin = offset;
    ArcOfCircle arcHead;
    arcHead.origin = sf::Vector2f(0.f, 4.25f);
    arcHead.minAngle = - PI / 4;
    arcHead.maxAngle = 5 * PI / 4;
    curveHead.CalculateShape(arcHead);
    vCurves.push_back(curveHead);

    Curve curve1;
    curve1.origin = offset;
    ArcOfCircle arc1;
    arc1.origin = sf::Vector2f(-1.5f, 2.5f);
    arc1.minAngle = PI / 4;
    arc1.maxAngle = 7 * PI / 4;
    curve1.CalculateShape(arc1);
    vCurves.push_back(curve1);

    Curve curve2;
    curve2.origin = offset;
    ArcOfCircle arc2;
    arc2.origin = sf::Vector2f(1.5f, 2.5f);
    arc2.minAngle = - 3 * PI / 4;
    arc2.maxAngle = 3 * PI / 4;
    curve2.CalculateShape(arc2);
    vCurves.push_back(curve2);

    Curve curve3;
    curve3.origin = offset;
    BezierCurve bezier1;
    bezier1.controlPoints = {
        {-1.f, 0.f},
        {-0.7f, 0.4f},
        {-0.4f, 2.f},
        {-0.4f, 2.f},
        {-0.4f, 2.f},
        {-0.8f, 1.79f}
    };
    curve3.CalculateShape(bezier1);
    vCurves.push_back(curve3);

    Curve curve4;
    curve4.origin = offset;
    BezierCurve bezier2;
    bezier2.controlPoints = {
        {1.f, 0.f},
        {0.7f, 0.4f},
        {0.4f, 2.f},
        {0.4f, 2.f},
        {0.4f, 2.f},
        {0.8f, 1.79f}
    };
    curve4.CalculateShape(bezier2);
    vCurves.push_back(curve4);

    Curve curve5;
    curve5.origin = offset;
    BezierCurve bezier3;
    bezier3.controlPoints = {
        {-0.8f, 3.21f},
        {-0.45f, 3.f},
        {-0.45f, 3.f},
        {-0.45f, 3.f},
        {-0.71f, 3.55f}
    };
    curve5.CalculateShape(bezier3);
    vCurves.push_back(curve5);

    Curve curve6;
    curve6.origin = offset;
    BezierCurve bezier4;
    bezier4.controlPoints = {
        {0.8f, 3.21f},
        {0.45f, 3.f},
        {0.45f, 3.f},
        {0.45f, 3.f},
        {0.71f, 3.55f}
    };
    curve6.CalculateShape(bezier4);
    vCurves.push_back(curve6);
}

void Graph::InitBezier()
{
    BezierCurve bezierCurve;
    bezierCurve.controlPoints = {
        {2.0f, 17.0f},
        {6.6f, 1.8f},
        {61.0f, 7.4f},
        {12.0f, 37.0f},
        {62.0f, 17.0f}
    };

    Curve curve;
    curve.CalculateShape(bezierCurve);
    vCurves.push_back(curve);
}

void Graph::TraceCourbe(Type type, std::vector<vertex> points, std::vector<vertex> deriv1Points, bool isMiror)
{
    Curve curve;
    switch (type)
    {
    case TypeBezier:
    {
        BezierCurve bezierCurve;
        bezierCurve.controlPoints = points;

        curve.CalculateShape(bezierCurve);
        break;
    }
    case TypeLagrange:
    {
        Lagrange lagrange;
        
        std::vector<float> x;
        std::vector<float> y;

        float minX = points[0].x;
        float maxX = points[0].x;

        for (vertex vertex : points)
        {
            x.push_back(vertex.x);
            y.push_back(vertex.y);

            if (minX > vertex.x)
                minX = vertex.x;

            else if (maxX < vertex.x)
                maxX = vertex.x;
        }

        lagrange.xs = x;
        lagrange.ys = y;

        curve.CalculateCurve(minX, maxX, 50, lagrange);
        break;
    }
    case TypeHermite:
    {
        if (points.size() != 2 || deriv1Points.size() != 2) return;

        Hermite hermite;

        hermite.v1.x = points[0].x;
        hermite.v1.y = points[0].y;
        hermite.fp1 = deriv1Points[0].y;

        hermite.v2.x = points[1].x;
        hermite.v2.y = points[1].y;
        hermite.fp2 = deriv1Points[1].y;

        float minX = hermite.v1.x;
        float maxX = hermite.v2.x;

        if (minX > maxX)
        {
            float temp = minX;
            minX = maxX;
            maxX = temp;
        }

        curve.CalculateCurve(minX, maxX, 50, hermite);
        break;
    }
    }

    
    vCurves.push_back(curve);

    if (isMiror) // only on y = 0 and x = 0
    {
        std::vector<vertex> altPoints;

        for (int i = 0; i < points.size(); ++i)
        {
            altPoints.push_back(-points[i]);
        }

        TraceCourbe(type, altPoints, deriv1Points);
    }
}


#endif
