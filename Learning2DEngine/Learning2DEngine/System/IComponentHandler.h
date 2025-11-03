#pragma once

#include <vector>

namespace Learning2DEngine
{
	namespace System
	{
		class IComponentHandler
		{
		public:
			virtual ~IComponentHandler() = default;

			virtual void Clear() = 0;

			virtual void Run() = 0;
		};
	}
}
