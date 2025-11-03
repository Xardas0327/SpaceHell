#pragma once

#include <vector>

#include "AnimationFrame.h"
#include "../Render/UVTexture2DContainer.h"
#include "../System/LateUpdaterComponent.h"
#include "../System/GameObject.h"

namespace Learning2DEngine
{
	namespace Animator
	{
		class AnimationController : public Learning2DEngine::System::LateUpdaterComponent
		{
			friend class Learning2DEngine::System::GameObject;
		protected:
			Render::UVTexture2DContainer* textureContainer;
			std::vector<AnimationFrame> frames;
			size_t currentIndex;
			float currentTime;
			bool isPlaying;

			AnimationController(System::GameObject* gameObject, Render::UVTexture2DContainer* textureContainer, bool isLoop = false);
			AnimationController(System::GameObject* gameObject, Render::UVTexture2DContainer* textureContainer, size_t minFrameSize, bool isLoop = false);

			void LateUpdate() override;

			inline void SetFrameByIndex()
			{
				textureContainer->SetTexture(*frames[currentIndex].texture);
				textureContainer->uvMatrix = frames[currentIndex].uvMatrix;
			}

		public:
			float speed;
			bool isLoop;

			void Play(bool reset = false);
			void Stop();
			void Add(const AnimationFrame& frame);
			void Add(AnimationFrame&& frame);
			void Remove(size_t index);
			void Clear();
			AnimationFrame& operator[](size_t index);
			const AnimationFrame& operator[](size_t index) const;

			//If the time is less then 0.0f, it will use the index frame time.
			void JumpToFrame(size_t index, float time = -1.0f);

			inline const AnimationFrame& GetCurrentFrame() const
			{
				return frames[currentIndex];
			}

			inline size_t GetCurrentIndex() const
			{
				return currentIndex;
			}

			inline float GetCurrentTime() const
			{
				return currentTime;
			}

			inline size_t GetFrameCount() const
			{
				return frames.size();
			}

		};
	}
}