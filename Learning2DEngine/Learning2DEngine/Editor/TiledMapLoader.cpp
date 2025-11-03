#include "TiledMapLoader.h"

#include <sstream>
#include <exception>
#include <memory>
#include <GLFW/glfw3.h>
#include <rapidxml/rapidxml_utils.hpp>

#include "../DebugTool/DebugMacro.h"
#include "../DebugTool/Log.h"
#include "../Render/RenderManager.h"
#include "../Render/SpriteRenderComponent.h"
#include "../System/ResourceManager.h"
#include "../Physics/BoxColliderComponent.h"
#include "../Physics/CircleColliderComponent.h"


namespace Learning2DEngine
{
    using namespace Render;
    using namespace System;
    using namespace Physics;

    namespace Editor
    {
        TiledMap TiledMapLoader::LoadFromFile(const std::string& filePath,
            const std::map<std::string, std::string>& textureMap)
        {
#if L2DE_DEBUG
            float startTime = static_cast<float>(glfwGetTime());
#endif
            auto doc = std::make_unique<rapidxml::xml_document<>>();
            try
            {
                std::size_t folderIndex = filePath.find_last_of("/");
                std::string folderPath = folderIndex == std::string::npos ? "" : filePath.substr(0, folderIndex) + "/";

                rapidxml::file<> xmlFile(filePath.c_str());
                doc->parse<0>(xmlFile.data());
                TiledMap map;

                // Map node
                auto mapNode = doc->first_node(TiledMapNodeMap);
                if (mapNode == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the map node is missing.");
                    return map;
                }

                TiledMapLoader::LoadMapAttributes(map, mapNode);

                std::vector<TiledMapTileset> tilesets;
                TiledMapLoader::LoadTilesets(mapNode, folderPath, textureMap, tilesets);
                TiledMapLoader::LoadLayers(map, mapNode, tilesets);
                TiledMapLoader::LoadObjectLayers(map, mapNode, tilesets, folderPath);

#if L2DE_DEBUG
                float loadingTime = static_cast<float>(glfwGetTime()) - startTime;
                L2DE_LOG_INFO("TiledMapLoader: " + filePath + " file's loading time: " + std::to_string(loadingTime) + "s");
#endif

                return map;
            }
            catch (std::exception& e)
            {
                L2DE_LOG_ERROR((std::string)"TiledMapLoader: " + e.what());
                throw e;
            }
        }

