#pragma once

#ifdef _DEBUG
#include <crtdbg.h>
#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DEBUG_NEW
#endif

#define TILE_SIZE 50.0f
#define PI 3.141592653589f
#define INF 1e6f

struct vertex
{
    float x;
    float y;

    vertex operator-()
    {
        vertex newVertex;
        newVertex.x = -x;
        newVertex.y = -y;

        return newVertex;
    }
};