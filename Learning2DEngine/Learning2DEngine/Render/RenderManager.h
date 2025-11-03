#pragma once

#include <mutex>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Resolution.h"
#include "Blend.h"
#include "WindowType.h"
#include "../System/Singleton.h"
#include "../EventSystem/EventHandler.h"
#include "../EventSystem/EventItem.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class RenderManager : public System::Singleton<RenderManager>
		{
			friend class System::Singleton<RenderManager>;
		private:
			GLFWwindow* window;
			Resolution resolution;
			glm::vec4 clearColor;
			EventSystem::EventHandler<int, int, int, int> keyboardEventHandler;
			EventSystem::EventHandler<Resolution> framebufferSizeEventHandler;
			EventSystem::EventHandler<int, int, int> mouseButtonEventHandler;
			EventSystem::EventHandler<double, double> cursorPositonEventHandler;
			EventSystem::EventHandler<bool> cursorEnterEventHandler;
			EventSystem::EventHandler<double, double> scrollEventHandler;
			BlendFuncFactor blendFuncFactor;
			bool isBlendActive;
			GLint maxTextureUnits;


			RenderManager();

			static void CallbackUpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
			void UpdateKeyboard(GLFWwindow* window, int key, int scancode, int action, int mode);
			static void CallbackUpdateFramebufferSize(GLFWwindow* window, int width, int height);
			void UpdateFramebufferSize(GLFWwindow* window, int width, int height);
			static void CallbackUpdateMouseButton(GLFWwindow* window, int button, int action, int mods);
			void UpdateMouseButton(GLFWwindow* window, int button, int action, int mods);
			static void CallbackUpdateCursorPositon(GLFWwindow* window, double xpos, double ypos);
			void UpdateCursorPositon(GLFWwindow* window, double xpos, double ypos);
			static void CallbackUpdateCursorEnter(GLFWwindow* window, int entered);
			void UpdateCursorEnter(GLFWwindow* window, int entered);
			static void CallbackUpdateScroll(GLFWwindow* window, double xoffset, double yoffset);
			void UpdateScroll(GLFWwindow* window, double xoffset, double yoffset);
		public:
			//It is a "global" mutex, that the OpenGL can be thread-safe
			std::mutex mutex;

			void Init(int majorVersion, int minorVersion, Resolution resolution, const char* title, WindowType windowType);
			void Terminate();
			void CloseWindow();
			bool IsWindowClosed();
			void SetVSync(bool value);
			void UpdateWindow();
			void ClearWindow();
			inline Resolution GetResolution()  const
			{
				return resolution;
			}

			void SetClearColor(float r, float g, float b, float a);
			inline glm::vec4 GetClearColor() const
			{
				return clearColor;
			}

			void EnableBlend();
			void DisableBlend();
			inline bool IsBlendActive() const
			{
				return isBlendActive;
			}

			void SetBlendFunc(unsigned int sourceFactor, unsigned int destinationFactor);
			void SetBlendFunc(BlendFuncFactor blendFuncFactor);
			inline BlendFuncFactor GetBlendFunc() const
			{
				return blendFuncFactor;
			}

			inline GLint GetMaxTextureUnits() const
			{
				return maxTextureUnits;
			}

			void AddKeyboardEvent(EventSystem::EventItem<int, int, int, int>* eventItem);
			void RemoveKeyboardEvent(EventSystem::EventItem<int, int, int, int>* eventItem);

			void AddFramebufferSizeEvent(EventSystem::EventItem<Resolution>* eventItem);
			void RemoveFramebufferSizeEvent(EventSystem::EventItem<Resolution>* eventItem);

			void AddMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem);
			void RemoveMouseButtonEvent(EventSystem::EventItem<int, int, int>* eventItem);

			void AddCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem);
			void RemoveCursorPositonEvent(EventSystem::EventItem<double, double>* eventItem);

			void AddCursorEnterEvent(EventSystem::EventItem<bool>* eventItem);
			void RemoveCursorEnterEvent(EventSystem::EventItem<bool>* eventItem);

			void AddScrollEvent(EventSystem::EventItem<double, double>* eventItem);
			void RemoveScrollEvent(EventSystem::EventItem<double, double>* eventItem);
		};
	}
}