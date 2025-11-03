#pragma once

#include "../Render/RenderData.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		// The TComponent has to be a component, which is derived from System::Component.
		template<class TComponent>
		struct DebugRenderData : public Render::RenderData
		{
			const TComponent* const objectComponent;

			DebugRenderData(const System::Component* component, const TComponent* objectComponent)
				: RenderData(component), objectComponent(objectComponent)
			{
			}
		};
	}
}