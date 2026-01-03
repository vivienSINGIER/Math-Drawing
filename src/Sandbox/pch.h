#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif

#define TILE_SIZE 50.0f
#define PI 3.141592653589f
#define INF 1e6f

#define TO_STRING(x) #x

#include <SFML/Graphics/Color.hpp>
#include <magic_enum.hpp>

struct vertex
{
    vertex() = default;
    vertex(float x, float y);
    
    float x;
    float y;

    float radius = 5.0f;
    bool isSelected = false;
    
    void Draw(sf::Color color);
    bool Contains(float x, float y);
};