        void TiledMapLoader::LoadMapAttributes(TiledMap& map, rapidxml::xml_node<>* mapNode)
        {
            bool foundWidth = false;
            bool foundHeight = false;
            bool foundTileWidth = false;
            bool foundTileHeight = false;
            for (auto attr = mapNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), TiledMapAttrVersion) == 0)
                {
                    map.version = attr->value();
                    if (map.version != TiledMapSupportedVersion)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the map version is not supported: "
                            + map.version + "\n Supported version: " + TiledMapSupportedVersion);
                    }
                }
                else if (strcmp(attr->name(), TiledMapAttrTiledVersion) == 0)
                {
                    map.tiledVersion = attr->value();
                }
                else if (strcmp(attr->name(), TiledMapAttrOrientation) == 0)
                {
                    map.orientation = attr->value();
                    if (map.orientation != TiledMapSupportedOrientation)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the map orientation is not supported: "
                            + map.orientation + "\n Supported version: " + TiledMapSupportedOrientation);
                    }
                }
                else if (strcmp(attr->name(), TiledMapAttrWidth) == 0)
                {
                    map.width = std::atoi(attr->value());
                    foundWidth = true;

                    if (map.width <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map width should be bigger then 0: " + std::to_string(map.width));
                }
                else if (strcmp(attr->name(), TiledMapAttrHeight) == 0)
                {
                    map.height = std::atoi(attr->value());
                    foundHeight = true;

                    if (map.height <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map height should be bigger then 0: " + std::to_string(map.height));
                }
                else if (strcmp(attr->name(), TiledMapAttrTileWidth) == 0)
                {
                    map.tileWidth = std::atoi(attr->value());
                    foundTileWidth = true;
                    if (map.tileWidth <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map tile width should be bigger then 0: " + std::to_string(map.tileWidth));
                }
                else if (strcmp(attr->name(), TiledMapAttrTileHeight) == 0)
                {
                    map.tileHeight = std::atoi(attr->value());
                    foundTileHeight = true;
                    if (map.tileHeight <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the map tile height should be bigger then 0: " + std::to_string(map.tileWidth));
                }
                else if (strcmp(attr->name(), TiledMapAttrInfinite) == 0)
                {
                    map.infinite = strcmp(attr->value(), "1") == 0 ? true : false;
                    if (map.infinite)
                        L2DE_LOG_WARNING("TiledMapLoader: the map is infinite. This is not supported.");
                }
                else if (strcmp(attr->name(), TiledMapAttrBackgroundColor) == 0)
                {
                    map.backgroundColor = TiledMapLoader::ConvertStringToColor(attr->value());
                }
            }

            if (!foundWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the map width is missing.");
            if (!foundHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the map height is missing.");
            if (!foundTileWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the map tile width is missing.");
            if (!foundTileHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the map tile height is missing.");

            if (TiledMapLoader::LoadMapBackground(mapNode))
            {
                RenderManager::GetInstance().SetClearColor(
                    map.backgroundColor.r,
                    map.backgroundColor.g,
                    map.backgroundColor.b,
                    map.backgroundColor.a);
            }
        }

        glm::vec4 TiledMapLoader::ConvertStringToColor(const std::string& hex)
        {
            glm::vec4 color(1.0f);

            if (hex.empty())
                return color;

            if (hex.length() != 7 && hex.length() != 9)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the background color is not valid: " + hex);
                return color;
            }

            int shift = 0;
            if (hex.length() == 9)
            {
                shift = 2;
                try
                {
                    int alpha = std::stoi(hex.substr(1, 2), nullptr, 16);
                    color.a = static_cast<float>(alpha) / 255.0f;
                }
                catch (const std::exception&)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the background color - alpha is not valid: " + hex);
                    return color;
                }
            }

            try
            {
                int r = std::stoi(hex.substr(1 + shift, 2), nullptr, 16);
                color.r = static_cast<float>(r) / 255.0f;

                int g = std::stoi(hex.substr(3 + shift, 2), nullptr, 16);
                color.g = static_cast<float>(g) / 255.0f;

                int b = std::stoi(hex.substr(5 + shift, 2), nullptr, 16);
                color.b = static_cast<float>(b) / 255.0f;
            }
            catch (const std::exception&)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the background color - RGB is not valid: " + hex);
                return color;
            }

            return color;
        }

        void TiledMapLoader::LoadTilesets(
            rapidxml::xml_node<>* mapNode,
            const std::string& folderPath,
            const std::map<std::string, std::string>& textureMap,
            std::vector<TiledMapTileset>& loadedTilesets)
        {
            for (
                auto mapTileset = mapNode->first_node(TiledMapNodeTileset);
                mapTileset != nullptr;
                mapTileset = mapTileset->next_sibling(TiledMapNodeTileset)
                )
            {
                TiledMapTileset tileset;
                tileset.firstGid = std::atoi(mapTileset->first_attribute(TiledMapAttrFirstGid)->value());
                if (tileset.firstGid <= 0)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset firstgid should be bigger then 0.");
                    continue;
                }

                std::string sourceName = mapTileset->first_attribute(TiledMapAttrSource)->value();
                if (sourceName.empty())
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset source is empty.");
                    continue;
                }

                if (TiledMapLoader::LoadTileset(folderPath, sourceName, textureMap, tileset))
                    loadedTilesets.push_back(tileset);
            }
        }

        bool TiledMapLoader::LoadTileset(
            const std::string& folderPath,
            const std::string& sourceName,
            const std::map<std::string, std::string>& textureMap,
            TiledMapTileset& tiledMapObject)
        {
            rapidxml::file<> xmlFile((folderPath + sourceName).c_str());
            auto doc = std::make_unique<rapidxml::xml_document<>>();
            doc->parse<0>(xmlFile.data());

            auto tilesetNode = doc->first_node(TiledMapNodeTileset);
            if (tilesetNode == nullptr)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset node is missing.");
                return false;
            }

            bool foundVersion = false;
            bool foundTileWidth = false;
            bool foundTileHeight = false;
            bool foundColumns = false;
            bool foundTileCount = false;
            std::string imageName = "";

            for (auto attr = tilesetNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), TiledMapAttrVersion) == 0)
                {
                    std::string version = attr->value();
                    foundVersion = true;

                    if (version != TiledMapSupportedVersion)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the " + sourceName + " tileset version is not supported: "
                            + version + "\n Supported version: " + TiledMapSupportedVersion);
                    }
                }
                else if (strcmp(attr->name(), TiledMapAttrTileWidth) == 0)
                {
                    tiledMapObject.tileSize.x = static_cast<float>(std::atoi(attr->value()));
                    foundTileWidth = true;

                    if (tiledMapObject.tileSize.x <= 0.0f)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width should be bigger then 0.");
                }
                else if (strcmp(attr->name(), TiledMapAttrTileHeight) == 0)
                {
                    tiledMapObject.tileSize.y = static_cast<float>(std::atoi(attr->value()));
                    foundTileHeight = true;

                    if (tiledMapObject.tileSize.y <= 0.0f)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height should be bigger then 0.");
                }
                else if (strcmp(attr->name(), TiledMapAttrColumns) == 0)
                {
                    tiledMapObject.columns = std::atoi(attr->value());
                    foundColumns = true;

                    if (tiledMapObject.columns <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns should be bigger then 0.");
                }
                else if (strcmp(attr->name(), TiledMapAttrTileCount) == 0)
                {
                    tiledMapObject.tileCount = std::atoi(attr->value());
                    foundTileCount = true;

                    if (tiledMapObject.tileCount <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tilecount should be bigger then 0.");
                }
                else if (strcmp(attr->name(), TiledMapAttrName) == 0)
                {
                    imageName = attr->value();
                }
                else if (strcmp(attr->name(), TiledMapAttrSpacing) == 0)
                {
                    tiledMapObject.spacing = std::atoi(attr->value());
                }
                else if (strcmp(attr->name(), TiledMapAttrMargin) == 0)
                {
                    tiledMapObject.margin = std::atoi(attr->value());
                }
            }

            if (!foundVersion)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset version is missing.");
            if (!foundTileWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile width is missing.");
            if (!foundTileHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tile height is missing.");
            if (!foundColumns)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset columns is missing.");
            if (!foundTileCount)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset tilecount is missing.");

            if (imageName.empty())
            {
                L2DE_LOG_ERROR("TiledMapLoader: the tileset name is empty.");
                return false;
            }

            auto tileOffsetNode = tilesetNode->first_node(TiledMapAttrTileOffset);
            if (tileOffsetNode != nullptr)
            {
                auto offsetXAttr = tileOffsetNode->first_attribute(TiledMapAttrX);
                if (offsetXAttr != nullptr)
                {
                    tiledMapObject.tileOffset.x = static_cast<float>(std::atoi(offsetXAttr->value()));
                }

                auto offsetYAttr = tileOffsetNode->first_attribute(TiledMapAttrY);
                if (offsetYAttr != nullptr)
                {
                    tiledMapObject.tileOffset.y = static_cast<float>(std::atoi(offsetYAttr->value()));
                }
            }

            auto imageNode = tilesetNode->first_node(TiledMapNodeImage);
            if (imageNode == nullptr)
            {
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " tileset image node is missing.");
                return false;
            }

            bool foundWidth = false;
            bool foundHeight = false;
            std::string imageSource = "";

            for (auto attr = imageNode->first_attribute();
                attr != nullptr;
                attr = attr->next_attribute())
            {
                if (strcmp(attr->name(), TiledMapAttrSource) == 0)
                {
                    imageSource = attr->value();
                }
                else if (strcmp(attr->name(), TiledMapAttrWidth) == 0)
                {
                    tiledMapObject.imageSize.x = static_cast<float>(std::atoi(attr->value()));
                    foundWidth = true;

                    if (tiledMapObject.imageSize.x <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image width should be bigger then 0.");
                }
                else if (strcmp(attr->name(), TiledMapAttrHeight) == 0)
                {
                    tiledMapObject.imageSize.y = static_cast<float>(std::atoi(attr->value()));
                    foundHeight = true;

                    if (tiledMapObject.imageSize.y <= 0)
                        L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image height should be bigger then 0.");
                }
            }
            if (!foundWidth)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image width is missing.");
            if (!foundHeight)
                L2DE_LOG_ERROR("TiledMapLoader: the " + sourceName + " image height is missing.");

            auto& resourceManager = ResourceManager::GetInstance();
            if (textureMap.find(imageName) != textureMap.end())
            {
                tiledMapObject.texture = &resourceManager.GetTexture(textureMap.at(imageName));
            }
            else if (resourceManager.IsTextureExist(imageName))
            {
                tiledMapObject.texture = &resourceManager.GetTexture(imageName);
            }
            else
            {

                if (imageSource.empty())
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the tileset image source is empty.");
                    return false;
                }

                tiledMapObject.texture = &resourceManager.LoadTextureFromFile(
                    imageName,
                    (folderPath + imageSource).c_str(),
                    Texture2DSettings(true));
            }

            TiledMapLoader::LoadProperties(tiledMapObject.commonProperties, tilesetNode, folderPath);
            TiledMapLoader::LoadTilesProperties(tiledMapObject.uniqueProperties, tilesetNode, sourceName, folderPath);
            TiledMapLoader::LoadTilesObjectItems(tiledMapObject.objects, tilesetNode, sourceName, folderPath);

            return true;
        }

        void TiledMapLoader::LoadLayers(TiledMap& map, rapidxml::xml_node<>* mapNode, std::vector<TiledMapTileset>& tilesets)
        {
            int layerId = 0;
            for (
                auto layerNode = mapNode->first_node(TiledMapNodeLayer);
                layerNode != nullptr;
                layerNode = layerNode->next_sibling(TiledMapNodeLayer)
                )
            {
                bool foundWidth = false;
                bool foundHeight = false;
                bool visible = true;
                float opacity = 1.0f;
                glm::vec4 tintColor(1.0f);
                glm::vec2 offset(0.0f);

                for (auto attr = layerNode->first_attribute();
                    attr != nullptr;
                    attr = attr->next_attribute())
                {
                    if (strcmp(attr->name(), TiledMapAttrWidth) == 0)
                    {
                        foundWidth = true;
                        if (std::atoi(attr->value()) != map.GetWidth())
                        {
                            L2DE_LOG_WARNING("TiledMapLoader: the layer width is not equal to the map width.");
                        }
                    }
                    else if (strcmp(attr->name(), TiledMapAttrHeight) == 0)
                    {
                        foundHeight = true;
                        if (std::atoi(attr->value()) != map.GetHeight())
                        {
                            L2DE_LOG_WARNING("TiledMapLoader: the layer width is not equal to the map width.");
                        }
                    }
                    else if (strcmp(attr->name(), TiledMapAttrVisible) == 0)
                    {
                        visible = strcmp(attr->value(), "1") == 0;
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOpacity) == 0)
                    {
                        opacity = static_cast<float>(std::atof(attr->value()));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrTintcolor) == 0)
                    {
                        tintColor = TiledMapLoader::ConvertStringToColor(attr->value());
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOffsetX) == 0)
                    {
                        offset.x = static_cast<float>(std::atof(attr->value()));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOffsetY) == 0)
                    {
                        offset.y = static_cast<float>(std::atof(attr->value()));
                    }
                }

                if (!foundWidth || !foundHeight)
                {
                    if (!foundWidth)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: the layer data width attribute is missing.");
                    }
                    if (!foundHeight)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: the layer data height attribute is missing.");
                    }
                    continue;
                }

                int overrideLayerId = 0;
                bool useOverrideLayerId = TiledMapLoader::LoadLayerId(layerNode, overrideLayerId);

                auto dataNode = layerNode->first_node(TiledMapNodeData);
                if (dataNode == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data node is missing.");
                    continue;
                }

                auto encodingAttr = dataNode->first_attribute(TiledMapAttrEncoding);
                if (encodingAttr == nullptr)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data encoding attribute is missing.");
                    continue;
                }

                std::string encoding = encodingAttr->value();
                if (encoding != TiledMapSupportedEncoding)
                {
                    L2DE_LOG_ERROR("TiledMapLoader: the layer data encoding is not supported: "
                        + encoding + "\n Supported encoding: " + TiledMapSupportedEncoding);
                    continue;
                }

                std::stringstream dataText(dataNode->value());
                std::string line;

                int row = 0;
                while (std::getline(dataText, line)) {
                    //Remove \r
                    line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
                    if (line.empty())
                        continue;

                    std::stringstream lineStream(line);
                    std::string token;

                    int column = 0;
                    while (std::getline(lineStream, token, ',')) {
                        if (!token.empty()) {
                            int id = std::atoi(token.c_str());
                            if (id > 0)
                            {
                                TiledMapLoader::CreateGameObject(
                                    map,
                                    LayerItemData(
                                        tilesets,
                                        id,
                                        row,
                                        column,
                                        useOverrideLayerId ? overrideLayerId : layerId,
                                        visible,
                                        opacity,
                                        offset,
                                        tintColor
                                    )
                                );
                            }

                        }
                        ++column;
                    }
                    ++row;
                }
                ++layerId;
            }
        }

        void TiledMapLoader::LoadObjectLayers(
            TiledMap& map,
            rapidxml::xml_node<>* mapNode,
            std::vector<TiledMapTileset>& tilesets,
            const std::string& folderPath
        )
        {
            int layerId = 0;
            for (
                auto objectLayerNode = mapNode->first_node(TiledMapNodeObjectGroup);
                objectLayerNode != nullptr;
                objectLayerNode = objectLayerNode->next_sibling(TiledMapNodeObjectGroup)
                )
            {
                bool visible = true;
                float opacity = 1.0f;
                glm::vec4 tintColor(1.0f);
                glm::vec2 offset(0.0f);

                for (auto attr = objectLayerNode->first_attribute();
                    attr != nullptr;
                    attr = attr->next_attribute())
                {
                    if (strcmp(attr->name(), TiledMapAttrVisible) == 0)
                    {
                        visible = strcmp(attr->value(), "1") == 0;
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOpacity) == 0)
                    {
                        opacity = static_cast<float>(std::atof(attr->value()));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrTintcolor) == 0)
                    {
                        tintColor = TiledMapLoader::ConvertStringToColor(attr->value());
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOffsetX) == 0)
                    {
                        offset.x = static_cast<float>(std::atof(attr->value()));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrOffsetY) == 0)
                    {
                        offset.y = static_cast<float>(std::atof(attr->value()));
                    }
                }

                int overrideLayerId = 0;
                bool useOverrideLayerId = TiledMapLoader::LoadLayerId(objectLayerNode, overrideLayerId);

                std::vector<ObjectItem> objects;
                LoadObjectItems(objects, objectLayerNode, folderPath);

                for (auto& object : objects)
                {
                    TiledMapLoader::CreateGameObject(
                        map,
                        ObjectLayerItemData(
                            tilesets,
                            object,
                            useOverrideLayerId ? overrideLayerId : layerId,
                            visible,
                            opacity,
                            offset,
                            tintColor
                        )
                    );
                }

                ++layerId;
            }
        }

        void TiledMapLoader::LoadProperties(
            std::map<std::string, System::Property>& loadedProperties,
            rapidxml::xml_node<>* node,
            const std::string& folderPath
        )
        {
            if (node == nullptr)
                return;

            if (strcmp(node->name(), TiledMapNodeProperties) != 0)
            {
                node = node->first_node(TiledMapNodeProperties);
            }

            if (node == nullptr)
                return;

            for (
                auto property = node->first_node(TiledMapNodeProperty);
                property != nullptr;
                property = property->next_sibling(TiledMapNodeProperty)
                )
            {
                auto nameAttr = property->first_attribute(TiledMapAttrName);
                if (nameAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the property name attribute is missing.");
                    continue;
                }

                auto valueAttr = property->first_attribute(TiledMapAttrValue);
                if (valueAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: the property value attribute is missing.");
                    continue;
                }

                auto typeAttr = property->first_attribute(TiledMapAttrType);
                PropertyType type = PropertyType::STRING;
                if (typeAttr != nullptr)
                {
                    std::string typeStr = typeAttr->value();
                    if (typeStr == TiledMapPropertyTypeBool)
                        type = PropertyType::BOOL;
                    else if (typeStr == TiledMapPropertyTypeColor)
                        type = PropertyType::COLOR;
                    else if (typeStr == TiledMapPropertyTypeFile)
                        type = PropertyType::FILE;
                    else if (typeStr == TiledMapPropertyTypeFloat)
                        type = PropertyType::FLOAT;
                    else if (typeStr == TiledMapPropertyTypeInt)
                        type = PropertyType::INT;
                    else if (typeStr == TiledMapPropertyTypeString)
                        type = PropertyType::STRING;
                    else
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the property type is not valid: " + typeStr);
                        continue;
                    }
                }

                switch (type)
                {
                case PropertyType::BOOL:
                    loadedProperties.emplace(nameAttr->value(), Property(strcmp(valueAttr->value(), "true") == 0 ? true : false));
                    break;
                case PropertyType::COLOR:
                    loadedProperties.emplace(nameAttr->value(), Property(TiledMapLoader::ConvertStringToColor(valueAttr->value())));
                    break;
                case PropertyType::FILE:
                    loadedProperties.emplace(nameAttr->value(), Property(folderPath + valueAttr->value(), true));
                    break;
                case PropertyType::FLOAT:
                    loadedProperties.emplace(nameAttr->value(), Property(static_cast<float>(std::atof(valueAttr->value()))));
                    break;
                case PropertyType::INT:
                    loadedProperties.emplace(nameAttr->value(), Property(std::atoi(valueAttr->value())));
                    break;
                case PropertyType::STRING:
                    loadedProperties.emplace(nameAttr->value(), Property(valueAttr->value(), false));
                    break;
                default:
                    break;
                }
            }
        }

        void TiledMapLoader::LoadTilesProperties(
            std::map<int, std::map<std::string, System::Property>>& loadedProperties,
            rapidxml::xml_node<>* node,
            const std::string& sourceName,
            const std::string& folderPath
        )
        {
            for (
                auto tile = node->first_node(TiledMapNodeTile);
                tile != nullptr;
                tile = tile->next_sibling(TiledMapNodeTile)
                )
            {
                auto idAttr = tile->first_attribute(TiledMapAttrId);
                if (idAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id attribute is missing.");
                    continue;
                }

                int id = std::atoi(idAttr->value());
                if (loadedProperties.count(id))
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id is duplicated: " + std::to_string(id));
                    continue;
                }

                std::map<std::string, System::Property> tileProperties;
                LoadProperties(tileProperties, tile, folderPath);

                if (!tileProperties.empty())
                    loadedProperties.emplace(id, std::move(tileProperties));
            }
        }

        void TiledMapLoader::LoadTilesObjectItems(
            std::map<int, std::vector<ObjectItem>>& loadedObjects,
            rapidxml::xml_node<>* node,
            const std::string& sourceName,
            const std::string& folderPath
        )
        {
            for (
                auto tile = node->first_node(TiledMapNodeTile);
                tile != nullptr;
                tile = tile->next_sibling(TiledMapNodeTile)
                )
            {
                auto idAttr = tile->first_attribute(TiledMapAttrId);
                if (idAttr == nullptr)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id attribute is missing.");
                    continue;
                }

                int id = std::atoi(idAttr->value());
                if (loadedObjects.count(id))
                {
                    L2DE_LOG_WARNING("TiledMapLoader: " + sourceName + " the tile id is duplicated: " + std::to_string(id));
                    continue;
                }

                std::vector<ObjectItem> tileObjects;
                LoadObjectItems(tileObjects, tile, folderPath);

                if (!tileObjects.empty())
                    loadedObjects.emplace(id, std::move(tileObjects));
            }
        }

        void TiledMapLoader::LoadObjectItems(std::vector<ObjectItem>& loadedObjects, rapidxml::xml_node<>* node, const std::string& folderPath)
        {
            if (node == nullptr)
                return;

            if (strcmp(node->name(), TiledMapNodeObjectGroup) != 0)
            {
                node = node->first_node(TiledMapNodeObjectGroup);
            }

            if (node == nullptr)
                return;

            for (
                auto object = node->first_node(TiledMapNodeObject);
                object != nullptr;
                object = object->next_sibling(TiledMapNodeObject)
                )
            {
                auto child = object->first_node();

                //the properties will be check later
                if (child != nullptr && strcmp(child->name(), TiledMapNodeProperties) == 0)
                    child = child->next_sibling();

                ObjectType type = ObjectType::BOX;
                if (child == nullptr)
                {
                    //it should be box.
                }
                else if (strcmp(child->name(), TiledMapNodePolygon) == 0)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: polygon object type is not supported.");
                    continue;
                }
                else if (strcmp(child->name(), TiledMapNodeText) == 0)
                {
                    L2DE_LOG_WARNING("TiledMapLoader: text object type is not supported.");
                    continue;
                }
                else if (strcmp(child->name(), TiledMapNodePoint) == 0)
                {
                    type = ObjectType::POINT;
                }
                else if (strcmp(child->name(), TiledMapNodeEllipse) == 0)
                {
                    type = ObjectType::ELLIPSE;
                }

                bool foundGid = false;
                bool foundX = false;
                bool foundY = false;
                bool foundWidth = false;
                bool foundHeight = false;
                int gid;
                glm::vec2 position;
                glm::vec2 size;
                bool visible = true;
                for (auto attr = object->first_attribute();
                    attr != nullptr;
                    attr = attr->next_attribute())
                {
                    if (strcmp(attr->name(), TiledMapAttrX) == 0)
                    {
                        position.x = static_cast<float>(std::atof(attr->value()));
                        foundX = true;
                    }
                    else if (strcmp(attr->name(), TiledMapAttrY) == 0)
                    {
                        position.y = static_cast<float>(std::atof(attr->value()));
                        foundY = true;
                    }
                    else if (strcmp(attr->name(), TiledMapAttrWidth) == 0)
                    {
                        size.x = static_cast<float>(std::atof(attr->value()));
                        foundWidth = true;

                        if (size.x <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object width should be bigger then 0: " + std::to_string(size.x));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrHeight) == 0)
                    {
                        size.y = static_cast<float>(std::atof(attr->value()));
                        foundHeight = true;

                        if (size.y <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object height should be bigger then 0: " + std::to_string(size.y));
                    }
                    else if (strcmp(attr->name(), TiledMapAttrGid) == 0)
                    {
                        gid = std::atoi(attr->value());
                        type = ObjectType::IMAGE;
                        if (gid <= 0)
                            L2DE_LOG_ERROR("TiledMapLoader: an object gid should be bigger then 0: " + std::to_string(gid));
                        else
                            foundGid = true;
                    }
                    else if (strcmp(attr->name(), TiledMapAttrVisible) == 0)
                    {
                        visible = strcmp(attr->value(), "1") == 0;
                    }
                }

                std::map<std::string, System::Property> properties;
                TiledMapLoader::LoadProperties(properties, object, folderPath);

                switch (type)
                {
                case ObjectType::POINT:
                    if (!foundX || !foundY)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a point object can't be created, because some data is missing.");
                        continue;
                    }
                    loadedObjects.push_back(ObjectItem(std::move(
                        ObjectPoint(position, std::move(properties), visible)
                    )));
                    break;
                case ObjectType::BOX:
                    if (!foundX || !foundY || !foundWidth || !foundHeight)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a box object can't be created, because some data is missing.");
                        continue;
                    }
                    loadedObjects.push_back(ObjectItem(std::move(
                        ObjectBox(position, size, std::move(properties), visible)
                    )));
                    break;
                case ObjectType::ELLIPSE:
                    if (!foundX || !foundY || !foundWidth || !foundHeight)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a ellipse object can't be created, because some data is missing.");
                        continue;
                    }
                    //We want radius instead of diameter.
                    size /= 2.0f;
                    //At the moment, there is circle, not ellipse collider.
                    if (size.x != size.y)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: an ellipse object should have the same width and height. Only the width will be used.");
                        size.y = size.x;
                    }
                    loadedObjects.push_back(ObjectItem(std::move(
                        ObjectEllipse(position, size, std::move(properties), visible)
                    )));
                    break;
                case ObjectType::IMAGE:
                    if (!foundX || !foundY || !foundWidth || !foundHeight || !foundGid)
                    {
                        L2DE_LOG_ERROR("TiledMapLoader: a ellipse object can't be created, because some data is missing.");
                        continue;
                    }
                    loadedObjects.push_back(ObjectItem(std::move(
                        ObjectImage(position, size, std::move(properties), gid, visible)
                    )));
                    break;
                }
            }
        }

        bool TiledMapLoader::LoadMapBackground(rapidxml::xml_node<>* mapNode)
        {
            std::map<std::string, System::Property> properties;
            TiledMapLoader::LoadProperties(properties, mapNode);
#if L2DE_DEBUG
            int tooMuchProperties = 1;
#endif

            if (properties.count(TiledMapSmartLoadBackground))
            {
#if L2DE_DEBUG
                ++tooMuchProperties;
#endif
                if (properties[TiledMapSmartLoadBackground].GetType() == PropertyType::BOOL)
                {
                    return properties[TiledMapSmartLoadBackground].GetBool();
                }
                else
                {
                    L2DE_LOG_ERROR((std::string)"TiledMapLoader: the " + TiledMapSmartLoadBackground + " property type is not valid. It should be Bool.");
                }
            }

#if L2DE_DEBUG
            if (properties.size() >= tooMuchProperties)
            {
                L2DE_LOG_WARNING((std::string)"TiledMapLoader: The Map's properties won't be processed. Except: " + TiledMapSmartLoadBackground);
            }
#endif
            return false;
        }

        bool TiledMapLoader::LoadLayerId(rapidxml::xml_node<>* layerNode, int& layerId)
        {
            std::map<std::string, System::Property> properties;
            TiledMapLoader::LoadProperties(properties, layerNode);

#if L2DE_DEBUG
            int tooMuchProperties = 1;
#endif

            if (properties.count(TiledMapSmartLayer))
            {
#if L2DE_DEBUG
                ++tooMuchProperties;
#endif
                if (properties[TiledMapSmartLayer].GetType() == PropertyType::INT)
                {
                    layerId = properties[TiledMapSmartLayer].GetInt();
                    return true;
                }
                else
                {
                    L2DE_LOG_ERROR((std::string)"TiledMapLoader: the " + TiledMapSmartLayer + " property type is not valid. It should be Int.");
                }
            }

#if L2DE_DEBUG
            if (properties.size() >= tooMuchProperties)
            {
                L2DE_LOG_WARNING((std::string)"TiledMapLoader: The Layer's and Object Layer's properties won't be processed. Except: " +
                    TiledMapSmartLayer);
            }
#endif
            return false;
        }

        const TiledMapTileset* TiledMapLoader::GetTilesetFromGid(const std::vector<TiledMapTileset>& tilesets, int gid)
        {
            for (auto& tileset : tilesets)
            {
                if (tileset.HasNumber(gid))
                {
                    return &tileset;
                }
            }
            return nullptr;
        }

        void TiledMapLoader::CreateGameObject(TiledMap& map, const LayerItemData& itemData)
        {
            auto selectedTileset = TiledMapLoader::GetTilesetFromGid(itemData.tilesets, itemData.gid);

            if (selectedTileset == nullptr)
            {
                L2DE_LOG_WARNING("TiledMapLoader: the tile id " + std::to_string(itemData.gid) + " is not valid.");
                return;
            }

            Transform transform(
                glm::vec2(
                    static_cast<float>(itemData.column) * map.GetTileWidth() + itemData.offset.x + selectedTileset->tileOffset.x,
                    static_cast<float>(itemData.row + 1) * map.GetTileHeight()
                    + itemData.offset.y + selectedTileset->tileOffset.y - selectedTileset->tileSize.y),
                selectedTileset->tileSize
            );
            auto gameObject = GameObjectManager::GetInstance().CreateGameObject(transform, itemData.visible);

            auto color = itemData.tintColor;
            color.a *= itemData.opacity;

            auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                RendererMode::RENDER,
                *selectedTileset->texture,
                itemData.layerId,
                color);
            renderer->data.uvMatrix = selectedTileset->GetUV(itemData.gid);

            std::map<std::string, System::Property> properties;
            if (selectedTileset->commonProperties.size() > 0 ||
                selectedTileset->uniqueProperties.count(selectedTileset->GetLocalId(itemData.gid)) > 0)
            {
                properties = selectedTileset->commonProperties;

                if (selectedTileset->uniqueProperties.count(selectedTileset->GetLocalId(itemData.gid)) > 0)
                {
                    for (auto& prop : selectedTileset->uniqueProperties.at(selectedTileset->GetLocalId(itemData.gid)))
                    {
                        properties[prop.first] = prop.second;
                    }
                }

                TiledMapLoader::AddColliderToGameObject(gameObject, properties);
            }

            if (selectedTileset->objects.count(selectedTileset->GetLocalId(itemData.gid)))
            {

                for (const auto& object : selectedTileset->objects.at(selectedTileset->GetLocalId(itemData.gid)))
                {
                    //The image is not supported in the tileds, even in Editor.
                    switch (object.type)
                    {
                    case ObjectType::POINT:
                    {
                        const ObjectPoint* point = static_cast<const ObjectPoint*>(object.GetData());

                        GameObject* objectGameObject = GameObjectManager::GetInstance().CreateGameObject(
                            Transform(gameObject->transform.GetPosition() + point->position),
                            point->visible
                        );

                        std::map<std::string, System::Property> pointProperties = point->properties;
                        TiledMapLoader::AddGameObjectToMap(map, objectGameObject, pointProperties);

                        if (pointProperties.size() > 0)
                            objectGameObject->AddComponent<PropertyComponent>(pointProperties);
                    }
                    break;
                    case ObjectType::BOX:
                        CreateColliderFromObjectItem<ObjectBox>(map, object, gameObject, properties);
                        break;
                    case ObjectType::ELLIPSE:
                        CreateColliderFromObjectItem<ObjectEllipse>(map, object, gameObject, properties);
                        break;
                    }
                }
            }

            TiledMapLoader::AddGameObjectToMap(map, gameObject, properties);

            if (properties.size() > 0)
                gameObject->AddComponent<PropertyComponent>(std::move(properties));
        }

        void TiledMapLoader::CreateGameObject(TiledMap& map, const ObjectLayerItemData& itemData)
        {
            GameObject* gameObject = nullptr;
            std::map<std::string, System::Property> properties = itemData.objectItem.GetData()->properties;

            switch (itemData.objectItem.type)
            {
            case ObjectType::POINT:
            {
                const ObjectPoint* point = static_cast<const ObjectPoint*>(itemData.objectItem.GetData());

                gameObject = GameObjectManager::GetInstance().CreateGameObject(
                    Transform(point->position + itemData.offset),
                    itemData.visible && point->visible
                );
            }
            break;
            case ObjectType::BOX:
            {
                const ObjectBox* box = static_cast<const ObjectBox*>(itemData.objectItem.GetData());
                gameObject = GameObjectManager::GetInstance().CreateGameObject(
                    Transform(box->position + itemData.offset),
                    itemData.visible
                );

                TiledMapLoader::AddColliderToGameObject(gameObject, *box, properties, false);
            }
            break;
            case ObjectType::ELLIPSE:
            {
                const ObjectEllipse* ellipse = static_cast<const ObjectEllipse*>(itemData.objectItem.GetData());
                gameObject = GameObjectManager::GetInstance().CreateGameObject(
                    Transform(ellipse->position + itemData.offset),
                    itemData.visible
                );

                TiledMapLoader::AddColliderToGameObject(gameObject, *ellipse, properties, false);
            }
            break;
            case ObjectType::IMAGE:
            {
                const ObjectImage* image = static_cast<const ObjectImage*>(itemData.objectItem.GetData());
                if (image->gid > 0)
                {
                    auto selectedTileset = TiledMapLoader::GetTilesetFromGid(itemData.tilesets, image->gid);

                    if (selectedTileset == nullptr)
                    {
                        L2DE_LOG_WARNING("TiledMapLoader: the tile id " + std::to_string(image->gid) + " is not valid.");
                        return;
                    }

                    gameObject = GameObjectManager::GetInstance().CreateGameObject(
                        Transform(image->position + itemData.offset, image->size),
                        itemData.visible
                    );

                    //The position in Tiled is bottom-left, but in Learning2DEngine is top-left.
                    gameObject->transform.AddPosition(glm::vec2(0.0f, -image->size.y));

                    auto color = itemData.tintColor;
                    color.a *= itemData.opacity;
                    auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
                        RendererMode::RENDER,
                        *selectedTileset->texture,
                        itemData.layerId,
                        color);
                    renderer->data.uvMatrix = selectedTileset->GetUV(image->gid);
					renderer->isActive = image->visible;

                    //it will have all tileset properties and the object properties.
                    std::map<std::string, System::Property> allProperties = selectedTileset->commonProperties;
                    if (selectedTileset->uniqueProperties.count(selectedTileset->GetLocalId(image->gid)))
                    {
                        for (auto& prop : selectedTileset->uniqueProperties.at(selectedTileset->GetLocalId(image->gid)))
                        {
                            allProperties[prop.first] = prop.second;
                        }
                    }
                    for (auto& prop : properties)
                    {
                        allProperties[prop.first] = prop.second;
                    }
                    properties = std::move(allProperties);
                    TiledMapLoader::AddColliderToGameObject(gameObject, properties);
                }
            }
            break;
            }

            if (gameObject != nullptr)
            {
                TiledMapLoader::AddGameObjectToMap(map, gameObject, properties);

                if (properties.size() > 0)
                    gameObject->AddComponent<PropertyComponent>(properties);
            }
        }

        void TiledMapLoader::AddColliderToGameObject(GameObject* gameObject, std::map<std::string, Property>& properties)
        {
            if (properties.count(TiledMapSmartCollider))
            {
                if (properties[TiledMapSmartCollider].GetType() != PropertyType::STRING)
                {
                    L2DE_LOG_WARNING((std::string)"TiledMapLoader: the " + TiledMapSmartCollider + " should be string.");
                    return;
                }

                auto type = ColliderType::DYNAMIC;
                auto mode = ColliderMode::COLLIDER;
                glm::vec2 offset(0.0f, 0.0f);
                int32_t maskLayer = ~0;

                if (properties.count(TiledMapSmartColliderIsKinematic) &&
                    properties[TiledMapSmartColliderIsKinematic].GetBool())
                {
                    type = ColliderType::KINEMATIC;
                }

                if (properties.count(TiledMapSmartColliderIsTrigger) &&
                    properties[TiledMapSmartColliderIsTrigger].GetBool())
                {
                    mode = ColliderMode::TRIGGER;
                }

                if (properties.count(TiledMapSmartColliderOffsetX))
                {
                    offset.x = properties[TiledMapSmartColliderOffsetX].GetFloat();
                }

                if (properties.count(TiledMapSmartColliderOffsetY))
                {
                    offset.y = properties[TiledMapSmartColliderOffsetY].GetFloat();
                }

                if (properties.count(TiledMapSmartColliderMaskLayer))
                {
                    maskLayer = properties[TiledMapSmartColliderMaskLayer].GetInt();
                }

                bool addedCollider = false;
                if (properties[TiledMapSmartCollider].GetString() == TiledMapSmartColliderValueBox)
                {
                    glm::vec2 size(gameObject->transform.GetScale());
                    if (properties.count(TiledMapSmartColliderSizeX))
                    {
                        size.x = properties[TiledMapSmartColliderSizeX].GetFloat();
                    }

                    if (properties.count(TiledMapSmartColliderSizeY))
                    {
                        size.y = properties[TiledMapSmartColliderSizeY].GetFloat();
                    }

                    gameObject->AddComponent<BoxColliderComponent>(size, type, mode, offset, maskLayer);
                    addedCollider = true;
                    properties.erase(TiledMapSmartColliderSizeX);
                    properties.erase(TiledMapSmartColliderSizeY);
                }
                else if (properties[TiledMapSmartCollider].GetString() == TiledMapSmartColliderValueCircle)
                {
                    float radius = gameObject->transform.GetScale().x / 2.0f;
                    if (properties.count(TiledMapSmartColliderRadius))
                    {
                        radius = properties[TiledMapSmartColliderRadius].GetFloat();
                    }

                    gameObject->AddComponent<CircleColliderComponent>(radius, type, mode, offset, maskLayer);
                    addedCollider = true;
                    properties.erase(TiledMapSmartColliderRadius);
                }
                else
                {
                    L2DE_LOG_WARNING((std::string)"TiledMapLoader: the " + TiledMapSmartCollider + " has invalid value. Supported values: "
                        + TiledMapSmartColliderValueBox + ", " + TiledMapSmartColliderValueCircle);
                }

                if (addedCollider)
                {
                    properties.erase(TiledMapSmartCollider);
                    properties.erase(TiledMapSmartColliderIsKinematic);
                    properties.erase(TiledMapSmartColliderIsTrigger);
                    properties.erase(TiledMapSmartColliderOffsetX);
                    properties.erase(TiledMapSmartColliderOffsetY);
                    properties.erase(TiledMapSmartColliderMaskLayer);
                }
            }
        }

        void TiledMapLoader::AddColliderToGameObject(
            GameObject* gameObject,
            const ObjectBox& object,
            std::map<std::string, Property>& properties,
            bool useObjectPositionAsOffset
        )
        {
            auto size = object.size;
            auto type = ColliderType::DYNAMIC;
            auto mode = ColliderMode::COLLIDER;
            glm::vec2 offset(0.0f, 0.0f);
            if (useObjectPositionAsOffset)
                offset = object.position;
            int32_t maskLayer = ~0;

            if (properties.count(TiledMapSmartColliderIsKinematic) &&
                properties[TiledMapSmartColliderIsKinematic].GetBool())
            {
                type = ColliderType::KINEMATIC;
            }

            if (properties.count(TiledMapSmartColliderIsTrigger) &&
                properties[TiledMapSmartColliderIsTrigger].GetBool())
            {
                mode = ColliderMode::TRIGGER;
            }

            if (properties.count(TiledMapSmartColliderOffsetX))
            {
                offset.x = properties[TiledMapSmartColliderOffsetX].GetFloat();
            }

            if (properties.count(TiledMapSmartColliderOffsetY))
            {
                offset.y = properties[TiledMapSmartColliderOffsetY].GetFloat();
            }

            if (properties.count(TiledMapSmartColliderMaskLayer))
            {
                maskLayer = properties[TiledMapSmartColliderMaskLayer].GetInt();
            }

            if (properties.count(TiledMapSmartColliderSizeX))
            {
                size.x = properties[TiledMapSmartColliderSizeX].GetFloat();
            }

            if (properties.count(TiledMapSmartColliderSizeY))
            {
                size.y = properties[TiledMapSmartColliderSizeY].GetFloat();
            }

            auto collider = gameObject->AddComponent<BoxColliderComponent>(size, type, mode, offset, maskLayer);
            collider->isActive = object.visible;

            //The L2DE_TILEDMAP_SMART_COLLIDER is not used here, because the object is box.
            properties.erase(TiledMapSmartColliderSizeX);
            properties.erase(TiledMapSmartColliderSizeY);
            properties.erase(TiledMapSmartColliderIsKinematic);
            properties.erase(TiledMapSmartColliderIsTrigger);
            properties.erase(TiledMapSmartColliderOffsetX);
            properties.erase(TiledMapSmartColliderOffsetY);
            properties.erase(TiledMapSmartColliderMaskLayer);
        }

        void TiledMapLoader::AddColliderToGameObject(
            GameObject* gameObject,
            const ObjectEllipse& object,
            std::map<std::string, Property>& properties,
            bool useObjectPositionAsOffset
        )
        {
            float radius = object.size.x;
            auto type = ColliderType::DYNAMIC;
            auto mode = ColliderMode::COLLIDER;
            glm::vec2 offset(0.0f, 0.0f);
            if (useObjectPositionAsOffset)
                offset = object.position;
            int32_t maskLayer = ~0;

            if (properties.count(TiledMapSmartColliderIsKinematic) &&
                properties[TiledMapSmartColliderIsKinematic].GetBool())
            {
                type = ColliderType::KINEMATIC;
            }

            if (properties.count(TiledMapSmartColliderIsTrigger) &&
                properties[TiledMapSmartColliderIsTrigger].GetBool())
            {
                mode = ColliderMode::TRIGGER;
            }

            if (properties.count(TiledMapSmartColliderOffsetX))
            {
                offset.x = properties[TiledMapSmartColliderOffsetX].GetFloat();
            }

            if (properties.count(TiledMapSmartColliderOffsetY))
            {
                offset.y = properties[TiledMapSmartColliderOffsetY].GetFloat();
            }

            if (properties.count(TiledMapSmartColliderMaskLayer))
            {
                maskLayer = properties[TiledMapSmartColliderMaskLayer].GetInt();
            }

            if (properties.count(TiledMapSmartColliderRadius))
            {
                radius = properties[TiledMapSmartColliderRadius].GetFloat();
            }

            auto collider = gameObject->AddComponent<CircleColliderComponent>(radius, type, mode, offset, maskLayer);
            collider->isActive = object.visible;

            //The L2DE_TILEDMAP_SMART_COLLIDER is not used here, because the object is box.
            properties.erase(TiledMapSmartColliderRadius);
            properties.erase(TiledMapSmartColliderIsKinematic);
            properties.erase(TiledMapSmartColliderIsTrigger);
            properties.erase(TiledMapSmartColliderOffsetX);
            properties.erase(TiledMapSmartColliderOffsetY);
            properties.erase(TiledMapSmartColliderMaskLayer);
        }

        void TiledMapLoader::AddGameObjectToMap(
            TiledMap& map,
            GameObject* gameObject,
            std::map<std::string, Property>& properties
        )
        {
            if (properties.count(TiledMapSmartGroupName))
            {
                if (properties[TiledMapSmartGroupName].GetType() != PropertyType::STRING)
                {
                    L2DE_LOG_WARNING((std::string)"TiledMapLoader: the " + TiledMapSmartGroupName + " should be string.");
                }
                else
                {
                    map.groupedGameObjects[
                        properties[TiledMapSmartGroupName].GetString()
                    ].push_back(gameObject);

                    properties.erase(TiledMapSmartGroupName);
                    return;
                }
            }

            map.gameObjects.push_back(gameObject);
        }
    }
}