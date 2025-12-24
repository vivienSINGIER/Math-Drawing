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

struct Lagrange : public MathFunction
{
    std::vector<float> xs;
    std::vector<float> ys;
    
    float operator()(float x) const override
    {
        float result = 0.0f;

        for (int i = 0; i < xs.size(); i++)
        {
            float numerator = 1.0f;
            float denominator = 1.0f;
            for (int j = 0; j < ys.size(); j++)
            {
                if ( j == i ) continue;
                
                numerator *= x - xs[j];
                denominator *= xs[i] - xs[j];
            }
            result += ys[i] * numerator / denominator;
        }

        return result;
    }
};

struct Hermite : public MathFunction
{
    sf::Vector2f v1;
    float fp1;
    
    sf::Vector2f v2;
    float fp2;
    
    float operator()(float x) const override
    {
        float h   = v2.x - v1.x;
        float f1  = v1.y;
        float f2  = v2.y;
        float x1  = v1.x;

        float a = 2 * (f1 - f2) / Utils::Pow(h, 3) + (fp1 + fp2) / Utils::Pow(h, 2);
        a *= Utils::Pow(x, 3);

        float b = -6 * x1 * (f1 - f2) / Utils::Pow(h, 3);
        b += (-3 * (f1 - f2) - 3 * x1 * (fp1 + fp2)) / Utils::Pow(h, 2);
        b += (-2 * fp1 - fp2) / h;
        b *= Utils::Pow(x, 2);

        float c = 6 * Utils::Pow(x1, 2) * (f1 - f2) / Utils::Pow(h, 3);
        c += (x1 * (6 * (f1 - f2) + 3 * x1 * (fp1 + fp2))) / Utils::Pow(h, 2);
        c += x1 * (4 * fp1 + 2 * fp2) / h;
        c += fp1;
        c *= x;

        float d = -2 * Utils::Pow(x1, 3) * (f1 - f2) / Utils::Pow(h, 3);
        d += Utils::Pow(x1, 2) * (-3 * (f1 - f2) - x1 * (fp1 + fp2)) / Utils::Pow(h, 2);
        d += Utils::Pow(x1, 2) * (-2 * fp1 - fp2) / h;
        d += f1;
        d -= x1 * fp1;

        float result = a + b + c + d;
        return result;
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

struct ArcOfCircle : public MathShape
{
    int n = 50;
    
    float a = 1.0f;
    float b = 1.0f;

    float minAngle = 0.0f;
    float maxAngle = PI;

    sf::Vector2f origin = sf::Vector2f(0, 0);
    
    std::vector<vertex> operator()() const override
    {
        std::vector<vertex> result;
        
        float tMax = maxAngle;
        float tStep = tMax / n;
        float tTemp = minAngle;

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