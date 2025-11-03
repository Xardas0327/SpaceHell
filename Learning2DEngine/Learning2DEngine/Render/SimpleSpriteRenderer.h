#pragma once

#include <vector>
#include <map>

#include <glad/glad.h>

#include "../System/Singleton.h"
#include "BaseRenderer.h"
#include "RenderData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class SimpleSpriteRenderer : public BaseRenderer, public System::Singleton<SimpleSpriteRenderer>
		{
			friend class Singleton<SimpleSpriteRenderer>;
		private:
			GLuint vboDynamicUV;

			//int is the layer
			std::map<RendererMode, std::map<int, std::vector<RenderData*>>> spriteRenderData;

			SimpleSpriteRenderer();

			void InitShader() override;
			void InitVao() override;
			void DestroyObject() override;
		public:
			//int is the layer
			void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) override;
			void Draw(RendererMode rendererMode, int layer) override;
		};
	}
}