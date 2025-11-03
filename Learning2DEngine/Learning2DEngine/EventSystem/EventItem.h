#pragma once

namespace Learning2DEngine
{
	namespace EventSystem
	{
		template<class ...Params>
		class EventItem
		{
		public:
			virtual ~EventItem() {}

			virtual void Call(Params... params) = 0;
		};
	}
}