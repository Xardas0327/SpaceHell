#pragma once

#include "Resolution.h"

namespace Learning2DEngine
{
	namespace Render
	{
		class IResolutionRefresher
		{
		public:
			virtual ~IResolutionRefresher() {}

			virtual void RefreshResolution(const Resolution& resolution) = 0;
		};
	}
}