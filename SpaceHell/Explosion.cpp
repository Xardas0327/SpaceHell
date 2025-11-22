#include "Explosion.h"

#include <Learning2DEngine/Animator/AnimationController.h>
#include <Learning2DEngine/System/GameObjectManager.h>
#include <Learning2DEngine/System/ResourceManager.h>
#include <Learning2DEngine/System/Time.h>
#include <Learning2DEngine/Render/SpriteRenderComponent.h>


using namespace Learning2DEngine::Animator;
using namespace Learning2DEngine::System;
using namespace Learning2DEngine::Render;

Explosion::Explosion(GameObject* gameObject)
	: UpdaterComponent(gameObject), Component(gameObject), timer(0.0f)
{

}

void Explosion::Init()
{
	UpdaterComponent::Init();

	auto renderer = gameObject->AddComponent<SpriteRenderComponent>(
		RendererMode::RENDER,
		ResourceManager::GetInstance().GetTexture(EXPLOSION_TEXTURE_IDS[0]),
		-1
	);

    auto animation = gameObject->AddComponent<AnimationController>(&renderer->data, EXPLOSION_ANIMATION_NUMBER, false);
    for (int i = 0; i < EXPLOSION_ANIMATION_NUMBER; ++i)
    {
        auto uvMatrix = glm::mat4x2{
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };
        animation->Add(AnimationFrame{
            &ResourceManager::GetInstance().GetTexture(EXPLOSION_TEXTURE_IDS[i]),
            uvMatrix,
            EXPLOSION_ANIMATION_FRAME_TIME
            });
    }
    animation->Play();

}

void Explosion::Update()
{
    timer += Time::GetDeltaTime();
    if (timer >= EXPLOSION_ANIMATION_NUMBER * EXPLOSION_ANIMATION_FRAME_TIME)
    {
        GameObjectManager::GetInstance().DestroyGameObject(gameObject);
    }
}

Explosion* Explosion::Create(const glm::vec2& position)
{
	auto object = GameObjectManager::GetInstance().CreateGameObject(Transform(position, EXPLOSION_SIZE));

	return object->AddComponent<Explosion>();
}