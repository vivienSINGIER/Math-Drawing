#pragma once

#include <list>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>

class Scene;
class Debug;

namespace sf 
{
    class RenderWindow;
    class Event;
}

class GameManager
{
    sf::RenderWindow* mpWindow;
    sf::View* mpView;
    sf::View* mpUIView;
    sf::Font mFont;

    Scene* mpScene;

    float mDeltaTime;
    float mTime;

    int mWindowWidth;
    int mWindowHeight;

    sf::Color mClearColor;

private:
    GameManager();

    void Run();
	
    void HandleInput();
    void Update();
    void Draw();

    void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
public:
    ~GameManager();
    static GameManager* Get();

    void CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit = 60, sf::Color clearColor = sf::Color::Black);

    template<typename T>
    void LaunchScene();

    sf::RenderWindow* GetWindow() const { return mpWindow; }
    float GetDeltaTime() const { return mDeltaTime; }
    float GetTime() const { return mTime; }
    Scene* GetScene() const { return mpScene; }
    sf::Font& GetFont() { return mFont; };
    sf::View* GetView() const { return mpView; }
    sf::View* GetUIView() const { return mpUIView; }

    friend Debug;
    friend Scene;
};

#include "GameManager.inl"