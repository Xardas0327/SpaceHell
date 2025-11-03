#pragma once

#include <vector>
#include <map>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "../Render/BaseRenderer.h"

namespace Learning2DEngine
{
	namespace UI
	{
		class SimpleText2DRenderer : public Render::BaseRenderer, public System::Singleton<SimpleText2DRenderer>
		{
			friend class Singleton<SimpleText2DRenderer>;
		private:
			GLuint vboDynamicPosition;

			//int is the layer
			std::map<Render::RendererMode, std::map<int, std::vector<Render::RenderData*>>> textRenderData;

			SimpleText2DRenderer();

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
