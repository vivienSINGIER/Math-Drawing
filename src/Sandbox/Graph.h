#ifndef GRAPH_H_DEFINED
#define GRAPH_H_DEFINED

#include "Curve.h"
#include "Scene.h"

class Graph : public Scene
{
public:
    Graph() = default;
    ~Graph() = default;

    std::vector<Curve> vCurves;
    
protected:
    void OnInitialize();
    void OnEvent(const sf::Event& event);
    void OnUpdate();

private:
    float m_maxX = 100.0f;
    float m_maxY = 100.0f;
    float m_minX = -100.0f;
    float m_minY = -100.0f;

    float m_interval_X = 1.0f;
    float m_interval_Y = 1.0f;

    // MouseMovement Handles
    sf::Vector2i m_mousePos;
    sf::Vector2i m_lastMousePos;
    bool m_isMousePressed = false;
    float m_movementSpeed = 70.0f;

    //Zoom
    sf::Vector2f m_windowSize;
    float m_currentZoom = 1.f;

    sf::View* m_pView = nullptr;
    
    void DrawGraph();
    void HandleMouseMovement();

    void InitDiamond();
    void InitHeart();
    void InitEllipse();
    void InitSpade();
};

#endif
