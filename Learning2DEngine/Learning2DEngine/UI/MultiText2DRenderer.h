#pragma once

#include <map>
#include <vector>
#include <tuple>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/BaseMultiRenderer.h"

#include "Text2DDynamicData.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class MultiText2DRenderer : public Render::BaseMultiRenderer<Text2DDynamicData>, public System::Singleton<MultiText2DRenderer>
		{
			friend class Singleton<MultiText2DRenderer>;
		private:
			//The int is the layer  
			//The map key is a GLuint, which a character texture id, and the value is a vector,
			//which contains the position, the color and isUseCameraView.
			std::map<Render::RendererMode, std::map<int, std::map<GLuint, std::vector<std::tuple<glm::mat4x2, glm::vec4, bool>>>>> textRenderData;

			MultiText2DRenderer();

			void InitShader() override;
			void InitVao() override;
			void DestroyObject() override;
		public:
			//int is the layer
			void SetData(const std::map<Render::RendererMode, std::map<int, std::vector<Render::RenderData*>>>& renderData) override;
			void Draw(Render::RendererMode rendererMode, int layer) override;
		};
	}
}
