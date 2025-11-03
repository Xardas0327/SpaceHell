#include "ColliderComponentHandler.h"

#include <algorithm>

namespace Learning2DEngine
{
	namespace Physics
	{
		ColliderComponentHandler::ColliderComponentHandler()
			: dynamicBoxColliders(), kinematicBoxColliders(), newBoxColliders(), removableBoxColliders(),
			dynamicCircleColliders(), kinematicCircleColliders(), newCircleColliders(), removableCircleColliders(),
			boxMutex(), circleMutex(), threads(), maxColliderPerThread()
		{
		}

		void ColliderComponentHandler::RefreshBoxColliders()
		{
			if (removableBoxColliders.size() > 0)
			{
				auto newDynamicEnd = remove_if(dynamicBoxColliders.begin(), dynamicBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				dynamicBoxColliders.erase(newDynamicEnd, dynamicBoxColliders.end());

				auto newKinematicEnd = remove_if(kinematicBoxColliders.begin(), kinematicBoxColliders.end(),
					[this](BaseBoxColliderComponent* collider)
					{
						auto it = std::find(removableBoxColliders.begin(), removableBoxColliders.end(), collider);
						return it != removableBoxColliders.end();
					});
				kinematicBoxColliders.erase(newKinematicEnd, kinematicBoxColliders.end());

				removableBoxColliders.clear();
			}

			if (newBoxColliders.size() > 0)
			{
				for (auto collider : newBoxColliders)
				{
					switch (collider->type)
					{
					case ColliderType::DYNAMIC:
						dynamicBoxColliders.push_back(collider);
						break;
					case ColliderType::KINEMATIC:
						kinematicBoxColliders.push_back(collider);
						break;
					}
				}

				newBoxColliders.clear();
			}
		}

		void ColliderComponentHandler::RefreshCircleColliders()
		{
			if (removableCircleColliders.size() > 0)
			{
				auto newDynamicEnd = remove_if(dynamicCircleColliders.begin(), dynamicCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				dynamicCircleColliders.erase(newDynamicEnd, dynamicCircleColliders.end());

				auto newKinematicEnd = remove_if(kinematicCircleColliders.begin(), kinematicCircleColliders.end(),
					[this](BaseCircleColliderComponent* collider)
					{
						auto it = std::find(removableCircleColliders.begin(), removableCircleColliders.end(), collider);
						return it != removableCircleColliders.end();
					});
				kinematicCircleColliders.erase(newKinematicEnd, kinematicCircleColliders.end());

				removableCircleColliders.clear();
			}

			if (newCircleColliders.size() > 0)
			{
				for (auto collider : newCircleColliders)
				{
					switch (collider->type)
					{
					case ColliderType::DYNAMIC:
						dynamicCircleColliders.push_back(collider);
						break;
					case ColliderType::KINEMATIC:
						kinematicCircleColliders.push_back(collider);
						break;
					}
				}

				newCircleColliders.clear();
			}
		}

		void ColliderComponentHandler::Add(BaseBoxColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				newBoxColliders.push_back(component);
			}
			else
			{
				newBoxColliders.push_back(component);
			}
		}

		void ColliderComponentHandler::RemoveItem(BaseBoxColliderComponent* component)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newBoxColliders.begin(), newBoxColliders.end(), component);
			if (it != newBoxColliders.end())
				newBoxColliders.erase(it);
			else
				removableBoxColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseBoxColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(boxMutex);
				RemoveItem(component);
			}
			else
			{
				RemoveItem(component);
			}
		}

		void ColliderComponentHandler::Add(BaseCircleColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				newCircleColliders.push_back(component);
			}
			else
			{
				newCircleColliders.push_back(component);
			}
		}

		void ColliderComponentHandler::RemoveItem(BaseCircleColliderComponent* component)
		{
			//Check it, that it is a new one or not.
			auto it = std::find(newCircleColliders.begin(), newCircleColliders.end(), component);
			if (it != newCircleColliders.end())
				newCircleColliders.erase(it);
			else
				removableCircleColliders.push_back(component);
		}

		void ColliderComponentHandler::Remove(BaseCircleColliderComponent* component, bool isThreadSafe)
		{
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(circleMutex);
				RemoveItem(component);
			}
			else
			{
				RemoveItem(component);
			}
		}

		void ColliderComponentHandler::Clear()
		{
			dynamicBoxColliders.clear();
			kinematicBoxColliders.clear();
			newBoxColliders.clear();
			removableBoxColliders.clear();

			dynamicCircleColliders.clear();
			kinematicCircleColliders.clear();
			newCircleColliders.clear();
			removableCircleColliders.clear();
		}

		void ColliderComponentHandler::RunDynamicColliderPart(size_t startIndex, size_t endIndex)
		{
			size_t end;
			if (startIndex < dynamicBoxColliders.size())
			{
				end = endIndex > dynamicBoxColliders.size() ? dynamicBoxColliders.size() : endIndex;
				for (size_t i = startIndex; i < end; ++i)
				{
					if (!dynamicBoxColliders[i]->isActive || !dynamicBoxColliders[i]->gameObject->isActive)
						continue;

					// dynamicBoxColliders
					if (!CheckCollisions(dynamicBoxColliders[i], dynamicBoxColliders, i + 1))
						continue;

					// dynamicCircleColliders
					CheckCollisions(dynamicBoxColliders[i], dynamicCircleColliders, 0);
				}

				// decrease the indexes for another loop
				startIndex = 0;
				endIndex -= end;
			}
			else
			{
				// decrease the indexes for another loop
				startIndex -= dynamicBoxColliders.size();
				endIndex -= dynamicBoxColliders.size();
			}

			end = endIndex > dynamicCircleColliders.size() ? dynamicCircleColliders.size() : endIndex;
			for (size_t i = startIndex; i < end; ++i)
			{
				// All active box colliders were checked in the previous loop
				if (!dynamicCircleColliders[i]->isActive || !dynamicCircleColliders[i]->gameObject->isActive)
					continue;

				// dynamicCircleColliders
				CheckCollisions(dynamicCircleColliders[i], dynamicCircleColliders, i + 1);
			}
		}

		void ColliderComponentHandler::RunKinematicColliderPart(size_t startIndex, size_t endIndex)
		{
			size_t end;
			if (startIndex < kinematicBoxColliders.size())
			{
				end = endIndex > kinematicBoxColliders.size() ? kinematicBoxColliders.size() : endIndex;
				for (size_t i = startIndex; i < end; ++i)
				{
					if (!kinematicBoxColliders[i]->isActive || !kinematicBoxColliders[i]->gameObject->isActive)
						continue;

					// dynamicBoxColliders
					if (!CheckCollisions(kinematicBoxColliders[i], dynamicBoxColliders, 0))
						continue;

					// dynamicCircleColliders
					CheckCollisions(kinematicBoxColliders[i], dynamicCircleColliders, 0);
				}

				// decrease the indexes for another loop
				startIndex = 0;
				endIndex -= end;
			}
			else
			{
				// decrease the indexes for another loop
				startIndex -= kinematicBoxColliders.size();
				endIndex -= kinematicBoxColliders.size();
			}

			end = endIndex > kinematicCircleColliders.size() ? kinematicCircleColliders.size() : endIndex;
			for (size_t i = startIndex; i < end; ++i)
			{
				if (!kinematicCircleColliders[i]->isActive || !kinematicCircleColliders[i]->gameObject->isActive)
					continue;

				// activeBoxColliders
				if (!CheckCollisions(kinematicCircleColliders[i], dynamicBoxColliders, 0))
					continue;

				// activeCircleColliders
				CheckCollisions(kinematicCircleColliders[i], dynamicCircleColliders, 0);
			}
		}

		void ColliderComponentHandler::RunOnThreads()
		{
			size_t oldCapacity = threads.capacity();
			threads.clear();

			size_t dynamicColliderNumber = GetDynamicColliderNumber();
			size_t kinematicColliderNumber = GetKinematicColliderNumber();

			size_t dynamicThreadNumber = dynamicColliderNumber / maxColliderPerThread;
			//if remainder is not 0
			if (dynamicColliderNumber % maxColliderPerThread)
				++dynamicThreadNumber;

			//the threads should work more or less with the same number of items
			size_t dynamicItemsPerThread = dynamicColliderNumber / dynamicThreadNumber;

			size_t kinematicThreadNumber = kinematicColliderNumber / maxColliderPerThread;
			//if remainder is not 0
			if (kinematicColliderNumber % maxColliderPerThread)
				++kinematicThreadNumber;

			//the threads should work more or less with the same number of items
			size_t kinematicItemsPerThread = kinematicColliderNumber / kinematicThreadNumber;

			// -1, because one thread is the main thread.
			size_t allThreadNumber = dynamicThreadNumber + kinematicThreadNumber - 1;

			//if the threads vector is too big, it will be reallocated.
			if (oldCapacity > allThreadNumber * 2)
				threads.shrink_to_fit();

			threads.reserve(allThreadNumber);

			for (size_t i = 0; i < allThreadNumber; ++i)
			{
				if (i < dynamicThreadNumber)
				{
					//If the kinematicThreadNumber is not 0, the last branch should run all remainder
					size_t endIndex = i + 1 == dynamicThreadNumber ? dynamicThreadNumber : (i + 1) * dynamicItemsPerThread;

					threads.emplace_back(
						&ColliderComponentHandler::RunDynamicColliderPart,
						this,
						i * dynamicItemsPerThread,
						endIndex
					);
				}
				else
					threads.emplace_back(
						&ColliderComponentHandler::RunKinematicColliderPart,
						this,
						(i - dynamicThreadNumber) * kinematicItemsPerThread,
						//if it is too big the RunKinematicColliderPart will fix it
						(i - dynamicThreadNumber + 1) * kinematicItemsPerThread
					);
			}

			// The main thread will run the last part
			if (allThreadNumber < dynamicThreadNumber)
				// This is possible if the kinematicThreadNumber is 0.
				RunDynamicColliderPart(allThreadNumber * dynamicThreadNumber, dynamicColliderNumber);
			else
				RunKinematicColliderPart((kinematicThreadNumber -1) * kinematicItemsPerThread, kinematicColliderNumber);


			for (std::thread& t : threads)
				t.join();

		}

		void ColliderComponentHandler::Run()
		{
			RefreshBoxColliders();
			RefreshCircleColliders();

			size_t dynamicColliderNumber = GetDynamicColliderNumber();
			size_t kinematicColliderNumber = GetKinematicColliderNumber();

			if (dynamicColliderNumber == 0 && kinematicColliderNumber == 0)
				return;

			size_t allColliderChecking = dynamicColliderNumber * (dynamicColliderNumber - 1) / 2 + dynamicColliderNumber * kinematicColliderNumber;

			if (maxColliderPerThread == 0 || allColliderChecking < maxColliderPerThread)
			{
				RunDynamicColliderPart(0, dynamicColliderNumber);
				RunKinematicColliderPart(0, kinematicColliderNumber);
			}
			else
				RunOnThreads();
		}

	}
}