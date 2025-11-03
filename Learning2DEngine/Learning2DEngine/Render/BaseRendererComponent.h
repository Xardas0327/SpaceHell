#pragma once

#include <string>
#include <utility>

#include "../System/Component.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from BaseRendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// The TRenderData should be a class, which is inhereted from IRenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about System::Component
		/// </summary>
		template<class TRenderData, class TRenderer>
		class BaseRendererComponent : public virtual System::Component
		{
		private:
			int layer;

		protected:
			template <class ...TRenderDataParams>
			BaseRendererComponent(System::GameObject* gameObject, int layer = 0, TRenderDataParams&&... renderDataParams)
				: System::Component(gameObject), layer(layer), data(this, std::forward<TRenderDataParams>(renderDataParams)...)
			{

			}

			virtual const std::string& GetId() const = 0;

			virtual TRenderer* GetInitedRenderer() = 0;
			virtual void DestroyRenderer() = 0;
		public:
			TRenderData data;

			virtual void SetLayer(int value)
			{
				layer = value;
			}

			inline int GetLayer() const
			{
				return layer;
			}
		};
	}
}