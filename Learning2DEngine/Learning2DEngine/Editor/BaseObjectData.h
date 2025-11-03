#pragma once

#include <map>
#include <string>

#include "../System/Property.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class BaseObjectData
		{
		protected:
			BaseObjectData(std::map<std::string, System::Property>&& properties, bool visible = true)
				: properties(std::move(properties)), visible(visible)
			{

			}

		public:
			bool visible;
			std::map<std::string, System::Property> properties;

		};
	}
}