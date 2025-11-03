#pragma once

#include <mutex>

#include "BaseRendererComponent.h"
#include "RendererMode.h"
#include "../System/ComponentManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		/// <summary>
		/// The classes, which are inherited from RendererComponent,
		/// have to have a constructor, which first parameter is GameObject* for gameObject member.
		/// The TRenderData should be a class, which is inhereted from RenderData.
		/// The TRenderer should be a class, which is inhereted from IRenderer.
		/// Please check for more info about `System::Component` and `BaseRendererComponent`.
		/// </summary>
		template<class TRenderData, class TRenderer>
		class RendererComponent : public BaseRendererComponent<TRenderData, TRenderer>
		{
		protected:
			/// <summary>
			/// It is counted, that how many RendererComponent<TRenderData, TRenderer> exist.
			/// </summary>
			static int refrenceNumber;
			static std::mutex mutex;

			template <class ...TRenderDataParams>
			RendererComponent(System::GameObject* gameObject, RendererMode mode, int layer = 0, TRenderDataParams&&... renderDataParams)
				: BaseRendererComponent<TRenderData, TRenderer>(gameObject, layer, std::forward<TRenderDataParams>(renderDataParams)...),
				System::Component(gameObject), mode(mode)
			{

			}

			void InitObject()
			{
				auto& componentManager = System::ComponentManager::GetInstance();
				if (!componentManager.IsRendererExist(this->GetId()))
				{
					componentManager.AddRenderer(this->GetId(), this->GetInitedRenderer());
				}

				componentManager.AddRenderData(mode, this->GetId(), &this->data, this->GetLayer());
				++RendererComponent::refrenceNumber;
			}

			void DestroyObject()
			{
				auto& componentManager = System::ComponentManager::GetInstance();
				componentManager.RemoveRenderData(&this->data);

				if (!(--RendererComponent::refrenceNumber))
				{
					this->DestroyRenderer();
					componentManager.RemoveRenderer(this->GetId());
				}
			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Init() in the first line.
			/// </summary>
			virtual void Init() override
			{
				if (System::ComponentManager::GetInstance().GetThreadSafe())
				{
					std::lock_guard<std::mutex> lock(mutex);
					InitObject();
				}
				else
				{
					InitObject();
				}
			}

			/// <summary>
			/// If this function is override, it must call the RendererComponent::Destroy() in the first line.
			/// </summary>
			virtual void Destroy() override
			{
				if (System::ComponentManager::GetInstance().GetThreadSafe())
				{
					std::lock_guard<std::mutex> lock(mutex);
					DestroyObject();
				}
				else
				{
					DestroyObject();
				}
			}

		public:
			const RendererMode mode;

			virtual void SetLayer(int value) override
			{
				BaseRendererComponent<TRenderData, TRenderer>::SetLayer(value);
				System::ComponentManager::GetInstance().ChangeRenderLayer(&this->data, this->GetLayer());
			}
		};

		template<class TRenderData, class TRenderer>
		int RendererComponent<TRenderData, TRenderer>::refrenceNumber = 0;

		template<class TRenderData, class TRenderer>
		std::mutex RendererComponent<TRenderData, TRenderer>::mutex;
	}
}