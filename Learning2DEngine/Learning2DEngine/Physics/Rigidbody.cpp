#include "Rigidbody.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 Rigidbody::gravity = Rigidbody::DefaultGravity;

        Rigidbody::Rigidbody(System::GameObject* gameObject, glm::vec2 velocity, bool isGravityEnabled, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::Component(gameObject),
            BaseRigidbody(velocity), isFrozen(isFrozen), isGravityEnabled(isGravityEnabled), gravityMultiplier(1.0f)
        {

        }

        Rigidbody::Rigidbody(System::GameObject* gameObject, bool isFrozen)
            : System::UpdaterComponent(gameObject), System::Component(gameObject),
            BaseRigidbody(glm::vec2(0.0f, 0.0f)), isFrozen(isFrozen), isGravityEnabled(false), gravityMultiplier(1.0f)
        {

        }

        void Rigidbody::Update()
        {
            if (!isFrozen)
            {
                if (isGravityEnabled)
                {
                    velocity += gravity * gravityMultiplier * System::Time::GetDeltaTime();
                }

                gameObject->transform.AddPosition(
                    velocity * System::Time::GetDeltaTime()
                );
            }
        }
    }
}