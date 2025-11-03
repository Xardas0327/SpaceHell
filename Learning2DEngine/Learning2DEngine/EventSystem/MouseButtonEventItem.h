#pragma once

#include "EventItem.h"
#include "../System/ICursorRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class MouseButtonEventItem final : public EventItem<int, int, int>
		{
		private:
			System::ICursorRefresher* refresher;
		public:
			MouseButtonEventItem(System::ICursorRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(int button, int action, int mods) override
			{
				refresher->RefreshMouseButton(button, action, mods);
			}
		};
	}
}