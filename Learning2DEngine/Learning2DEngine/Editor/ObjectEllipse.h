#pragma once

#include "ObjectPoint.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		//The size.x will be used only, because of CircleColliderComponent
		struct ObjectEllipse : public ObjectPoint
		{
			glm::vec2 size;

			ObjectEllipse(const glm::vec2& position, const glm::vec2& size, std::map<std::string, System::Property>&& properties, bool visible = true)
				: ObjectPoint(position, std::move(properties), visible), size(size)
			{

			}
		};
	}
}