#include "pch.h"
#ifndef MATH_FUNCTIONS_HPP_DEFINED
#define MATH_FUNCTIONS_HPP_DEFINED

#include <vector>

#include "Debug.h"
#include "Utils.h"

enum FunctionType
{
    LINEAR,
    PARABOLA,
    LAGRANGE,
    HERMITE,
    BEZIER,
    ELLIPSE,
    ARC_CIRCLE
};

struct MathFunction
{
    ~MathFunction()
    {
        ClearControlPoints();
    }

    void ClearControlPoints()
    {
        for (vertex* v : controlPoints)
        {
            delete v;
        }
        controlPoints.clear();
    }
    
    std::vector<vertex*> controlPoints;
    int controlPointLimit = -1;
    
    virtual void Update() {}
    virtual void OnDrawPoints() {}
    virtual std::vector<vertex> operator()()
    {
        return std::vector<vertex>();
    }

    vertex* HandleSelection( float x, float y )
    {
        for (vertex* v : controlPoints)
        {
            if (v->Contains(x, y))
                return v;
        }
        return nullptr;
    }
    virtual void SetVertexPosition( vertex* v, float x, float y )
    {
        v->x = x;
        v->y = y;
    }
    virtual vertex* AddControlPoint( float x, float y )
    {
        if (controlPoints.size() >= controlPointLimit) return nullptr;
        
        vertex* v = new vertex;
        v->x = x;
        v->y = y;
        controlPoints.push_back(v);
        return v;
    }
};

struct Linear : public MathFunction
{
    Linear()
    {
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(1.f, 1.f));
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() <= 1) return result;
        
        for (int i = 0; i < controlPoints.size(); i++)
            result.push_back(*controlPoints[i]);

        return result;
    }
};

struct Parabola : public MathFunction
{

    int n = 50;
    
    Parabola()
    {
        controlPointLimit = 2;
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(1.f, 1.f));
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() <= 1) return result;

        float x0 = controlPoints[0]->x;
        float y0 = controlPoints[0]->y;
        float x1 = controlPoints[1]->x;
        float y1 = controlPoints[1]->y;

        float dx = x1 - x0;
        if (dx == 0.f) return result;
        
        float c = (y1 - y0) / (dx * dx);

        float start = -Utils::Abs(dx);
        float step = (Utils::Abs(dx) * 2.f) / n;

        for (int i = 0; i <= n; i++)
        {
            float x = x0 + start;
            float y = y0 + c * start * start;
            result.emplace_back(x, y);
            start += step;
        }

        return result;
    }
};

struct Lagrange : public MathFunction
{
    int n = 50;
    
    Lagrange()
    {
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(0.8f, 0.5f));
        controlPoints.push_back(new vertex(1.f, 1.f));
    }

    void SortControlPoints()
    {
        if (controlPoints.size() <= 1) return;
        
        for (int i = 0; i < controlPoints.size() - 1; i++)
        {
            for (int j = i + 1; j < controlPoints.size(); j++)
            {
                if (controlPoints[i]->x > controlPoints[j]->x)
                {
                    vertex* temp = controlPoints[i];
                    controlPoints[i] = controlPoints[j];
                    controlPoints[j] = temp;
                }
            }
        }
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() <= 1) return result;

        SortControlPoints();
        
        float start = controlPoints[0]->x;
        float end = controlPoints.back()->x;
        float distance = end - start;
        float step = Utils::Abs(distance) / n;
        
        for (int k = 0; k <= n; k++)
        {
            float yResult = 0.0f;

            for (int i = 0; i < controlPoints.size(); i++)
            {
                float numerator = 1.0f;
                float denominator = 1.0f;
                for (int j = 0; j < controlPoints.size(); j++)
                {
                    if ( j == i ) continue;
                
                    numerator *= start - controlPoints[j]->x;
                    denominator *= controlPoints[i]->x - controlPoints[j]->x;
                }
                yResult += controlPoints[i]->y * numerator / denominator;
            }
            result.push_back(vertex(start, yResult));
            start += step;
        }

        return result;
    }
};

struct Hermite : public MathFunction
{
    int n = 50;
    
    Hermite()
    {
        controlPointLimit = 4;
        
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(1.f, 1.f));

