#pragma once

namespace Learning2DEngine
{
    namespace System
    {
        class Game;

        class Time
        {
            friend class Game;
        private:
            static constexpr const float DefaultTimeScale = 1.0f;

            /// <summary>
            /// It is multiplied by timeScale.
            /// Before the first frame, it is 0.0f.
            /// </summary>
            static float deltaTime;
            static float timeScale;
            static float lastTime;
            static bool isInited;

            Time() = default;
            ~Time() = default;

            static void CalcDeltaTime();

        public:
            static inline void ResetTimeScale()
            {
                timeScale = Time::DefaultTimeScale;
            }

            static inline void SetTimeScale(float value)
            {
                timeScale = value;
            }

            static inline float GetTimeScale()
            {
                return timeScale;
            }

            static inline float GetDeltaTime()
            {
                return deltaTime;
            }
        };
    }
}