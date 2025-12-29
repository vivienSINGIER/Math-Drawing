#include "pch.h"
#ifndef CURVE_CPP_DEFINED
#define CURVE_CPP_DEFINED
#include "Curve.h"

#include "Debug.h"

void Curve::SetType(FunctionType type)
{
    if (m_function != nullptr)
    {
        delete m_function;
        m_function = nullptr;
    }

    m_type = type;
    
    switch (type)
    {
    case LINEAR:
        m_function = new Linear();
        break;
    case PARABOLA:
        m_function = new Parabola();
        break;
    case LAGRANGE:
        m_function = new Lagrange();
        break;
    case HERMITE:
        m_function = new Hermite();
        break;
    case BEZIER:
        m_function = new BezierCurve();
        break;
    case ELLIPSE:
        m_function = new Ellipse();
        break;
    case ARC_CIRCLE:
        m_function = new ArcOfCircle();
        break;
    }
}

void Curve::DrawPath(sf::Color color)
{
    if (m_vertices.size() <= 1)
        return;
    
    for (int i = 0; i < m_vertices.size() - 1; ++i)
    {
        vertex v1 = m_vertices[i];
        vertex v2 = m_vertices[(i + 1)];
        
        Debug::DrawLine(v1.x * TILE_SIZE, -v1.y * TILE_SIZE, v2.x * TILE_SIZE, -v2.y * TILE_SIZE, color);
    }
}

void Curve::DrawPoints()
{
    if (m_function != nullptr)
    {
        m_function->OnDrawPoints();
        
        for (vertex* v : m_function->controlPoints)
        {
            bool b = v->isSelected;
            if (b)
                v->Draw(sf::Color::Cyan);
            else
                v->Draw(sf::Color::Yellow);
        }
    }
}

void Curve::CalculateCurve(MathFunction* f)
{
    if (f != nullptr)
    {
        if (m_function != nullptr)
        {
            delete m_function;
            m_function = f;
        }
    }
    
    m_vertices.clear();
    m_vertices = (*m_function)();

    for (vertex v : m_vertices)
    {
        v.x += origin.x ;
        v.y += origin.y ;
    }
}

#endif