        controlPoints.push_back(new vertex(0.5f, 0.f));
        controlPoints.push_back(new vertex(1.0f, 0.5f));
    }

    void OnDrawPoints() override
    {
        if (controlPoints.size() != 4) return;
        
        Debug::DrawLine(controlPoints[0]->x * TILE_SIZE,
            -controlPoints[0]->y * TILE_SIZE,
            controlPoints[2]->x * TILE_SIZE,
            -controlPoints[2]->y * TILE_SIZE, sf::Color::White);

        Debug::DrawLine(controlPoints[1]->x * TILE_SIZE,
            -controlPoints[1]->y * TILE_SIZE,
            controlPoints[3]->x * TILE_SIZE,
            -controlPoints[3]->y * TILE_SIZE, sf::Color::White);
    }

    void SortControlPoints()
    {
        if (controlPoints.size() != 4) return;

        if (controlPoints[0]->x > controlPoints[2]->x)
        {
            vertex* temp = controlPoints[0];
            controlPoints[0] = controlPoints[2];
            controlPoints[2] = temp;

            temp = controlPoints[1];
            controlPoints[1] = controlPoints[3];
            controlPoints[3] = temp;
        }
    }

    void SetVertexPosition(vertex* v, float x, float y) override
    {
        if (controlPoints.size() != 4) return;
        
        if (v == controlPoints[0])
        {
            float xDistance = controlPoints[2]->x - controlPoints[0]->x;
            float yDistance = controlPoints[2]->y - controlPoints[0]->y;

            v->x = x;
            v->y = y;
            controlPoints[2]->x = x + xDistance;
            controlPoints[2]->y = y + yDistance;
        }

        else if (v == controlPoints[1])
        {
            float xDistance = controlPoints[3]->x - controlPoints[1]->x;
            float yDistance = controlPoints[3]->y - controlPoints[1]->y;

            v->x = x;
            v->y = y;
            controlPoints[3]->x = x + xDistance;
            controlPoints[3]->y = y + yDistance;
        }
        else
        {
            v->x = x;
            v->y = y;
        }
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() != 4) return result;

        SortControlPoints();
        
        float start = controlPoints[0]->x;
        float end = controlPoints[1]->x;
        float distance = end - start;
        float step = Utils::Abs(distance) / n;

        vertex v1 = *controlPoints[0];
        vertex v2 = *controlPoints[1];

        sf::Vector2f vfp1 = {controlPoints[2]->x - v1.x, controlPoints[2]->y - v1.y};
        float fp1 = 0.0f;
        if (vfp1.x == 0.0f)
            fp1 = 500.0f;            
        else
            fp1 = vfp1.y / vfp1.x;

        sf::Vector2f vfp2 = {controlPoints[3]->x - v2.x, controlPoints[3]->y - v2.y};
        float fp2 = 0.0f;
        if (vfp2.x == 0.0f)
            fp2 = 500.0f;
        else
            fp2 = vfp2.y / vfp2.x;
        
        for (int k = 0; k <= n; k++)
        {
            float h   = v2.x - v1.x;
            float f1  = v1.y;
            float f2  = v2.y;
            float x1  = v1.x;

            float a = 2 * (f1 - f2) / Utils::Pow(h, 3) + (fp1 + fp2) / Utils::Pow(h, 2);
            a *= Utils::Pow(start, 3);

            float b = -6 * x1 * (f1 - f2) / Utils::Pow(h, 3);
            b += (-3 * (f1 - f2) - 3 * x1 * (fp1 + fp2)) / Utils::Pow(h, 2);
            b += (-2 * fp1 - fp2) / h;
            b *= Utils::Pow(start, 2);

            float c = 6 * Utils::Pow(x1, 2) * (f1 - f2) / Utils::Pow(h, 3);
            c += (x1 * (6 * (f1 - f2) + 3 * x1 * (fp1 + fp2))) / Utils::Pow(h, 2);
            c += x1 * (4 * fp1 + 2 * fp2) / h;
            c += fp1;
            c *= start;

            float d = -2 * Utils::Pow(x1, 3) * (f1 - f2) / Utils::Pow(h, 3);
            d += Utils::Pow(x1, 2) * (-3 * (f1 - f2) - x1 * (fp1 + fp2)) / Utils::Pow(h, 2);
            d += Utils::Pow(x1, 2) * (-2 * fp1 - fp2) / h;
            d += f1;
            d -= x1 * fp1;

            float yResult = a + b + c + d;
            result.push_back(vertex(start, yResult));
            start += step;
        }

        return result;
    }
};

struct BezierCurve : public MathFunction
{
    int steps = 50;

    BezierCurve()
    {
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(1.5f, 3.5f));
        controlPoints.push_back(new vertex(5.f, 5.f));
    }

    void OnDrawPoints() override
    {
        for (int i = 0; i < controlPoints.size() - 1; i++)
        {
            Debug::DrawLine(controlPoints[i]->x * TILE_SIZE,
            -controlPoints[i]->y * TILE_SIZE,
            controlPoints[i + 1]->x * TILE_SIZE,
            -controlPoints[i + 1]->y * TILE_SIZE, sf::Color::White);   
        }
    }
    
    vertex* AddControlPoint(float x, float y) override
    {
        vertex* v = new vertex;
        v->x = x;
        v->y = y;
        controlPoints.insert(controlPoints.end() - 1, v);
        return v;
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        
        float t = 0.0f;
        float tStep = 1.0f / steps;

        while (t <= 1.0f)
        {
            vertex v = {0.0f, 0.0f};

            std::vector<vertex> temp;

            for (int i = 0; i < controlPoints.size(); i++)
            {
                temp.push_back(*controlPoints[i]);
            }
            
            for (int k = 1; k < controlPoints.size(); k++)
            {
                for (int i = 0; i < temp.size() - k; i++)
                {
                    temp[i].x = (1.0f - t) * temp[i].x + t * temp[i + 1].x;
                    temp[i].y = (1.0f - t) * temp[i].y + t * temp[i + 1].y;
                }
            }
            result.push_back(temp[0]);
            t += tStep;
        }
        return result;
    }
};

