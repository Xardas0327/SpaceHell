#pragma once

#include "EventItem.h"
#include "../System/ICursorRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class ScrollEventItem final : public EventItem<double, double>
		{
		private:
			System::ICursorRefresher* refresher;
		public:
			ScrollEventItem(System::ICursorRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(double xoffset, double yoffset) override
			{
				refresher->RefreshScroll(xoffset, yoffset);
			}
		};
	}
}