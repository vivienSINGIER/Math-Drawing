#pragma once

#include "GameManager.h"
#include "Scene.h"

template<typename T>
void GameManager::LaunchScene()
{
    static_assert(std::is_base_of<Scene, T>::value, "T must be derived from Scene");
    _ASSERT(mpScene == nullptr);

    T* newScene = new T();
    mpScene = newScene;

    mpScene->SetGameManager(this);
    mpScene->OnInitialize();

    Run();
}