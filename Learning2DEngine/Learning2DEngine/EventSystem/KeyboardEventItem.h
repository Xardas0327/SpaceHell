#pragma once

#include "EventItem.h"
#include "../System/IKeyboardRefresher.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class KeyboardEventItem final : public EventItem<int, int, int, int>
		{
		private:
			System::IKeyboardRefresher* refresher;
		public:
			KeyboardEventItem(System::IKeyboardRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(int key, int scancode, int action, int mode) override
			{
				refresher->RefreshKeyboard(key, scancode, action, mode);
			}
		};
	}
}