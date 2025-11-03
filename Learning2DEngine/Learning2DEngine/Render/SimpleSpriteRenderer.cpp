#include "SimpleSpriteRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "ShaderConstant.h"
#include "SpriteRenderData.h"
#include "RenderManager.h"

namespace Learning2DEngine
{
	using namespace System;

	namespace Render
	{

		SimpleSpriteRenderer::SimpleSpriteRenderer()
			: BaseRenderer(), spriteRenderData(), vboDynamicUV(0)
		{

		}

		void SimpleSpriteRenderer::InitShader()
		{
			auto& resourceManager = System::ResourceManager::GetInstance();
			shader = resourceManager.IsShaderExist(ShaderConstant::SIMPLE_SPRITE_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SIMPLE_SPRITE_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SIMPLE_SPRITE_SHADER_NAME,
					ShaderConstant::GetSimpleSpriteVertexShader(),
					ShaderConstant::GetSimpleSpriteFragmentShader());
		}

		void SimpleSpriteRenderer::InitVao()
		{
			float positions[] = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f,
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2, positions, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glGenBuffers(1, &vboDynamicUV);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicUV);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4x2), NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		void SimpleSpriteRenderer::DestroyObject()
		{
			BaseRenderer::DestroyObject();

			glDeleteBuffers(1, &vboDynamicUV);

			spriteRenderData.clear();
		}

		void SimpleSpriteRenderer::SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData)
		{
			spriteRenderData = renderData;
		}

		void SimpleSpriteRenderer::Draw(RendererMode rendererMode, int layer)
		{
			if (spriteRenderData.find(rendererMode) == spriteRenderData.end())
				return;
			if (spriteRenderData[rendererMode].find(layer) == spriteRenderData[rendererMode].end())
				return;

			shader->Use();
			shader->SetInteger("spriteTexture", 0);
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());
			glBindVertexArray(vao);

			for (auto data : spriteRenderData[rendererMode][layer])
			{
				auto spriteData = static_cast<SpriteRenderData*>(data);

				shader->SetMatrix4("model", spriteData->component->gameObject->transform.GetModelMatrix());
				shader->SetInteger("useView", spriteData->isUseCameraView ? GL_TRUE : GL_FALSE);
				shader->SetVector4f("spriteColor", spriteData->color);
				shader->SetInteger("isUseTexture", spriteData->IsUseTexture());

				if (spriteData->IsUseTexture())
				{
					glActiveTexture(GL_TEXTURE0);
					spriteData->GetTexture()->Bind();
				}

				glBindBuffer(GL_ARRAY_BUFFER, vboDynamicUV);
				glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4x2), &spriteData->uvMatrix[0][0]);
				glBindBuffer(GL_ARRAY_BUFFER, 0);

				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			glBindVertexArray(0);
		}
	}
}