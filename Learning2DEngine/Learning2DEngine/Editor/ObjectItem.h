#pragma once

#include "ObjectType.h"
#include "BaseObjectData.h"
#include "ObjectPoint.h"
#include "ObjectBox.h"
#include "ObjectEllipse.h"
#include "ObjectImage.h"

namespace Learning2DEngine
{
	namespace Editor
	{
		class ObjectItem final
		{
		private:
			BaseObjectData* data;

		public:
			const ObjectType type;

			ObjectItem(ObjectPoint&& point)
				: type(ObjectType::POINT), data(new ObjectPoint(std::move(point)))
			{
			}

			ObjectItem(ObjectBox&& box)
				: type(ObjectType::BOX), data(new ObjectBox(std::move(box)))
			{
			}

			ObjectItem(ObjectEllipse&& ellipse)
				: type(ObjectType::ELLIPSE), data(new ObjectEllipse(std::move(ellipse)))
			{
			}

			ObjectItem(ObjectImage&& image)
				: type(ObjectType::IMAGE), data(new ObjectImage(std::move(image)))
			{
			}

			ObjectItem(const ObjectItem& other)
				: type(other.type), data(nullptr)
			{
				switch (type)
				{
				case ObjectType::POINT:
					data = new ObjectPoint(*static_cast<ObjectPoint*>(other.data));
					break;
				case ObjectType::BOX:
					data = new ObjectBox(*static_cast<ObjectBox*>(other.data));
					break;
				case ObjectType::ELLIPSE:
					data = new ObjectEllipse(*static_cast<ObjectEllipse*>(other.data));
					break;
				case ObjectType::IMAGE:
					data = new ObjectImage(*static_cast<ObjectImage*>(other.data));
					break;
				}
			}

			~ObjectItem()
			{
				delete data;
			}

			inline const BaseObjectData* GetData() const
			{
				return data;
			}
		};
	}
}