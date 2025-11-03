#pragma once

#include "ObjectBox.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		struct ObjectImage : public ObjectBox
		{
			int gid;

			ObjectImage(
				const glm::vec2& position,
				const glm::vec2& size,
				std::map<std::string, System::Property>&& properties,
				int gid,
				bool visible = true
			)
				: ObjectBox(position, size, std::move(properties), visible), gid(gid)
			{

			}
		};
	}
}