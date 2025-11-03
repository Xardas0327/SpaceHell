#include "CollisionHelper.h"

namespace Learning2DEngine
{
    namespace Physics
    {
        glm::vec2 CollisionHelper::GetEdge(const BaseBoxColliderComponent& boxCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 boxHalfExtents(boxCollider.colliderSize.x / 2.0f, boxCollider.colliderSize.y / 2.0f);
            glm::vec2 clamped = glm::clamp(distanceBetweenCenters, -boxHalfExtents, boxHalfExtents);

            return boxCollider.GetColliderCenter() + clamped;
        }

        glm::vec2 CollisionHelper::GetEdge(const BaseCircleColliderComponent& circleCollider, glm::vec2 distanceBetweenCenters)
        {
            glm::vec2 edge = glm::normalize(distanceBetweenCenters) * circleCollider.colliderRadius;

            return circleCollider.GetColliderCenter() + edge;
        }

        CollisionHelper::HitDirection CollisionHelper::GetDirection(glm::vec2 vector)
        {
            static glm::vec2 directions[] = {
                glm::vec2(0.0f, 1.0f),	// up
                glm::vec2(1.0f, 0.0f),	// right
                glm::vec2(0.0f, -1.0f),	// down
                glm::vec2(-1.0f, 0.0f)	// left
            };

            float max = 0.0f;
            unsigned int bestId = 0;
            for (unsigned int i = 0; i < 4; i++)
            {
                float dotProduct = glm::dot(glm::normalize(vector), directions[i]);

                //if dotProduct > max means, that the vector is closer to the direction (their angel is smaller)
                if (dotProduct > max)
                {
                    max = dotProduct;
                    bestId = i;
                }
            }
            return (HitDirection)bestId;
        }

        void CollisionHelper::FixPosition(const BaseBoxColliderComponent& boxCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier)
        {
            glm::vec2 difference = edgeOfCollidedObject - boxCollider.GetColliderCenter();
            HitDirection direction = CollisionHelper::GetDirection(difference);
            if (direction == HitDirection::LEFT || direction == HitDirection::RIGHT)
            {
                float penetration = (boxCollider.colliderSize.x / 2.0f - std::abs(difference.x)) * fixMultiplier;
                if (direction == HitDirection::LEFT)
                {
                    boxCollider.gameObject->transform.AddPosition(glm::vec2(penetration, 0.0f));
                    if (boxCollider.GetRigidbody() != nullptr && boxCollider.GetRigidbody()->velocity.x < 0.0f)
                        boxCollider.GetRigidbody()->velocity.x = 0.0f;
                }
                else
                {
                    boxCollider.gameObject->transform.AddPosition(glm::vec2(-penetration, 0.0f));
                    if (boxCollider.GetRigidbody() != nullptr && boxCollider.GetRigidbody()->velocity.x > 0.0f)
                        boxCollider.GetRigidbody()->velocity.x = 0.0f;
                }
            }
            else
            {
                float penetration = (boxCollider.colliderSize.y / 2.0f - std::abs(difference.y)) * fixMultiplier;
                if (direction == HitDirection::UP)
                {
                    boxCollider.gameObject->transform.AddPosition(glm::vec2(0.0f, -penetration));
                    if (boxCollider.GetRigidbody() != nullptr && boxCollider.GetRigidbody()->velocity.y > 0.0f)
                        boxCollider.GetRigidbody()->velocity.y = 0.0f;
                }
                else
                {
                    boxCollider.gameObject->transform.AddPosition(glm::vec2(0.0f, penetration));
                    if (boxCollider.GetRigidbody() != nullptr && boxCollider.GetRigidbody()->velocity.y < 0.0f)
                        boxCollider.GetRigidbody()->velocity.y = 0.0f;
                }
            }
        }

