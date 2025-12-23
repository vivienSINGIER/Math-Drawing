#include "pch.h"
#ifndef CURVE_CPP_DEFINED
#define CURVE_CPP_DEFINED
#include "Curve.h"

#include "Debug.h"

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

void Curve::DrawPoints(sf::Color color, float radius)
{
    for (vertex v : m_vertices)
    {
        Debug::DrawCircle(v.x * TILE_SIZE, -v.y * TILE_SIZE, radius, color);
    }
}

#endif