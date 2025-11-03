#pragma once

#include <glad/glad.h>
#include <map>
#include <vector>
#include <tuple>

#include "../System/Singleton.h"
#include "../Render/BaseMultiRenderer.h"
#include "../Render/MultiSpriteDynamicData.h"

#include "ParticleRenderData.h"


namespace Learning2DEngine
{
	namespace ParticleSimulator
	{
		class ParticleRenderer : public Render::BaseMultiRenderer<Render::MultiSpriteDynamicData>, public System::Singleton<ParticleRenderer>
		{
			friend class Singleton<ParticleRenderer>;
		private:
			//The first int is the layer
			//The vector contains the tuples, which renderable at the same time.  
			//The tuple contains a map, whose keys are the texture ids and values the vector of SpriteRenderData*,
			//a bool which means, that it uses blend or not, the blend function factor and the max active particle count.
			std::map<Render::RendererMode, 
				std::map<int, std::vector<std::tuple<std::map<GLuint, std::vector<ParticleRenderData*>>, bool, Render::BlendFuncFactor, size_t>>>
			> particleRenderData;

			ParticleRenderer();

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