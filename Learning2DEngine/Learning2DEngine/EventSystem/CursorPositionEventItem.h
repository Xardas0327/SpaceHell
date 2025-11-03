#pragma once

#include "EventItem.h"
#include "../System/ICursorRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class CursorPositionEventItem final : public EventItem<double, double>
		{
		private:
			System::ICursorRefresher* refresher;
		public:
			CursorPositionEventItem(System::ICursorRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(double xpos, double ypos) override
			{
				refresher->RefreshCursorPosition(xpos, ypos);
			}
		};
	}
}