#pragma once

#include "../Render/Resolution.h"
#include "../System/GameObject.h"
#include "../System/UpdaterComponent.h"
#include "../System/Transform.h"
#include "../UI/FontSizePair.h"
#include "../UI/SimpleText2DRenderComponent.h"

namespace Learning2DEngine
{
	namespace Object
	{
		class FpsShower : public System::UpdaterComponent,
							public UI::SimpleText2DRenderComponent
		{
			friend class System::GameObject;
		protected:
			float previousTime;
			int fps;

			FpsShower(
				System::GameObject* gameObject,
				const UI::FontSizePair& fontSizePair,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));

			void Init() override;
			void Destroy() override;
			void Update() override;

		public:
			static FpsShower* CreateFpsShowerObject(
				const System::Transform& transform,
				const UI::FontSizePair& fontSizePair,
				int layer = 0,
				glm::vec4 color = glm::vec4(1.0f));
		};
	}
}