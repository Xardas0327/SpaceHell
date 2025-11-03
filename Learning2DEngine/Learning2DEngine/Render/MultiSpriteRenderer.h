#pragma once

#include <glad/glad.h>

#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "BaseMultiRenderer.h"
#include "SpriteRenderData.h"
#include "MultiSpriteDynamicData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class MultiSpriteRenderer : public BaseMultiRenderer<MultiSpriteDynamicData>, public System::Singleton<MultiSpriteRenderer>
		{
			friend class Singleton<MultiSpriteRenderer>;
		private:
			//The int is the layer
			//The third map's key is the texture id and the value is the SpriteRenderData vector, which use that texture.
			//If the sprite doesn't use texture, the key is 0.
			std::map <RendererMode, std::map<int, std::map<GLuint, std::vector<SpriteRenderData*>>>> spriteRenderData;

			MultiSpriteRenderer();

			void InitShader() override;
			void InitVao() override;
			void DestroyObject() override;
		public:
			//int is the layer
			void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) override;
			void Draw(Render::RendererMode rendererMode, int layer) override;
		};
	}
}