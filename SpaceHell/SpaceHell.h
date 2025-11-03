#pragma once

#include <glm/glm.hpp>

#include <Learning2DEngine/System/Game.h>

class SpaceHell : public Learning2DEngine::System::Game
{
public:
    SpaceHell() = default;
    ~SpaceHell() = default;

    void Init() override;
};