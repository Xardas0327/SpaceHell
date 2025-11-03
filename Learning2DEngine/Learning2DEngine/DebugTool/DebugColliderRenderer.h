#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../Render/ShaderConstant.h"
#include "../Render/IRenderer.h"
#include "../Render/BaseMultiRenderer.h"
#include "../Render/BaseColorDynamicData.h"

#include "DebugRenderData.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		template<class TColliderComponent>
		class DebugColliderRenderer : public Render::BaseMultiRenderer<Render::BaseColorDynamicData>
		{
		protected:
			//The int is the layer
			std::map<Render::RendererMode, std::map<int, std::vector<DebugRenderData<TColliderComponent>*>>> debugRenderData;

			DebugColliderRenderer()
				: BaseMultiRenderer(), debugRenderData()
			{

			}

			void InitShader() override
			{
				auto& resourceManager = System::ResourceManager::GetInstance();

				shader = resourceManager.IsShaderExist(Render::ShaderConstant::BASE_COLOR_NAME)
					? &resourceManager.GetShader(Render::ShaderConstant::BASE_COLOR_NAME)
					: &resourceManager.LoadShader(
						Render::ShaderConstant::BASE_COLOR_NAME,
						Render::ShaderConstant::GetBaseColorVertexShader(),
						Render::ShaderConstant::GetBaseColorFragmentShader());
			}

			void DestroyObject() override
			{
				Render::BaseMultiRenderer<Render::BaseColorDynamicData>::DestroyObject();

				debugRenderData.clear();
			}
		public:
			//int is the layer
			void SetData(const std::map<Render::RendererMode, std::map<int, std::vector<Render::RenderData*>>>& renderData) override
			{
				debugRenderData.clear();
				size_t maxDynamicSize = 0;
				for (auto& modeData : renderData)
				{
					size_t actualDynamicSize = 0;
					for (auto& layerData : modeData.second)
					{
						auto& actualLayerData = debugRenderData[modeData.first][layerData.first];

						for (auto& data : layerData.second)
						{
							auto colliderData = static_cast<DebugRenderData<TColliderComponent>*>(data);
							if (!colliderData->objectComponent->isActive)
								continue;

							actualLayerData.push_back(colliderData);
						}

						if (actualDynamicSize < actualLayerData.size())
							actualDynamicSize = actualLayerData.size();
					}

					if(maxDynamicSize < actualDynamicSize)
						maxDynamicSize = actualDynamicSize;
				}

				CalcDynamicDataSize(maxDynamicSize);
			}
		};
	}
}