#pragma once

namespace Learning2DEngine
{
	namespace Editor
	{
		constexpr const char* TiledMapSupportedVersion = "1.10";
		constexpr const char* TiledMapSupportedOrientation = "orthogonal";
		constexpr const char* TiledMapSupportedEncoding = "csv";

		// Common
		constexpr const char* TiledMapAttrName = "name";
		constexpr const char* TiledMapAttrWidth = "width";
		constexpr const char* TiledMapAttrHeight = "height";
		constexpr const char* TiledMapAttrTileWidth = "tilewidth";
		constexpr const char* TiledMapAttrTileHeight = "tileheight";
		constexpr const char* TiledMapAttrVisible = "visible";
		constexpr const char* TiledMapAttrOpacity = "opacity";
		constexpr const char* TiledMapAttrTintcolor = "tintcolor";
		constexpr const char* TiledMapAttrOffsetX = "offsetx";
		constexpr const char* TiledMapAttrOffsetY = "offsety";
		constexpr const char* TiledMapAttrX = "x";
		constexpr const char* TiledMapAttrY = "y";

		// Map
		constexpr const char* TiledMapNodeMap = "map";
		constexpr const char* TiledMapAttrVersion = "version";
		constexpr const char* TiledMapAttrTiledVersion = "tiledversion";
		constexpr const char* TiledMapAttrOrientation = "orientation";
		constexpr const char* TiledMapAttrInfinite = "infinite";
		constexpr const char* TiledMapAttrBackgroundColor = "backgroundcolor";

		// Layer
		constexpr const char* TiledMapNodeLayer = "layer";
		constexpr const char* TiledMapNodeData = "data";
		constexpr const char* TiledMapAttrEncoding = "encoding";

		// Tileset
		constexpr const char* TiledMapNodeTileset = "tileset";
		constexpr const char* TiledMapNodeImage = "image";
		constexpr const char* TiledMapAttrFirstGid = "firstgid";
		constexpr const char* TiledMapAttrSource = "source";
		constexpr const char* TiledMapAttrColumns = "columns";
		constexpr const char* TiledMapAttrTileCount = "tilecount";
		constexpr const char* TiledMapAttrSpacing = "spacing";
		constexpr const char* TiledMapAttrMargin = "margin";
		constexpr const char* TiledMapAttrTileOffset = "tileoffset";

		// Tile
		constexpr const char* TiledMapNodeTile = "tile";
		constexpr const char* TiledMapAttrId = "id";

		// Object Group
		constexpr const char* TiledMapNodeObjectGroup = "objectgroup";
		constexpr const char* TiledMapNodeObject = "object";
		constexpr const char* TiledMapNodePoint = "point";
		constexpr const char* TiledMapNodeEllipse = "ellipse";
		constexpr const char* TiledMapNodePolygon = "polygon";
		constexpr const char* TiledMapNodeText = "text";
		constexpr const char* TiledMapAttrGid = "gid";

		// Property
		constexpr const char* TiledMapNodeProperties = "properties";
		constexpr const char* TiledMapNodeProperty = "property";
		constexpr const char* TiledMapAttrType = "type";
		constexpr const char* TiledMapAttrValue = "value";
		constexpr const char* TiledMapPropertyTypeBool = "bool";
		constexpr const char* TiledMapPropertyTypeColor = "color";
		constexpr const char* TiledMapPropertyTypeFile = "file";
		constexpr const char* TiledMapPropertyTypeFloat = "float";
		constexpr const char* TiledMapPropertyTypeInt = "int";
		constexpr const char* TiledMapPropertyTypeString = "string";
		constexpr const char* TiledMapPropertyTypeObject = "object";

		// Smart Property
		constexpr const char* TiledMapSmartLoadBackground = "LoadBackground";
		constexpr const char* TiledMapSmartLayer = "Layer";
		constexpr const char* TiledMapSmartGroupName = "GroupName";
		constexpr const char* TiledMapSmartOnGameObject = "OnGameObject";
		constexpr const char* TiledMapSmartCollider = "Collider";
		constexpr const char* TiledMapSmartColliderValueBox = "Box";
		constexpr const char* TiledMapSmartColliderValueCircle = "Circle";
		constexpr const char* TiledMapSmartColliderSizeX = "SizeX";
		constexpr const char* TiledMapSmartColliderSizeY = "SizeY";
		constexpr const char* TiledMapSmartColliderRadius = "Radius";
		constexpr const char* TiledMapSmartColliderIsKinematic = "IsKinematic";
		constexpr const char* TiledMapSmartColliderIsTrigger = "IsTrigger";
		constexpr const char* TiledMapSmartColliderOffsetX = "OffsetX";
		constexpr const char* TiledMapSmartColliderOffsetY = "OffsetY";
		constexpr const char* TiledMapSmartColliderMaskLayer = "MaskLayer";
	}
}

