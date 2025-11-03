#pragma once

#include <list>

#include "EventItem.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		template<class ...Params>
		class EventHandler
		{
		private:
			std::list<EventItem<Params...>*> events;

		public:
			inline void Add(EventItem<Params...>* e)
			{
				events.push_back(e);
			}

			inline void Remove(EventItem<Params...>* e)
			{
				events.remove(e);
			}

			inline void Clear()
			{
				events.clear();
			}

			void Invoke(Params... params)
			{
				for (auto& item : events)
				{
					item->Call(params...);
				}
			}
		};
	}
}