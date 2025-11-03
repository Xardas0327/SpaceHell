#pragma once

#include "EventItem.h"
#include "../System/ICursorRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class CursorEnterEventItem final : public EventItem<bool>
		{
		private:
			System::ICursorRefresher* refresher;
		public:
			CursorEnterEventItem(System::ICursorRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(bool entered) override
			{
				refresher->RefreshCursorInWindows(entered);
			}
		};
	}
}