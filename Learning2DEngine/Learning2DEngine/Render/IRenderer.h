#pragma once

#include "map"
#include "vector"

#include "RenderData.h"
#include "RendererMode.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct IRenderer
		{
			virtual void Init() = 0;
			virtual void Destroy() = 0;
			//int is the layer
			virtual void SetData(const std::map<RendererMode, std::map<int, std::vector<RenderData*>>>& renderData) = 0;
			virtual void Draw(RendererMode rendererMode, int layer) = 0;
			virtual ~IRenderer() = default;
		};
	}
}
