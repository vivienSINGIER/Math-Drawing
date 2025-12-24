#ifndef BEZIERPLAYGROUND_H_DEFINED
#define BEZIERPLAYGROUND_H_DEFINED

#include "Curve.h"
#include "Scene.h";

class BezierPlayGround : public Scene
{
public:
    BezierPlayGround()  = default;
    ~BezierPlayGround() = default;

    
    
    Curve curve;
};

#endif
