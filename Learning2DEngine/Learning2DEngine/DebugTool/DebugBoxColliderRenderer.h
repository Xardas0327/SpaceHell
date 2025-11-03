#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/BaseBoxColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugBoxColliderRenderer : public DebugColliderRenderer<Physics::BaseBoxColliderComponent>, public System::Singleton<DebugBoxColliderRenderer>
		{
			friend class System::Singleton<DebugBoxColliderRenderer>;
		private:
			DebugBoxColliderRenderer();

			void InitVao() override;
		public:
			void Draw(Render::RendererMode rendererMode, int layer) override;
		};
	}
}