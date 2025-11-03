#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class IKeyboardRefresher
		{
		public:
			virtual ~IKeyboardRefresher() {}

			virtual void RefreshKeyboard(int key, int scancode, int action, int mode) = 0;
		};
	}
}