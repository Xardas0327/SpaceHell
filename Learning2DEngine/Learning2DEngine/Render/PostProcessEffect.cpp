#include "PostProcessEffect.h"

#include "../DebugTool/Log.h"
#include "../System/ResourceManager.h"
#include "RenderManager.h"
#include "ShaderConstant.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{
		PostProcessEffect::PostProcessEffect()
			: defaultShader(nullptr), effectShader(nullptr), texture(),
			vao(0), vbo(0), ebo(0), fbo(0)
		{

		}

		PostProcessEffect::~PostProcessEffect()
		{

		}

		void PostProcessEffect::Init(Resolution resolution)
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			texture.Create(resolution.GetWidth(), resolution.GetHeight(), NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.GetId(), 0);
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				L2DE_LOG_ERROR("POSTPROCESSEFFECT: Failed to initialize framebuffer.");
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			InitVao();
			InitDefaultShader();
		}

		void PostProcessEffect::InitVao()
		{
			float vertices[] = {
				// pos      // tex
				 1.0f,  1.0f, 1.0f, 1.0f,
				 1.0f, -1.0f, 1.0f, 0.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				-1.0f,  1.0f, 0.0f, 1.0f,
			};

			unsigned int indices[] = {
			   0, 1, 3,
			   1, 2, 3
			};

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);
			glGenBuffers(1, &ebo);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void PostProcessEffect::InitDefaultShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();

			defaultShader = resourceManager.IsShaderExist(ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME)
				? &resourceManager.GetShader(ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME,
					ShaderConstant::GetDefaultPostprocessVertexShader(),
					ShaderConstant::GetDefaultPostprocessFragmentShader());

			defaultShader->Use();
			defaultShader->SetInteger("scene", 0);
		}

		void PostProcessEffect::Destroy()
		{
			texture.Destroy();
			glDeleteFramebuffers(1, &fbo);
			glDeleteVertexArrays(1, &vao);
			glDeleteBuffers(1, &vbo);
			glDeleteBuffers(1, &ebo);
		}

		void PostProcessEffect::StartRender(bool useClear)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			if (useClear)
				RenderManager::GetInstance().ClearWindow();
		}

		void PostProcessEffect::EndRender()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void PostProcessEffect::Render()
		{
			if (effectShader != nullptr)
				effectShader->Use();
			else
				defaultShader->Use();

			glActiveTexture(GL_TEXTURE0);
			texture.Bind();
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		void PostProcessEffect::SetShader(const Shader& shader)
		{
			effectShader = &shader;
		}

		void PostProcessEffect::ClearShader()
		{
			effectShader = nullptr;
		}
	}
}