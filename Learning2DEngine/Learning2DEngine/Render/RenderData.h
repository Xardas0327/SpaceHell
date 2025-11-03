#pragma once

#include "../System/GameObject.h"
#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct RenderData
		{
			const System::Component* const component;

			RenderData(const System::Component* component)
				: component(component)
			{
			}

			virtual ~RenderData() = default;
		};
	}
}