#include "AnimationController.h"

#include <cmath>

#include "../System/Time.h"

using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

namespace Learning2DEngine
{
	namespace Animator
	{
		AnimationController::AnimationController(GameObject* gameObject, UVTexture2DContainer* textureContainer, bool isLoop)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			textureContainer(textureContainer), frames(), currentIndex(0), currentTime(0.0f), isPlaying(false),
			speed(1.0f), isLoop(isLoop)
		{
			// there are less than 2 frames, it is useles
			frames.reserve(2);
		}

		AnimationController::AnimationController(GameObject* gameObject, UVTexture2DContainer* textureContainer, size_t minFrameSize, bool isLoop)
			: LateUpdaterComponent(gameObject), Component(gameObject),
			textureContainer(textureContainer), frames(), currentIndex(0), currentTime(0.0f), isPlaying(false),
			speed(1.0f), isLoop(isLoop)
		{
			frames.reserve(minFrameSize);
		}

		void AnimationController::LateUpdate()
		{
			if (!isPlaying)
				return;

			bool isForward = speed >= 0.0f;
			float currentSpeed = fabs(speed);

			currentTime -= currentSpeed * Time::GetDeltaTime();

			if (currentTime > 0.0f)
				return;

			while (currentTime < 0.0f)
			{
				if (isForward)
				{
					if (currentIndex == frames.size() - 1)
					{
						if (isLoop)
							currentIndex = 0;
						else
						{
							Stop();
							return;
						}
					}
					else
						++currentIndex;
				}
				else
				{
					if (currentIndex == 0)
					{
						if (isLoop)
							currentIndex = frames.size() - 1;
						else
						{
							Stop();
							return;
						}
					}
					else
						--currentIndex;
				}

				currentTime+= frames[currentIndex].time;
			}
			SetFrameByIndex();
		}

		void AnimationController::Play(bool reset)
		{
			if(reset)
				currentIndex = 0;

			currentTime = frames[currentIndex].time;
			SetFrameByIndex();
			isPlaying = true;
		}

		void AnimationController::Stop()
		{
			isPlaying = false;
		}

		void AnimationController::Add(const AnimationFrame& frame)
		{
			frames.push_back(frame);
		}

		void AnimationController::Add(AnimationFrame&& frame)
		{
			frames.push_back(std::move(frame));
		}

		void AnimationController::Remove(size_t index)
		{
			frames.erase(frames.begin() + index);
		}

		void AnimationController::Clear()
		{
			frames.clear();
		}

		AnimationFrame& AnimationController::operator[](size_t index)
		{
			return frames[index];
		}

		const AnimationFrame& AnimationController::operator[](size_t index) const
		{
			return frames[index];
		}

		void AnimationController::JumpToFrame(size_t index, float time)
		{
			currentIndex = index;
			currentTime = time < 0.0f ? frames[currentIndex].time : time;
			SetFrameByIndex();
		}
	}
}