#pragma once

#include <glm/glm.hpp>

#include "../Render/Texture2D.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		struct AnimationFrame
		{
			Render::Texture2D* texture;
			/// <summary>
			/// The texture coordinate order:
			/// Top Left,
			/// Top Right,
			/// Bottom Right,
			/// Bottom Left
			/// </summary>
			glm::mat4x2 uvMatrix;
			float time;
		};
	}
}