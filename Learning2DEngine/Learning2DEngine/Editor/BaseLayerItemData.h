#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "TiledMapTileset.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class BaseLayerItemData
		{
		protected:
			BaseLayerItemData(const std::vector<TiledMapTileset>& tilesets, int layerId = 0,
				bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f),
				glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
				: layerId(layerId), visible(visible), opacity(opacity), offset(offset), tintColor(tintColor),
				tilesets(tilesets)
			{
			}

		public:
			int layerId;
			bool visible;
			float opacity;
			glm::vec2 offset;
			glm::vec4 tintColor;

			const std::vector<TiledMapTileset>& tilesets;

			virtual ~BaseLayerItemData() = default;
		};
	}
}