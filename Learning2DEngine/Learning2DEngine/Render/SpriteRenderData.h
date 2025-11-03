#pragma once

#include <glm/glm.hpp>

#include "RenderData.h"
#include "Texture2D.h"
#include "UVTexture2DContainer.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SpriteRenderData : public RenderData, public UVTexture2DContainer
		{
		public:
			glm::vec4 color;
			bool isUseCameraView;

			SpriteRenderData(const System::Component* component, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(true),
				UVTexture2DContainer()
			{
			}

			SpriteRenderData(const System::Component* component, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(isUseCameraView),
				UVTexture2DContainer()
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(true),
				UVTexture2DContainer(texture)
			{
			}

			SpriteRenderData(const System::Component* component, const Texture2D& texture, bool isUseCameraView, glm::vec4 color = glm::vec4(1.0f))
				: RenderData(component), color(color), isUseCameraView(isUseCameraView),
				UVTexture2DContainer(texture)
			{
			}
		};
	}
}