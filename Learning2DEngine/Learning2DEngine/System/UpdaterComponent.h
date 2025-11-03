#pragma once

#include "BaseUpdaterComponent.h"
#include "ComponentManager.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// The classes, which are inherited from UpdaterComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about `Component` and `BaseUpdaterComponent`.
		/// </summary>
		class UpdaterComponent : public BaseUpdaterComponent
		{
			friend class GameObject;
		protected:
			UpdaterComponent(GameObject* gameObject)
				: BaseUpdaterComponent(gameObject)
			{

			}

			/// <summary>
			/// If this function is override, it must call the UpdaterComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				ComponentManager::GetInstance().AddToUpdate(this);
			}

			/// <summary>
			/// If this function is override, it must call the UpdaterComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				ComponentManager::GetInstance().RemoveFromUpdate(this);
			}
		};
	}
}