#pragma once

#include "GameObject.h"
#include "ThreadComponentHandler.h"
#include "BaseUpdaterComponent.h"

namespace Learning2DEngine
{
	namespace System
	{
		class UpdaterComponentHandler : public ThreadComponentHandler<BaseUpdaterComponent>
		{
		private:
			/// <param name="startIndex">Inclusive</param>
			/// <param name="endIndex">Exclusive</param>
			void RunPart(size_t startIndex, size_t endIndex) override
			{
				for (size_t i = startIndex; i < endIndex; ++i)
				{
					//A GameObject will only be destroyed at the end of the frame.
					if (components[i]->isActive && components[i]->gameObject->isActive)
						components[i]->Update();
				}
			}
		public:
			UpdaterComponentHandler()
				: ThreadComponentHandler<BaseUpdaterComponent>()
			{

			}
		};
	}
}