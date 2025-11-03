#pragma once

#include <vector>
#include <algorithm>
#include <mutex>

#include "IComponentHandler.h"

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class BaseComponentHandler : public IComponentHandler
		{
		protected:
			std::vector<T*> components;
			std::vector<T*> newComponents;
			std::vector<T*> removableComponents;
			std::mutex mutex;

			BaseComponentHandler()
				: components(), newComponents(), removableComponents(), mutex()
			{
			}

			virtual void RefreshComponents()
			{
				if (removableComponents.size() > 0)
				{
					auto newEnd = remove_if(components.begin(), components.end(),
						[this](T* component)
						{
							auto it = std::find(removableComponents.begin(), removableComponents.end(), component);
							return it != removableComponents.end();
						});
					components.erase(newEnd, components.end());

					removableComponents.clear();
				}

				if (newComponents.size() > 0)
				{
					components.insert(components.end(), newComponents.begin(), newComponents.end());
					newComponents.clear();
				}
			}

			void RemoveItem(T* component)
			{
				//Check it, that it is a new one or not.
				auto it = std::find(newComponents.begin(), newComponents.end(), component);
				if (it != newComponents.end())
					newComponents.erase(it);
				else
					removableComponents.push_back(component);
			}
		public:
			virtual void Add(T* component, bool isThreadSafe)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(mutex);
					newComponents.push_back(component);
				}
				else
				{
					newComponents.push_back(component);
				}
			}

			virtual void Remove(T* component, bool isThreadSafe)
			{
				if (isThreadSafe)
				{
					std::lock_guard<std::mutex> lock(mutex);
					RemoveItem(component);
				}
				else
				{
					RemoveItem(component);
				}
			}

			virtual void Clear() override
			{
				components.clear();
				newComponents.clear();
				removableComponents.clear();
			}
		};
	}
}