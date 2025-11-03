#pragma once

#include <glad/glad.h>
#include <vector>
#include <map>

#include "../System/Singleton.h"
#include "../Physics/BaseCircleColliderComponent.h"

#include "DebugColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		class DebugCircleColliderRenderer : public DebugColliderRenderer<Physics::BaseCircleColliderComponent>, public System::Singleton<DebugCircleColliderRenderer>
		{
			friend class System::Singleton<DebugCircleColliderRenderer>;
		private:
			static constexpr const int CircleSegment = 50;

			DebugCircleColliderRenderer();

			void InitVao() override;
		public:
			void Draw(Render::RendererMode rendererMode, int layer) override;
		};
	}
}