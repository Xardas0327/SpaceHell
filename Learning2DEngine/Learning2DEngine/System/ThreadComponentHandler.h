#pragma once

#include <thread>

#include "BaseComponentHandler.h"

namespace Learning2DEngine
{
	namespace System
	{
		template<class T>
		class ThreadComponentHandler : public BaseComponentHandler<T>
		{
		protected:
			//If it is 0, the class will not use threads
			unsigned int maxComponentPerThread;
			std::vector<std::thread> threads;

			virtual void RunPart(size_t startIndex, size_t endIndex) = 0;

			void RunOnThreads()
			{
				size_t oldCapacity = threads.capacity();
				threads.clear();

				size_t threadNumber = this->components.size() / maxComponentPerThread;

				//if remainder is not 0
				if (this->components.size() % maxComponentPerThread)
					++threadNumber;

				//the threads should work more or less with the same number of items
				size_t itemsPerThread = this->components.size() / threadNumber;
				// -1, because one thread is the main thread.
				--threadNumber;
				
				//if the threads vector is too big, it will be reallocated.
				if (oldCapacity > threadNumber * 2)
					threads.shrink_to_fit();

				threads.reserve(threadNumber);

				for (size_t i = 0; i < threadNumber; ++i)
					threads.emplace_back(&ThreadComponentHandler::RunPart, this, i * itemsPerThread, (i + 1) * itemsPerThread);

				RunPart(threadNumber * itemsPerThread, this->components.size());

				for (std::thread& t : threads)
					t.join();
			}

		public:
			ThreadComponentHandler()
				: BaseComponentHandler<T>(), maxComponentPerThread(0), threads()
			{
			}

			virtual void Run() override
			{
				this->RefreshComponents();

				if (this->components.size() == 0)
					return;

				if (maxComponentPerThread == 0 || this->components.size() < maxComponentPerThread)
					RunPart(0, this->components.size());
				else
					RunOnThreads();
			}

			//If it is 0, the class will not use threads
			inline void SetMaxComponentPerThread(unsigned int value)
			{
				maxComponentPerThread = value;
			}

			//If it is 0, the class will not use threads
			inline unsigned int GetMaxComponentPerThread()
			{
				return maxComponentPerThread;
			}
		};
	}
}