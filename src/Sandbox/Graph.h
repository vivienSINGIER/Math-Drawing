#ifndef GRAPH_H_DEFINED
#define GRAPH_H_DEFINED

#include "Curve.h"
#include "Scene.h"

class Interface;

class Graph : public Scene
{
public:
    Graph() = default;
    ~Graph() = default;

    void TraceCourbe(FunctionType type, std::vector<vertex*> points, bool isMirorO = false, bool isMirorX = false, bool isMirorY = false);
    
protected:
    void OnInitialize();
    void OnEvent(const sf::Event& event);
    void OnUpdate();

private:
    std::vector<Curve> vCurves;

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
    sf::View* m_pUiView = nullptr;
    sf::RenderWindow* m_pWindow = nullptr;

    // ShapeType Selection
    FunctionType m_selectedType = LINEAR;

    // Curve Selection
    int m_selectedIndex = 0;
    Curve* m_selectedCurve = nullptr;
    std::vector<Curve*> m_vCurves;
    vertex* m_selectedVertex = nullptr;

    bool showSymetrie = true;

    bool showAxe = false;

    Interface* m_pInterface = nullptr;
    
    void DrawGraph();
    void HandleMouseMovement();
    void HandleVertexSelection(bool state);

    void InitDiamond();
    void InitHeart();
    void InitSpade();
    void InitClub();
    void InitClover();

    void InitBezier();
};

#endif
