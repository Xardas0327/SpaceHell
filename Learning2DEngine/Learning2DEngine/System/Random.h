#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		class Random
		{
		private:
			Random() { }
		public:
			/// <summary>
			/// Return a random int within [minInclusive..maxExclusive)
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxExclusive"></param>
			/// <returns></returns>
			static int GetNumber(int minInclusive, int maxExclusive);

			/// <summary>
			/// Return a random float within [minInclusive..maxInclusive]
			/// </summary>
			/// <param name="minInclusive"></param>
			/// <param name="maxInclusive"></param>
			/// <returns></returns>
			static float GetNumber(float minInclusive, float maxInclusive);
		};
	}
}