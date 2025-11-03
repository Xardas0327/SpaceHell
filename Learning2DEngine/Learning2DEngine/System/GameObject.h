#pragma once

#include <vector>
#include <utility>

#include "Transform.h"
#include "Component.h"

namespace Learning2DEngine
{
	namespace System
	{
		class GameObjectManager;

		class GameObject final
		{
			friend class GameObjectManager;
		private:
			std::vector<Component*> components;

			GameObject(bool isActive = true)
				: isActive(isActive), transform(), components()
			{
			}

			GameObject(const Transform& transform, bool isActive = true)
				:isActive(isActive), transform(transform), components()
			{
			}
		public:
			bool isActive;
			Transform transform;

			~GameObject() = default;

			GameObject(const GameObject&) = delete;
			GameObject& operator=(const GameObject&) = delete;

			void Destroy()
			{
				for (Component* component : components)
				{
					if (component != nullptr)
					{
						component->Destroy();
						delete component;
					}
				}
				components.clear();
			}

			template <class TComponent, class ...Params>
			TComponent* AddComponent(Params&&... params)
			{
				TComponent* component = new TComponent(this, std::forward<Params>(params)...);
				components.push_back(component);
				component->Init();

				return component;
			}

			template <class TComponent>
			TComponent* GetComponent()
			{
				TComponent* selectedComponent = nullptr;
				for (Component* component : components)
				{
					selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
						break;
				}

				return selectedComponent;
			}

			template <class TComponent>
			const TComponent* GetComponent() const
			{
				TComponent* selectedComponent = nullptr;
				for (Component* component : components)
				{
					selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
						break;
				}

				return selectedComponent;
			}

			template <class TComponent>
			std::vector<TComponent*> GetComponents()
			{
				std::vector<TComponent*> selectedComponents;
				for (Component* component : components)
				{
					TComponent* selectedComponent = dynamic_cast<TComponent*>(component);

					if (selectedComponent != nullptr)
					{
						selectedComponents.push_back(selectedComponent);
					}
				}

				return selectedComponents;
			}
		};
	}
}