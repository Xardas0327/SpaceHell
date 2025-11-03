#include "Random.h"

#include <random>
#include <exception>
#include <string>

#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
	namespace System
	{
		int Random::GetNumber(int minInclusive, int maxExclusive)
		{
			if (minInclusive >= maxExclusive)
			{
				std::string errorMessage = "Random::GetNumber: The minInclusive ("+std::to_string(minInclusive)
					+ ") is not lesser than maxExclusive ("+ std::to_string(maxExclusive) + ").";
				L2DE_LOG_ERROR(errorMessage);
				throw std::exception(errorMessage.c_str());
			}

			//now the max number is exclusive.
			--maxExclusive;

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(minInclusive, maxExclusive);

			return dis(gen);
		}

		float Random::GetNumber(float minInclusive, float maxInclusive)
		{
			if (minInclusive >= maxInclusive)
			{
				std::string errorMessage = "Random::GetNumber: The minInclusive (" + std::to_string(minInclusive) 
					+ ") is not lesser than maxInclusive (" + std::to_string(maxInclusive) + ").";
				L2DE_LOG_ERROR(errorMessage);
				throw std::exception(errorMessage.c_str());
			}

			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(minInclusive, std::nextafter(maxInclusive, DBL_MAX));

			return static_cast<float>(dis(gen));
		}
	}
}