        void CollisionHelper::FixPosition(const BaseCircleColliderComponent& circleCollider, glm::vec2 edgeOfCollidedObject, float fixMultiplier)
        {
            glm::vec2 difference = edgeOfCollidedObject - circleCollider.GetColliderCenter();
            HitDirection direction = CollisionHelper::GetDirection(difference);
            if (direction == HitDirection::LEFT || direction == HitDirection::RIGHT)
            {
                float penetration = (circleCollider.colliderRadius - std::abs(difference.x)) * fixMultiplier;
                if (direction == HitDirection::LEFT)
                {
                    circleCollider.gameObject->transform.AddPosition(glm::vec2(penetration, 0.0f));
                    if (circleCollider.GetRigidbody() != nullptr && circleCollider.GetRigidbody()->velocity.x < 0.0f)
                        circleCollider.GetRigidbody()->velocity.x = 0.0f;
                }
                else
                {
                    circleCollider.gameObject->transform.AddPosition(glm::vec2(-penetration, 0.0f));
                    if (circleCollider.GetRigidbody() != nullptr && circleCollider.GetRigidbody()->velocity.x > 0.0f)
                        circleCollider.GetRigidbody()->velocity.x = 0.0f;
                }
            }
            else
            {
                float penetration = (circleCollider.colliderRadius - std::abs(difference.y)) * fixMultiplier;
                if (direction == HitDirection::UP)
                {
                    circleCollider.gameObject->transform.AddPosition(glm::vec2(0.0f, -penetration));
                    if (circleCollider.GetRigidbody() != nullptr && circleCollider.GetRigidbody()->velocity.y > 0.0f)
                        circleCollider.GetRigidbody()->velocity.y = 0.0f;
                }
                else
                {
                    circleCollider.gameObject->transform.AddPosition(glm::vec2(0.0f, penetration));
                    if (circleCollider.GetRigidbody() != nullptr && circleCollider.GetRigidbody()->velocity.y < 0.0f)
                        circleCollider.GetRigidbody()->velocity.y = 0.0f;
                }
            }
        }

        CollisionData CollisionHelper::CheckCollision(const BaseBoxColliderComponent& collider1, const BaseBoxColliderComponent& collider2)
        {
            glm::vec2 box1Center = collider1.GetColliderCenter();
            glm::vec2 halfSize1 = collider1.colliderSize / 2.0f;
            glm::vec2 box2Center = collider2.GetColliderCenter();
            glm::vec2 halfSize2 = collider2.colliderSize / 2.0f;

            CollisionData data;
            data.isCollided =
                // x-axis
                box1Center.x + halfSize1.x >= box2Center.x - halfSize2.x
                && box2Center.x + halfSize2.x >= box1Center.x - halfSize1.x
                // Y-axis
                && box1Center.y + halfSize1.y >= box2Center.y - halfSize2.y
                && box2Center.y + halfSize2.y >= box1Center.y - halfSize1.y;

            if (data.isCollided)
            {
                data.edge1 = CollisionHelper::GetEdge(collider1, box2Center - box1Center);
                data.edge2 = CollisionHelper::GetEdge(collider2, box1Center - box2Center);
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseCircleColliderComponent& collider1, const BaseCircleColliderComponent& collider2)
        {
            glm::vec2 distance = collider2.GetColliderCenter() - collider1.GetColliderCenter();
            float radius = collider1.colliderRadius + collider2.colliderRadius;
            CollisionData data;
            data.isCollided = CollisionHelper::GetLength2(distance) <= (radius * radius);

            if (data.isCollided)
            {
                data.edge1 = GetEdge(collider1, distance);
                data.edge2 = GetEdge(collider2, -distance);
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseCircleColliderComponent& circleCollider, const BaseBoxColliderComponent& boxCollider)
        {
            glm::vec2 circleCenter = circleCollider.GetColliderCenter();
            glm::vec2 boxCenter = boxCollider.GetColliderCenter();

            glm::vec2 boxEdge = GetEdge(boxCollider, circleCenter - boxCenter);

            glm::vec2 distance = boxEdge - circleCenter;

            CollisionData data;
            data.isCollided = CollisionHelper::GetLength2(distance) <= (circleCollider.colliderRadius * circleCollider.colliderRadius);

            if (data.isCollided)
            {
                data.edge1 = GetEdge(circleCollider, boxCenter - circleCenter);
                data.edge2 = boxEdge;
            }

            return data;
        }

        CollisionData CollisionHelper::CheckCollision(const BaseBoxColliderComponent& boxCollider, const BaseCircleColliderComponent& circleCollider)
        {
            CollisionData data = CollisionHelper::CheckCollision(circleCollider, boxCollider);

            if (data.isCollided)
            {
                glm::vec2 temp = data.edge1;
                data.edge1 = data.edge2;
                data.edge2 = temp;
            }

            return data;
        }
    }
}