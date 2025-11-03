#pragma once

#include "../Render/Resolution.h"
#include "../Render/IResolutionRefresher.h"
#include "EventItem.h"

namespace Learning2DEngine
{
	namespace EventSystem
	{
		class ResolutionEventItem final : public EventItem<Render::Resolution>
		{
		private:
			Render::IResolutionRefresher* refresher;
		public:
			ResolutionEventItem(Render::IResolutionRefresher* refresher)
				: refresher(refresher)
			{

			}

			void Call(Render::Resolution resolution) override
			{
				refresher->RefreshResolution(resolution);
			}
		};
	}
}