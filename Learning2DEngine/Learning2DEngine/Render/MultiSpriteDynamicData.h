#pragma once

#include "BaseColorDynamicData.h"

namespace Learning2DEngine
{
	namespace Render
	{
		struct MultiSpriteDynamicData : public BaseColorDynamicData
		{
			float uvMatrix[8];
			float textureId;
			float isUseCameraView;
		};
	}
}