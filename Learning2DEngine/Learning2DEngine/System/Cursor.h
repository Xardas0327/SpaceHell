#pragma once

#include <glm/glm.hpp>
#include "InputStatus.h"

namespace Learning2DEngine
{
	namespace System
	{
		struct Cursor {
			static constexpr const int MouseButtonNumber = 8;

			InputStatus mouseButtons[Cursor::MouseButtonNumber];
			glm::vec2 position;
			glm::vec2 scroll;
			bool isInWindow;
		};
	}
}