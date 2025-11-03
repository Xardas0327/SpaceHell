#pragma once

#include <glad/glad.h>
#include "IRenderer.h"
#include "Shader.h"
#include "RenderManager.h"
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class BaseRenderer : public IRenderer
		{
		protected:
			Shader* shader;
			GLuint vao;
			GLuint ebo;
			GLuint vbo;

			BaseRenderer()
				: shader(nullptr), vao(0), ebo(0), vbo(0)
			{

			}

			virtual void InitShader() = 0;
			virtual void InitVao() = 0;
			virtual void DestroyObject()
			{
				if(vao)
					glDeleteVertexArrays(1, &vao);

				if (vbo)
					glDeleteBuffers(1, &vbo);

				if (ebo)
					glDeleteBuffers(1, &ebo);
			}
		public:
			void Init() override
			{
				if (System::ComponentManager::GetInstance().GetThreadSafe())
				{
					std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
					InitShader();
					InitVao();
				}
				else
				{
					InitShader();
					InitVao();
				}
			}

			void Destroy() override
			{
				if (System::ComponentManager::GetInstance().GetThreadSafe())
				{
					std::lock_guard<std::mutex> lock(RenderManager::GetInstance().mutex);
					DestroyObject();
				}
				else
				{
					DestroyObject();
				}
			}
		};
	}
}