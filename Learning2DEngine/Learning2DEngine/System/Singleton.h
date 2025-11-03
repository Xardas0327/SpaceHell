#pragma once

namespace Learning2DEngine
{
	namespace System
	{
		template<typename T>
		class Singleton
		{
		protected:
			Singleton() {};
		public:
			static T& GetInstance()
			{
				static T instance;
				return instance;
			}
		};
	}
}