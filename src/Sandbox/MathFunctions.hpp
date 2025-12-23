#include "pch.h"
#ifndef MATH_FUNCTIONS_HPP_DEFINED
#define MATH_FUNCTIONS_HPP_DEFINED

#include <vector>
#include "Utils.h"

struct MathFunction
{
    virtual float operator()(float x) const
    {
        return x;
    }
};

struct Parabola : public MathFunction
{
    float operator()(float x) const override
    {
        return Utils::Pow(x, 2);
    }
};

struct MathShape
{
    virtual std::vector<vertex> operator()() const
    {
        return std::vector<vertex>();
    }
};

struct Ellipse : public MathShape
{
    int n = 50;
    
    float a = 1.0f;
    float b = 1.0f;

    sf::Vector2f origin = sf::Vector2f(0, 0);
    
    std::vector<vertex> operator()() const override
    {
        std::vector<vertex> result;
        
        float tMax = 2 * PI;
        float tStep = tMax / n;
        float tTemp = 0.0f;

        while (tTemp < tMax + tStep)
        {
            vertex v;
            v.x = origin.x + a * cos(tTemp);
            v.y = origin.y + b * sin(tTemp);
            result.push_back(v);
            tTemp += tStep;
        }
        return result;
    }
};

struct Diamond : public MathShape
{
    int n = 50;
    
    float a = 1.0f;
    float b = 1.0f;

    sf::Vector2f origin = sf::Vector2f(0, 0);
    
    std::vector<vertex> operator()() const override
    {
        std::vector<vertex> result;
        
        float tMax = 2 * PI;
        float tStep = tMax / n;
        float tTemp = 0.0f;

        while (tTemp < tMax + tStep)
        {
            vertex v;
            v.x = origin.x + a * pow(cos(tTemp), 3);
            v.y = origin.y + b * pow(sin(tTemp), 3);
            result.push_back(v);
            tTemp += tStep;
        }
        return result;
    }
};

#endif