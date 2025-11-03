#pragma once

#include "BaseRenderer.h"

namespace Learning2DEngine
{
	namespace Render
	{
		template<class TDynamicData>
		class BaseMultiRenderer : public BaseRenderer
		{
		protected:
			GLuint vboDynamicObject;
			size_t maxObjectSize;

			TDynamicData* dynamicData;

			BaseMultiRenderer()
				: BaseRenderer(), vboDynamicObject(0), maxObjectSize(0), dynamicData(nullptr)
			{

			}

			virtual void DestroyObject()
			{
				BaseRenderer::DestroyObject();

				if (vboDynamicObject)
					glDeleteBuffers(1, &vboDynamicObject);

				if (dynamicData != nullptr)
				{
					delete[] dynamicData;
				}
			}

			virtual void CalcDynamicDataSize(size_t maxDynamicSize)
			{
				//if the size is not enough or too big, it will be reallocated.
				if (maxDynamicSize > maxObjectSize || maxObjectSize > maxDynamicSize * 2)
				{
					//It allocates 20% more space, so that it does not have to allocate again
					//if there are some dynamic renderers. 
					maxObjectSize = static_cast<size_t>(
						static_cast<float>(maxDynamicSize) * 1.2f
						);

					glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);
					glBufferData(GL_ARRAY_BUFFER, sizeof(TDynamicData) * maxObjectSize, NULL, GL_DYNAMIC_DRAW);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					delete[] dynamicData;
					dynamicData = new TDynamicData[maxObjectSize];
				}
			}
		};
	}
}