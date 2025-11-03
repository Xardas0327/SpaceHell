#pragma once

#include <glm/glm.hpp>

#include "BaseBoxColliderComponent.h"
#include "BaseCircleColliderComponent.h"
#include "CollisionData.h"
#include "ColliderType.h"
#include "ColliderMode.h"


namespace Learning2DEngine
{
    namespace Physics
    {
        class CollisionHelper final
        {
            enum HitDirection { UP, RIGHT, DOWN, LEFT };
        private:
            CollisionHelper() {}

            static glm::vec2 GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters);
            static glm::vec2 GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters);

            static HitDirection GetDirection(glm::vec2 vector);

            //The fixMultiplier should be 1.0f if another object is Kinematic or 0.5f if it is Dynamic.
            static void FixPosition(const BaseBoxColliderComponent& boxCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier);
            //The fixMultiplier should be 1.0f if another object is Kinematic or 0.5f if it is Dynamic.
            static void FixPosition(const BaseCircleColliderComponent& circleCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier);

            //It doesn't use sqrt.
            static inline float GetLength2(glm::vec2 distance)
            {
                return distance.x * distance.x + distance.y * distance.y;
            }
        public:
            static CollisionData CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2);
            static CollisionData CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2);
            static CollisionData CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider);
            static CollisionData CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider);


            //Fix the position of the game objects, if they are dynamic with collider mode and another collider has collider mode too.
            //If both colliders are dynamic with collider mode, they will moved only half of the distance.
            //When the function fix the position, it can reset the velocity of the rigidbody if it is inited.
            template<class T, class U>
            static void FixPosition(T& first, glm::vec2 edge1, U& second, glm::vec2 edge2)
            {
                if (first.mode != ColliderMode::COLLIDER || second.mode != ColliderMode::COLLIDER)
                    return;

                if (first.type == ColliderType::DYNAMIC)
                {
                    FixPosition(first, edge2, second.type == ColliderType::DYNAMIC ? 0.5f : 1.0f);
                }
                if (second.type == ColliderType::DYNAMIC)
                {
                    FixPosition(second, edge1, first.type == ColliderType::DYNAMIC ? 0.5f : 1.0f);
                }
            }
        };
    }
}