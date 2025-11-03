#include "SpaceHell.h"
#include <Learning2DEngine/Render/Resolution.h>
#include <Learning2DEngine/Render/WindowType.h>

using namespace Learning2DEngine::Render;

const Resolution resolution(480, 700);
SpaceHell game;

int main()
{
    game.InitWithRender(3, 3, resolution, "Space Hell", WindowType::FIXED_SIZE);
    game.Run();
    game.Terminate();
    return 0;
}