struct Ellipse : public MathFunction
{
    int n = 50;

    Ellipse()
    {
        controlPointLimit = 3;
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(5.0f, 0.0f));
        controlPoints.push_back(new vertex(0.0f, 5.0f));
    }

    void SetVertexPosition(vertex* v, float x, float y) override
    {
        if (controlPoints.size() != 3) return;
        
        if (v == controlPoints[0])
        {
            float xDist = controlPoints[1]->x - controlPoints[0]->x;
            float yDist = controlPoints[2]->y - controlPoints[0]->y;

            v->x = x;
            v->y = y;

            controlPoints[1]->x = x + xDist;
            controlPoints[1]->y = y;
            controlPoints[2]->x = x;
            controlPoints[2]->y = y + yDist;
        }

        if (v == controlPoints[1])
            v->x = x;
        if (v == controlPoints[2])
            v->y = y;
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() != 3) return result;
        
        vertex origin = *controlPoints[0];
        float a = controlPoints[1]->x - origin.x;
        float b = controlPoints[2]->y - origin.y;
        
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

struct ArcOfCircle : public MathFunction
{
    int n = 50;

    float radius = 5.0f;

    ArcOfCircle()
    {
        controlPointLimit = 3;
        controlPoints.push_back(new vertex(0.f, 0.f));
        controlPoints.push_back(new vertex(5.0f, 0.0f));
        controlPoints.push_back(new vertex(0.0f, 5.0f));
        radius = 5.0f;
    }

    void SetVertexPosition(vertex* v, float x, float y) override
    {
        if (controlPoints.size() != 3) return;

        if (v == controlPoints[0])
        {
            float xDist = controlPoints[1]->x - controlPoints[0]->x;
            float yDist = controlPoints[1]->y - controlPoints[0]->y;
            float xDist2 = controlPoints[2]->x - controlPoints[0]->x;
            float yDist2 = controlPoints[2]->y - controlPoints[0]->y;

            v->x = x;
            v->y = y;
            controlPoints[1]->x = x + xDist;
            controlPoints[1]->y = y + yDist;
            controlPoints[2]->x = x + xDist2;
            controlPoints[2]->y = y + yDist2;
        }

        else if (v == controlPoints[1])
        {
            sf::Vector2f vect = {v->x - controlPoints[0]->x, v->y - controlPoints[0]->y};
            float newRadius = Utils::Norm(vect);

            sf::Vector2f vect2 = {controlPoints[2]->x - controlPoints[0]->x, controlPoints[2]->y - controlPoints[0]->y};
            Utils::Normalize(vect2);
            vect2 *= newRadius;

            v->x = x;
            v->y = y;
            controlPoints[2]->x = controlPoints[0]->x + vect2.x;
            controlPoints[2]->y = controlPoints[0]->y + vect2.y;

            radius = newRadius;
        }

        else if (v == controlPoints[2])
        {
            sf::Vector2f vect2 = {controlPoints[1]->x - controlPoints[0]->x, controlPoints[1]->y - controlPoints[0]->y};
            Utils::Normalize(vect2);
            vect2 *= radius;

            v->x = x;
            v->y = y;
            controlPoints[1]->x = controlPoints[0]->x + vect2.x;
            controlPoints[1]->y = controlPoints[0]->y + vect2.y;
        }
    }
    
    std::vector<vertex> operator()() override
    {
        std::vector<vertex> result;
        if (controlPoints.size() != 3) return result;
        
        vertex origin = *controlPoints[0];
        vertex a = *controlPoints[1];
        vertex b = *controlPoints[2];

        sf::Vector2f v1 = {a.x - origin.x, -(a.y - origin.y)};
        sf::Vector2f v2 = {b.x - origin.x, -(b.y - origin.y)};
        float minAngle = Utils::GetAngle(v1);
        float maxAngle = Utils::GetAngle(v2);

        radius = Utils::Norm(v1);
        
        if (maxAngle < minAngle)
            maxAngle += 2 * PI;
        
        float angleRange = maxAngle - minAngle;
        float tStep = angleRange / n;
        float tTemp = minAngle;

        for (int i = 0; i <= n; i++)
        {
            vertex v;
            v.x = origin.x + radius * cos(tTemp);
            v.y = origin.y + radius * sin(tTemp);
            result.push_back(v);
            tTemp += tStep;
        }
        
        return result;
    }
};

#endif