#include "pch.h"
#ifndef CURVE_CPP_DEFINED
#define CURVE_CPP_DEFINED
#include "Curve.h"

#include "Debug.h"
#include "GameManager.h"
#include "Utils.h"

#include <iostream>

struct Segment
{
    sf::Vector2f start;
    sf::Vector2f end;
};

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
    //Update color
    if (randomColor)
    {
        //classic
        //color.r = (sin(time) + 1) * 255/2;
        //color.g = (cos(time) + 1) * 255 / 2;
        //color.b = (cos(time) + 1) * 255 / 2;

        //courbe
        float y = m_vertices[currentPos].y;

        float value = (y - minY) * 255.0f / (maxY - minY);
        currentPos += dir;

        if (currentPos == m_vertices.size() - 1)
        {
            dir = -1;
        }
        else if (currentPos == 0)
        {
            dir = 1;
        }

        float t = value / 255.f;
        float hue = t * 360.f;

        sf::Color c = Utils::HSVtoRGB(hue, 0.5f, 1.f);

        color.r = c.r;
        color.g = c.g;
        color.b = c.b;
    }
    //
    if (m_vertices.size() <= 1)
        return;
    
    for (int i = 0; i < m_vertices.size() - 1; ++i)
    {
        vertex v1 = m_vertices[i];
        vertex v2 = m_vertices[(i + 1)];
        
        std::vector<Segment> segments;
        segments.push_back({{v1.x, v1.y}, {v2.x,v2.y}});

        auto reflect = [&](const vertex& v, const sf::Vector2f& pos, const sf::Vector2f& dir)
        {
            sf::Vector2f p(v.x, v.y);
            sf::Vector2f rel = p - pos;
            sf::Vector2f parallel = Utils::Dot(rel, dir) * dir;
            sf::Vector2f perpendicular = rel - parallel;
            sf::Vector2f reflected = pos + parallel - perpendicular;
            return reflected;
        };
        
        for (int j = 0; j < m_vSymetries.size(); ++j)
        {
            SymetryAxis* sa = m_vSymetries[j];
            sf::Vector2f axisDir = sa->dir;
            Utils::Normalize(axisDir);
            sf::Vector2f axisPos = { sa->controlPoints[0]->x, sa->controlPoints[0]->y };

            Debug::DrawLine(sa->controlPoints[0]->x * TILE_SIZE, -sa->controlPoints[0]->y * TILE_SIZE,
                sa->controlPoints[1]->x * TILE_SIZE, -sa->controlPoints[1]->y * TILE_SIZE, sf::Color::Magenta);

            int size = segments.size();
            for (int k = 0; k < size; ++k)
            {
                sf::Vector2f V1 = segments[k].start;
                sf::Vector2f V2 = segments[k].end;
                V1 = reflect(vertex(V1.x, V1.y), axisPos, axisDir);
                V2 = reflect(vertex(V2.x, V2.y), axisPos, axisDir);

                segments.push_back({{V1.x, V1.y}, {V2.x, V2.y}});
            }
        }

        for (Segment& s : segments)
        {
            Debug::DrawLine(s.start.x * TILE_SIZE, -s.start.y * TILE_SIZE, s.end.x * TILE_SIZE, -s.end.y * TILE_SIZE, color);
        }
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

    for (SymetryAxis* sa : m_vSymetries)
    {
        for (vertex* v : sa->controlPoints)
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

    minY = m_vertices[0].y;
    maxY = m_vertices[0].y;

    for (vertex v : m_vertices)
    {
        v.x += origin.x ;
        v.y += origin.y ;

        if (v.y < minY)
            minY = v.y;
        if (v.y > maxY)
            maxY = v.y;
    }

    for (SymetryAxis* sa : m_vSymetries)
    {
        (*sa)();
    }
}

vertex* Curve::HandleSelection(float x, float y)
{
    vertex* selected = nullptr;

    if (m_function != nullptr)
    {
        selected = m_function->HandleSelection(x, y);
    }

    if (selected != nullptr) return selected;

    for (SymetryAxis* sa : m_vSymetries)
    {
        selected = sa->HandleSelection(x, y);
        if (selected != nullptr) return selected;
    }
        
    return selected;
}

SymetryAxis* Curve::AddSymetry()
{
    SymetryAxis* sa = new SymetryAxis();
    m_vSymetries.push_back(sa);
    return sa;
}

void Curve::RemoveVertex(vertex* vertexToRemove)
{
    m_function->RemoveControlPoint(vertexToRemove);
    CalculateCurve();
}

void Curve::ChangeRandomColor()
{
    randomColor = !randomColor;
}

#endif
