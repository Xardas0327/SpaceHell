#pragma once

#include "../Physics/BaseBoxColliderComponent.h"
#include "../Render/RendererComponent.h"

#include "DebugRenderData.h"
#include "DebugBoxColliderRenderer.h"

namespace Learning2DEngine
{
	namespace DebugTool
	{
		// It is used in Render Mode only
		// and it works well with renderers if the renderers use the camera view matrix.
		class DebugBoxColliderRenderComponent : public Render::RendererComponent<DebugRenderData<Physics::BaseBoxColliderComponent>, DebugBoxColliderRenderer>
		{
			friend class System::GameObject;
		private:
			static const std::string id;

		protected:
			DebugBoxColliderRenderComponent(System::GameObject* gameObject, const Physics::BaseBoxColliderComponent* collider);

			const std::string& GetId() const override;
			DebugBoxColliderRenderer* GetInitedRenderer() override;
			void DestroyRenderer() override;
		};
	}
}