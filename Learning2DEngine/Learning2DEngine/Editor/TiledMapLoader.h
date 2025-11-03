#pragma once

#include <vector>
#include <map>
#include <string>

#include <rapidxml/rapidxml.hpp>

#include "TiledMap.h"
#include "TiledMapTileset.h"
#include "ObjectItem.h"
#include "TiledMapConstant.h"
#include "LayerItemData.h"
#include "ObjectLayerItemData.h"
#include "../System/Property.h"
#include "../System/GameObject.h"
#include "../System/GameObjectManager.h"
#include "../System/PropertyComponent.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class TiledMapLoader final
		{
			friend class TiledMapLoaderTest;
		private:
			TiledMapLoader() = default;

			static void LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode);
			static glm::vec4 ConvertStringToColor(const std::string& hex);

			static void LoadTilesets(
				rapidxml::xml_node<>* mapNode,
				const std::string& folderPath,
				const std::map<std::string, std::string>& textureMap,
				std::vector<TiledMapTileset>& loadedTilesets);

			static bool LoadTileset(
				const std::string& folderPath, 
				const std::string& sourceName,
				const std::map<std::string, std::string>& textureMap,
				TiledMapTileset& tiledMapObject);

			static void LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets);

			static void LoadObjectLayers(
				TiledMap& map,
				rapidxml::xml_node<>* mapNode,
				std::vector<TiledMapTileset>& tilesets,
				const std::string& folderPath = ""
			);

			// The folderPath is used when the property type is file.
			static void LoadProperties(
				std::map<std::string, System::Property>& loadedProperties,
				rapidxml::xml_node<>* node,
				const std::string& folderPath = ""
			);

			// The folderPath is used when the property type is file.
			// The sourceName is used for the logging only.
			static void LoadTilesProperties(
				std::map<int, std::map<std::string, System::Property>>& loadedProperties,
				rapidxml::xml_node<>* node,
				const std::string& sourceName,
				const std::string& folderPath = ""
			);

			// The folderPath is used when the property type is file.
			static void LoadObjectItems(std::vector<ObjectItem>& loadedObjects, rapidxml::xml_node<>* node, const std::string& folderPath = "");

			// The folderPath is used when the property type is file.
			// The sourceName is used for the logging only.
			static void LoadTilesObjectItems(
				std::map<int, std::vector<ObjectItem>>& loadedObjects,
				rapidxml::xml_node<>* node,
				const std::string& sourceName,
				const std::string& folderPath = ""
			);

			static bool LoadMapBackground(rapidxml::xml_node<>* mapNode);
			static bool LoadLayerId(rapidxml::xml_node<>* layerNode, int& layerId);

			static const TiledMapTileset* GetTilesetFromGid(const std::vector<TiledMapTileset>& tilesets, int gid);
			static void CreateGameObject(TiledMap& map, const LayerItemData& itemData);
			static void CreateGameObject(TiledMap& map, const ObjectLayerItemData& itemData);

			template<class T>
			static void CreateColliderFromObjectItem(
				TiledMap& map,
				const ObjectItem& objectItem,
				System::GameObject* tiledGameObject,
				std::map<std::string, System::Property>& tiledProperties)
			{
				const auto& object = static_cast<const T&>(*objectItem.GetData());
				System::GameObject* objectGameObject = nullptr;
				std::map<std::string, System::Property> objectProperties = object.properties;
				bool isOnGameObject = objectProperties.count(TiledMapSmartOnGameObject) &&
					objectProperties[TiledMapSmartOnGameObject].GetBool();
				objectProperties.erase(TiledMapSmartOnGameObject);

				//the object will be the tiledGameObject or a new game object
				if (isOnGameObject)
					objectGameObject = tiledGameObject;
				else
					objectGameObject = System::GameObjectManager::GetInstance().CreateGameObject(tiledGameObject->transform);

				TiledMapLoader::AddColliderToGameObject(objectGameObject, object, objectProperties, true);

				//if it is on the main game object, add the properties to the main properties
				if (isOnGameObject)
				{
					for (const auto& kv : objectProperties) {
						tiledProperties[kv.first] = kv.second;
					}
				}
				else
				{
					TiledMapLoader::AddGameObjectToMap(map, objectGameObject, objectProperties);

					if (objectProperties.size() > 0)
						objectGameObject->AddComponent<System::PropertyComponent>(std::move(objectProperties));
				}
			}

			static void AddColliderToGameObject(System::GameObject* gameObject, std::map<std::string, System::Property>& properties);

			//the object.position will be the offset if the useObjectPositionAsOffset is true
			static void AddColliderToGameObject(
				System::GameObject* gameObject,
				const ObjectBox& object,
				std::map<std::string, System::Property>& properties,
				bool useObjectPositionAsOffset
			);
			//the object.position will be the offset if the useObjectPositionAsOffset is true
			static void AddColliderToGameObject(
				System::GameObject* gameObject,
				const ObjectEllipse& object,
				std::map<std::string, System::Property>& properties,
				bool useObjectPositionAsOffset
			);

			static void AddGameObjectToMap(
				TiledMap& map,
				System::GameObject* gameObject,
				std::map<std::string, System::Property>& properties
			);
		public:
			~TiledMapLoader() = default;

			static TiledMap LoadFromFile(
				const std::string& filePath,
				const std::map<std::string, std::string>& textureMap = std::map<std::string, std::string>()
			);
		};
	}
}