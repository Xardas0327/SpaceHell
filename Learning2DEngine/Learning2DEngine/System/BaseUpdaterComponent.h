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
		class BaseUpdaterComponent : public virtual Component
		{
		protected:
			BaseUpdaterComponent(GameObject* gameObject)
				: Component(gameObject)
			{

			}
		public:
			virtual void Update() = 0;
		};
    }
}