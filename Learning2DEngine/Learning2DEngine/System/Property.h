#pragma once

#include <string>
#include <glm/glm.hpp>

#include "../DebugTool/Log.h"
#include "PropertyType.h"

namespace Learning2DEngine
{
	namespace System
	{
		class Property final
		{
		private:
			PropertyType type;

			union {
				bool boolValue;
				glm::vec4 colorValue;
				float floatValue;
				int intValue;
			};

			//for text and file
			std::string textValue;
		public:
			Property()
				: type(PropertyType::NONE), boolValue(false), textValue()
			{
			}
			Property(bool value)
				: type(PropertyType::BOOL), boolValue(value), textValue()
			{
			}

			Property(const glm::vec4& value)
				: type(PropertyType::COLOR), colorValue(value), textValue()
			{
			}

			Property(float value)
				: type(PropertyType::FLOAT), floatValue(value), textValue()
			{
			}

			Property(int value)
				: type(PropertyType::INT), intValue(value), textValue()
			{
			}

			Property(const std::string& value, bool isFilePath = false)
				: type(isFilePath ? PropertyType::FILE : PropertyType::STRING), boolValue(false), textValue(value)
			{
			}

			Property(std::string&& value, bool isFilePath = false)
				: type(isFilePath ? PropertyType::FILE : PropertyType::STRING), boolValue(false), textValue(std::move(value))
			{
			}

			~Property() = default;

			inline PropertyType GetType() const
			{
				return type;
			}

			inline bool GetBool() const
			{
				return boolValue;
			}

			inline glm::vec4 GetColor() const
			{
				return colorValue;
			}

			inline float GetFloat() const
			{
				return floatValue;
			}

			inline int GetInt() const
			{
				return intValue;
			}

			inline const std::string& GetString() const
			{
				return textValue;
			}

			inline const std::string& GetFile() const
			{
				return textValue;
			}
		};
	}
}