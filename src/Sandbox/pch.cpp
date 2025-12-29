#include "pch.h"

#include "Debug.h"
#include "Utils.h"

vertex::vertex(float px, float py)
{
    x = px;
    y = py;
}


void vertex::Draw(sf::Color color)
{
    Debug::DrawCircle(x * TILE_SIZE, -y * TILE_SIZE, radius, color);
}

bool vertex::Contains(float px, float py)
{
    return Utils::GetDistance(x * TILE_SIZE, -y * TILE_SIZE, px, py) < radius;
}

