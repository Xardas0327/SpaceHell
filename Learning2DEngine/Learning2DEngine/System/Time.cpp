
#include "Time.h"

#include <GLFW/glfw3.h>

#include "../DebugTool/DebugMacro.h"

namespace Learning2DEngine
{
    namespace System
    {
        float Time::deltaTime = 0.0f;
        float Time::timeScale = Time::DefaultTimeScale;
        float Time::lastTime = 0.0f;
        bool Time::isInited = false;

        void Time::CalcDeltaTime()
        {
            if (!Time::isInited)
            {
                Time::isInited = true;
                lastTime = static_cast<float>(glfwGetTime());
            }

            // Calc deltaTime
            float currentTime = static_cast<float>(glfwGetTime());
            Time::deltaTime = (currentTime - lastTime);
            lastTime = currentTime;

#if L2DE_DEBUG
            // It is useful if the game stop by a breakpoint
            if (Time::deltaTime > L2DE_DEBUG_MAX_BASE_DELTATIME)
                Time::deltaTime = L2DE_DEBUG_MAX_BASE_DELTATIME;
#endif
            Time::deltaTime *= timeScale;
        }
    }
}