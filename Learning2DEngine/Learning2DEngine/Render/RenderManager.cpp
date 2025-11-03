#include "RenderManager.h"

#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    using namespace EventSystem;

    namespace Render
    {
        RenderManager::RenderManager() :
            window(nullptr), resolution(0, 0), clearColor(0.0f, 0.0f, 0.0f, 1.0f),
            keyboardEventHandler(), framebufferSizeEventHandler(),
            mouseButtonEventHandler(), cursorPositonEventHandler(), cursorEnterEventHandler(), scrollEventHandler(),
            blendFuncFactor(GL_ONE, GL_ZERO), isBlendActive(false), maxTextureUnits(1), mutex()
        {

        }

        void RenderManager::Init(int majorVersion, int minorVersion, Resolution resolution, const char* title, WindowType windowType)
        {
            this->resolution = resolution;

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, majorVersion);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minorVersion);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            GLFWmonitor* monitor = nullptr;
            switch (windowType)
            {   
            case WindowType::FIXED_SIZE:
                glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
                break;
            case WindowType::BORDERLESS:
                monitor = glfwGetPrimaryMonitor();
                break;
            case WindowType::RESIZABLE:
                break;
            }

            window = glfwCreateWindow(resolution.GetWidth(), resolution.GetHeight(), title, monitor, nullptr);
            if (window == NULL)
            {
                glfwTerminate();
                L2DE_LOG_ERROR("GLFW: Failed to create GLFW window");
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSetFramebufferSizeCallback(window, RenderManager::CallbackUpdateFramebufferSize);
            glfwSetKeyCallback(window, RenderManager::CallbackUpdateKeyboard);
            glfwSetMouseButtonCallback(window, RenderManager::CallbackUpdateMouseButton);
            glfwSetCursorPosCallback(window, RenderManager::CallbackUpdateCursorPositon);
            glfwSetCursorEnterCallback(window, RenderManager::CallbackUpdateCursorEnter);
            glfwSetScrollCallback(window, RenderManager::CallbackUpdateScroll);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                L2DE_LOG_ERROR("GLAD: Failed to initialize GLAD");
                return;
            }

            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);

            glViewport(0, 0, resolution.GetWidth(), resolution.GetHeight());
        }

        void RenderManager::Terminate()
        {
            glfwTerminate();
        }

        void RenderManager::CloseWindow()
        {
            glfwSetWindowShouldClose(window, true);
        }

        bool RenderManager::IsWindowClosed()
        {
            return glfwWindowShouldClose(window);
        }

        void RenderManager::SetVSync(bool value)
        {
            glfwSwapInterval(value);
        }

        void RenderManager::UpdateWindow()
        {
            glfwSwapBuffers(window);
        }

        void RenderManager::ClearWindow()
        {
            glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void RenderManager::SetClearColor(float r, float g, float b, float a)
        {
            clearColor = glm::vec4(r, g, b, a);
        }

        void RenderManager::EnableBlend()
        {
            if (!isBlendActive)
            {
                isBlendActive = true;
                glEnable(GL_BLEND);
            }
        }

        void RenderManager::DisableBlend()
        {
            if (isBlendActive)
            {
                isBlendActive = false;
                glDisable(GL_BLEND);
            }
        }

        void RenderManager::SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor)
        {
            BlendFuncFactor newBlendFuncFactor(sourceFactor, destinationFactor);
            if (blendFuncFactor != newBlendFuncFactor)
            {
                blendFuncFactor = newBlendFuncFactor;
                glBlendFunc(blendFuncFactor.first, blendFuncFactor.second);
            }
        }

        void RenderManager::SetBlendFunc(BlendFuncFactor blendFuncFactor)
        {
            if (this->blendFuncFactor != blendFuncFactor)
            {
                this->blendFuncFactor = blendFuncFactor;
                glBlendFunc(this->blendFuncFactor.first, this->blendFuncFactor.second);
            }
        }

        void RenderManager::CallbackUpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            RenderManager::GetInstance().UpdateKeyboard(window, key, scancode, action, mode);
        }

        void RenderManager::UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            keyboardEventHandler.Invoke(key, scancode, action, mode);
        }

        void RenderManager::CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height)
        {
            RenderManager::GetInstance().UpdateFramebufferSize(window, width, height);
        }

        void RenderManager::UpdateFramebufferSize(GLFWwindow* window, int width, int height)
        {
            glViewport(0, 0, width, height);
            resolution = Resolution(width, height);
            framebufferSizeEventHandler.Invoke(resolution);
        }

        void RenderManager::CallbackUpdateMouseButton(GLFWwindow* window, int button, int action, int mods)
        {
            RenderManager::GetInstance().UpdateMouseButton(window, button, action, mods);
        }

        void RenderManager::UpdateMouseButton(GLFWwindow* window, int button, int action, int mods)
        {
            mouseButtonEventHandler.Invoke(button, action, mods);
        }

        void RenderManager::CallbackUpdateCursorPositon(GLFWwindow* window, double xpos, double ypos)
        {
            RenderManager::GetInstance().UpdateCursorPositon(window, xpos, ypos);
        }

        void RenderManager::UpdateCursorPositon(GLFWwindow* window, double xpos, double ypos)
        {
            cursorPositonEventHandler.Invoke(xpos, ypos);
        }

        void RenderManager::CallbackUpdateCursorEnter(GLFWwindow* window, int entered)
        {
            RenderManager::GetInstance().UpdateCursorEnter(window, entered);
        }

        void RenderManager::UpdateCursorEnter(GLFWwindow* window, int entered)
        {
            cursorEnterEventHandler.Invoke((bool)entered);
        }

        void RenderManager::CallbackUpdateScroll(GLFWwindow* window, double xoffset, double yoffset)
        {
            RenderManager::GetInstance().UpdateScroll(window, xoffset, yoffset);
        }

        void RenderManager::UpdateScroll(GLFWwindow* window, double xoffset, double yoffset)
        {
            scrollEventHandler.Invoke(xoffset, yoffset);
        }

        void RenderManager::AddKeyboardEvent(EventItem<int, int, int, int>* eventItem)
        {
            keyboardEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveKeyboardEvent(EventItem<int, int, int, int>* eventItem)
        {
            keyboardEventHandler.Remove(eventItem);
        }

        void RenderManager::AddFramebufferSizeEvent(EventItem<Resolution>* eventItem)
        {
            framebufferSizeEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveFramebufferSizeEvent(EventItem<Resolution>* eventItem)
        {
            framebufferSizeEventHandler.Remove(eventItem);
        }

        void RenderManager::AddMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem)
        {
            mouseButtonEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem)
        {
            mouseButtonEventHandler.Remove(eventItem);
        }

        void RenderManager::AddCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem)
        {
            cursorPositonEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem)
        {
            cursorPositonEventHandler.Remove(eventItem);
        }

        void RenderManager::AddCursorEnterEvent(EventSystem::EventItem<bool>* eventItem)
        {
            cursorEnterEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveCursorEnterEvent(EventSystem::EventItem<bool>* eventItem)
        {
            cursorEnterEventHandler.Remove(eventItem);
        }

        void RenderManager::AddScrollEvent(EventSystem::EventItem<double, double>* eventItem)
        {
            scrollEventHandler.Add(eventItem);
        }

        void RenderManager::RemoveScrollEvent(EventSystem::EventItem<double, double>* eventItem)
        {
            scrollEventHandler.Remove(eventItem);
        }
    }
}