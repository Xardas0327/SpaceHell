#pragma once

#include "BaseLayerItemData.h"
#include "ObjectItem.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class ObjectLayerItemData : public BaseLayerItemData
		{
		public:
			const ObjectItem& objectItem;

			ObjectLayerItemData(const std::vector<TiledMapTileset>& tilesets,
				const ObjectItem& objectItem, int layerId = 0,
				bool visible = true, float opacity = 1.0f, glm::vec2 offset = glm::vec2(0.0f),
				glm::vec4 tintColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
				: BaseLayerItemData(tilesets, layerId, visible, opacity, offset, tintColor),
				objectItem(objectItem)
			{
			}
		};
	}
}