#include "GameObjectManager.h"

namespace Learning2DEngine
{
	namespace System
	{

		GameObjectManager::GameObjectManager()
			: gameObjects(), removableGameObjects(), addMutex(), removeMutex(), isThreadSafe(false)
		{
		}

		GameObject* GameObjectManager::CreateGameObject(bool isActive)
		{
			auto gameObject = new GameObject(isActive);

			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(addMutex);
				gameObjects.push_back(gameObject);
			}
			else
				gameObjects.push_back(gameObject);

			return gameObject;
		}

		GameObject* GameObjectManager::CreateGameObject(const Transform& transform, bool isActive)
		{
			auto gameObject = new GameObject(transform, isActive);

			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(addMutex);
				gameObjects.push_back(gameObject);
			}
			else
				gameObjects.push_back(gameObject);

			return gameObject;
		}

		void GameObjectManager::DestroyGameObject(GameObject* gameObject)
		{
			gameObject->isActive = false;
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(removeMutex);
				removableGameObjects.push_back(gameObject);
			}
			else
				removableGameObjects.push_back(gameObject);
		}

		void GameObjectManager::DestroyGameObject(Component* component)
		{
			component->gameObject->isActive = false;
			if (isThreadSafe)
			{
				std::lock_guard<std::mutex> lock(removeMutex);
				removableGameObjects.push_back(component->gameObject);
			}
			else
				removableGameObjects.push_back(component->gameObject);
		}

		void GameObjectManager::DestroyMarkedGameObjects()
		{
			if (removableGameObjects.size() > 0)
			{
				auto newEnd = remove_if(gameObjects.begin(), gameObjects.end(),
					[this](GameObject* gameObject)
					{
						auto it = std::find(removableGameObjects.begin(), removableGameObjects.end(), gameObject);
						if (it != removableGameObjects.end())
						{
							gameObject->Destroy();
							delete gameObject;
							return true;
						}

						return false;
					});
				gameObjects.erase(newEnd, gameObjects.end());

				removableGameObjects.clear();
			}
		}

		void GameObjectManager::DestroyAllGameObjects()
		{
			for (GameObject* gameObject : gameObjects)
			{
				gameObject->Destroy();
				delete gameObject;
			}

			gameObjects.clear();
			removableGameObjects.clear();
		}
	}
}