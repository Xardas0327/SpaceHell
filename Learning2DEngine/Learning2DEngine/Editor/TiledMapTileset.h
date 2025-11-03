#pragma once

#include <glm/glm.hpp>
#include <string>
#include <map>
#include <vector>


#include "../Render/Texture2D.h"
#include "../System/Property.h"
#include "ObjectItem.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct TiledMapTileset final
		{
			int firstGid;
			int columns;
			int tileCount;
			int spacing;
			int margin;
			glm::vec2 tileSize;
			glm::vec2 imageSize;
			glm::vec2 tileOffset;
			Render::Texture2D* texture;
			std::map<std::string, System::Property> commonProperties;
			std::map<int, std::map<std::string, System::Property>> uniqueProperties;
			std::map<int, std::vector<ObjectItem>> objects;

			TiledMapTileset()
				: firstGid(0), columns(0), tileCount(0), spacing(0), margin(0),
				tileSize(0.0f), imageSize(0.0f), tileOffset(0.0f), texture(nullptr),
				commonProperties(), uniqueProperties(), objects()
			{
			}

			~TiledMapTileset() = default;

			bool HasNumber(int gid) const
			{
				return gid >= firstGid && gid < firstGid + tileCount;
			}

			int GetLocalId(int gid) const
			{
				return gid - firstGid;
			}

			/// <summary>
			/// The texture coordinate order:
			/// Top Left,
			/// Top Right,
			/// Bottom Right,
			/// Bottom Left
			/// </summary>
			glm::mat4x2 GetUV(int gid) const
			{
				int actualId = gid - firstGid;
				float column = static_cast<float>(actualId % columns);
				float row = static_cast<float>(actualId / columns);

				glm::vec2 topLeft(
					static_cast<float>(margin) + column * (tileSize.x + spacing),
					static_cast<float>(margin) + row * (tileSize.y + spacing)
				);

				//if it is not start from 0, add 1 pixel offset to avoid texture bleeding
				float startX = (topLeft.x + static_cast<float>(column != 0.0f)) / imageSize.x;
				float startY = (topLeft.y + static_cast<float>(row != 0.0f)) / imageSize.y;

				float endX = (topLeft.x + tileSize.x) / imageSize.x;
				float endY = (topLeft.y + tileSize.y) / imageSize.y;

				return {
					startX, startY,
					endX, startY,
					endX, endY,
					startX,endY
				};
			}
		};
	}
}