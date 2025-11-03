#include "MultiText2DRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		MultiText2DRenderer::MultiText2DRenderer()
			: BaseMultiRenderer(), textRenderData()
		{

		}

		void MultiText2DRenderer::InitShader()
		{
			auto& resourceManager = ResourceManager::GetInstance();

			shader = resourceManager.IsShaderExist(ShaderConstant::TEXT2D_SHADER_NAME)
				? &resourceManager.GetShader(ShaderConstant::TEXT2D_SHADER_NAME)
				: &resourceManager.LoadShader(
					ShaderConstant::TEXT2D_SHADER_NAME,
					ShaderConstant::GetText2DVertexShader(),
					ShaderConstant::GetText2DFragmentShader());
		}

		void MultiText2DRenderer::InitVao()
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

			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);


			glGenBuffers(1, &vboDynamicObject);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Text2DDynamicData), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
				sizeof(Text2DDynamicData),
				(void*)offsetof(Text2DDynamicData, position));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(Text2DDynamicData),
				(void*)(offsetof(Text2DDynamicData, position) + sizeof(float) * 2));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE,
				sizeof(Text2DDynamicData),
				(void*)(offsetof(Text2DDynamicData, position) + sizeof(float) * 4));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE,
				sizeof(Text2DDynamicData),
				(void*)(offsetof(Text2DDynamicData, position) + sizeof(float) * 6));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, color));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, textureId));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(Text2DDynamicData), (void*)offsetof(Text2DDynamicData, isUseCameraView));

			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

			maxObjectSize = 1;
			dynamicData = new Text2DDynamicData[maxObjectSize];
		}

		void MultiText2DRenderer::DestroyObject()
		{
			BaseMultiRenderer::DestroyObject();

			textRenderData.clear();
		}

		void MultiText2DRenderer::SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData)
		{
			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			textRenderData.clear();
			size_t maxDynamicSize = 0;
			for (auto& modeData : renderData)
			{
				size_t actualDynamicSize = 0;
				for (auto& layerData : modeData.second)
				{
					auto& actualLayerData = textRenderData[modeData.first][layerData.first];

					for (auto& data : layerData.second)
					{
						auto textData = static_cast<Text2DRenderData*>(data);
						if (textData->GetText().size() > 0)
						{
							textCharacterSet.Load(textData->GetFontSizePair());
							CharacterMap& characterMap = textCharacterSet[textData->GetFontSizePair()];

							auto& textMap = textData->GetCharacterVertices();
							for (auto& chPair : textMap)
							{
								const auto& ch = characterMap[chPair.first];
								for (auto& vertices : chPair.second)
								{
									actualLayerData[ch.textureId].emplace_back(
										vertices,
										textData->color,
										textData->isUseCameraView
									);
								}
							}
						}
					}

					//Calculate the maximum size of the dynamic data
					int textureUnitNumber = 0;
					size_t actualSize = 0;
					int elementCount = 0;
					for (auto& data : actualLayerData)
					{
						actualSize += data.second.size();
						++textureUnitNumber;
						++elementCount;

						//Check if the texture unit number is arrived to max or this is the last data
						if (textureUnitNumber >= maxTextureUnit || elementCount == actualLayerData.size())
						{
							textureUnitNumber = 0;
							if (actualSize > actualDynamicSize)
								actualDynamicSize = actualSize;
						}
					}
				}

				if (maxDynamicSize < actualDynamicSize)
					maxDynamicSize = actualDynamicSize;
			}

			CalcDynamicDataSize(maxDynamicSize);
		}

		void MultiText2DRenderer::Draw(RendererMode rendererMode, int layer)
		{
			if (textRenderData.find(rendererMode) == textRenderData.end())
				return;
			if (textRenderData[rendererMode].find(layer) == textRenderData[rendererMode].end())
				return;

			GLint maxTextureUnit = RenderManager::GetInstance().GetMaxTextureUnits();
			TextCharacterSet& textCharacterSet = TextCharacterSet::GetInstance();

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());

			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);

			int dynamicDataCount = 0;
			GLint textureUnitId = 0;
			size_t elementCount = 0;
			for (auto& characterData : textRenderData[rendererMode][layer])
			{
				shader->SetInteger(("characterTextures[" + std::to_string(textureUnitId) + "]").c_str(), textureUnitId);
				glActiveTexture(GL_TEXTURE0 + textureUnitId);
				glBindTexture(GL_TEXTURE_2D, characterData.first);

				float tUnitId = static_cast<float>(textureUnitId);
				for (auto& character : characterData.second)
				{
					std::memcpy(dynamicData[dynamicDataCount].position,
						glm::value_ptr(std::get<0>(character)),
						sizeof(dynamicData[dynamicDataCount].position));

					std::memcpy(dynamicData[dynamicDataCount].color,
						glm::value_ptr(std::get<1>(character)),
						sizeof(dynamicData[dynamicDataCount].color));

					dynamicData[dynamicDataCount].textureId = tUnitId;
					dynamicData[dynamicDataCount].isUseCameraView = static_cast<float>(std::get<2>(character));
					++dynamicDataCount;
				}
				++textureUnitId;
				++elementCount;

				//Check if the texture unit number is arrived to max or this is the last data
				if (textureUnitId >= maxTextureUnit || elementCount == textRenderData[rendererMode][layer].size())
				{
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Text2DDynamicData) * dynamicDataCount, dynamicData);
					glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, dynamicDataCount);

					dynamicDataCount = 0;
					textureUnitId = 0;
				}
			}

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}