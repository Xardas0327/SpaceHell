#include "SimpleText2DRenderer.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"
#include "../Render/RenderManager.h"
#include "Text2DRenderData.h"
#include "TextCharacterSet.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;

	namespace UI
	{
		SimpleText2DRenderer::SimpleText2DRenderer()
			: BaseRenderer(), textRenderData(), vboDynamicPosition(0)
		{

		}

		void SimpleText2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME,
					ShaderConstant::GetSimpleText2DVertexShader(),
					ShaderConstant::GetSimpleText2DFragmentShader());
		}

		void SimpleText2DRenderer::InitVao()
		{
			float textureCoords[] = {
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
			glBindVertexArray(vao);

			glGenBuffers(1, &ebo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

			glGenBuffers(1, &vboDynamicPosition);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicPosition);
			glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4x2), NULL, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}

		void SimpleText2DRenderer::DestroyObject()
		{
			BaseRenderer::DestroyObject();

			textRenderData.clear();
		}

		void SimpleText2DRenderer::SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData)
		{
			textRenderData = renderData;
		}

		void SimpleText2DRenderer::Draw(RendererMode rendererMode, int layer)
		{
			if (textRenderData.find(rendererMode) == textRenderData.end())
				return;
			if (textRenderData[rendererMode].find(layer) == textRenderData[rendererMode].end())
				return;

			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader->Use();
			shader->SetInteger("characterTexture", 0);
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);

			for (auto& data : textRenderData[rendererMode][layer])
			{
				auto textData = static_cast<Text2DRenderData*>(data);

				shader->SetVector4f("characterColor", textData->color);
				shader->SetInteger("useView", textData->isUseCameraView ? GL_TRUE : GL_FALSE);

				textCharacterSet.Load(textData->GetFontSizePair());
				CharacterMap& characterMap = textCharacterSet[textData->GetFontSizePair()];

				auto& textMap = textData->GetCharacterVertices();
				for (auto& chPair : textMap)
				{
					const auto& ch = characterMap[chPair.first];
					for (auto& vertices : chPair.second)
					{
						glBindTexture(GL_TEXTURE_2D, ch.textureId);

						glBindBuffer(GL_ARRAY_BUFFER, vboDynamicPosition);
						glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::mat4x2), &vertices[0][0]);
						glBindBuffer(GL_ARRAY_BUFFER, 0);

						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
					}
				}
			}

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}