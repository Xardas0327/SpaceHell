#pragma once

#include <glm/glm.hpp>

#include "../System/GameObject.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        struct Collision
        {
            // The closest point of current object to collided object
            glm::vec2 edge;
            // The closest point of collided object to current object
            glm::vec2 edgeOfCollidedObject;
            System::GameObject* collidedObject;

            Collision(
                glm::vec2 edge,
                glm::vec2 edgeOfCollidedObject,
                System::GameObject* collidedObject)
                : edge(edge), edgeOfCollidedObject(edgeOfCollidedObject),
                collidedObject(collidedObject)
            {

            }
        };
    }
}