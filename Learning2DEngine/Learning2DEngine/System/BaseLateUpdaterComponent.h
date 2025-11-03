#pragma once

#include "Component.h"

namespace Learning2DEngine
{
	namespace System
	{
		/// <summary>
		/// The classes, which are inherited from BaseUpdateComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Please check for more info about System::Component
		/// </summary>
		class BaseLateUpdaterComponent : public virtual Component
		{
		protected:
			BaseLateUpdaterComponent(GameObject* gameObject)
				: Component(gameObject)
			{

			}
		public:
			virtual void LateUpdate() = 0;
		};
	}
}
