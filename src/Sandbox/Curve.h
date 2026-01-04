#include "pch.h"
#ifndef CURVE_H_DEFINED
#define CURVE_H_DEFINED
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "MathFunctions.hpp"

class Curve 
{
public:
    Curve() = default;
    ~Curve() = default;

    FunctionType m_type = LINEAR;
    
    sf::Vector2f origin;
    std::vector<vertex> m_vertices;
    std::vector<SymetryAxis*> m_vSymetries;
    
    MathFunction* m_function = nullptr;

    FunctionType GetFunctionType() { return m_type; }
    void SetType(FunctionType type);
    
    void DrawPath(sf::Color color);
    void DrawPoints();
    
    void CalculateCurve(MathFunction* func = nullptr);
    vertex* HandleSelection(float x, float y);
    SymetryAxis* AddSymetry();

    void RemoveVertex(vertex* vertexToRemove);
};

#include "Curve.inl"

#endif
