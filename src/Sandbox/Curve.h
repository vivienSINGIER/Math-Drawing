#include "pch.h"
#ifndef CURVE_H_DEFINED
#define CURVE_H_DEFINED
#include <vector>
#include <SFML/Graphics/Color.hpp>

class Curve 
{
public:
    Curve() = default;
    ~Curve() = default;
    
    std::vector<vertex> m_vertices;

    void DrawPath(sf::Color color);
    void DrawPoints(sf::Color color, float radius);
    
    template <typename Func>
    void CalculateCurve(float xmin, float xmax, int n, Func&& func);

    template <typename Func>
    void CalculateShape(Func&& func);    

};

#include "Curve.inl"

#endif
