#pragma once

#include <utility>

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// First is the source factor.
		/// Second is the destination factor.
		/// </summary>
		typedef std::pair<unsigned int, unsigned int> BlendFuncFactor;
	}
}