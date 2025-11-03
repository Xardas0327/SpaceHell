#pragma once

#include "BaseLayerItemData.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class LayerItemData : public BaseLayerItemData
		{
		public:
			int gid;
			int row;
			int column;

			LayerItemData(const std::vector<TiledMapTileset>& tilesets, 
				int gid, int row, int column, int layerId = 0,
				bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f),
				glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
				: BaseLayerItemData(tilesets, layerId, visible, opacity, offset, tintColor),
				gid(gid), row(row), column(column)
			{
			}
		};
	}
}