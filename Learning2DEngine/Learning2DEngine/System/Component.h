#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class GameObject;

		/// <summary>
		/// The classes, which are inherited from Component,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// Moreover, it is recommended, that the constructor, the Init() and Destroy() of the inherited class is protected
		/// and only the GameObject can use them.
		/// </summary>
		class Component
		{
			friend class GameObject;
		protected:
			Component(GameObject* gameObject)
				: gameObject(gameObject), isActive(true)
			{

			}

			virtual void Init() {}
			virtual void Destroy() {}
		public:
			bool isActive;
			GameObject* const gameObject;

			virtual ~Component() = default;
		};
	}
}