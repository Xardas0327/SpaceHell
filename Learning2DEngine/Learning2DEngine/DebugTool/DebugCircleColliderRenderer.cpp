#include "DebugCircleColliderRenderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DebugMacro.h"

#include "../System/Game.h"
#include "../System/ResourceManager.h"
#include "../System/ComponentManager.h"
#include "../Render/RenderManager.h"
#include "../Render/ShaderConstant.h"

namespace Learning2DEngine
{
	using namespace System;
	using namespace Render;
	using namespace Physics;

	namespace DebugTool
	{
		DebugCircleColliderRenderer::DebugCircleColliderRenderer()
			: DebugColliderRenderer()
		{

		}

		void DebugCircleColliderRenderer::InitVao()
		{
			float vertices[DebugCircleColliderRenderer::CircleSegment * 2];

			for (int i = 0; i < DebugCircleColliderRenderer::CircleSegment; ++i) {
				float theta = 2.0f * 3.14159265359f * i / DebugCircleColliderRenderer::CircleSegment;
				vertices[i * 2] = glm::cos(theta);
				vertices[i * 2 + 1] = glm::sin(theta);
			}

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

			glGenBuffers(1, &vboDynamicObject);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(BaseColorDynamicData), NULL, GL_DYNAMIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)offsetof(BaseColorDynamicData, modelMatrix));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 4));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 8));
			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)(offsetof(BaseColorDynamicData, modelMatrix) + sizeof(float) * 12));
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE,
				sizeof(BaseColorDynamicData),
				(void*)offsetof(BaseColorDynamicData, color));

			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);
			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			maxObjectSize = 1;
			dynamicData = new BaseColorDynamicData[maxObjectSize];
		}

		void DebugCircleColliderRenderer::Draw(RendererMode rendererMode, int layer)
		{
			if (debugRenderData.find(rendererMode) == debugRenderData.end())
				return;
			if (debugRenderData[rendererMode].find(layer) == debugRenderData[rendererMode].end())
				return;

			shader->Use();
			shader->SetMatrix4("projection", Game::mainCamera.GetProjection());
			shader->SetMatrix4("view", Game::mainCamera.GetViewMatrix());

			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vboDynamicObject);

			for (int i = 0; i < debugRenderData[rendererMode][layer].size(); ++i)
			{
				auto colliderData = debugRenderData[rendererMode][layer][i];

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model,
					glm::vec3(colliderData->objectComponent->GetColliderCenter(), 0.0f)
				);
				model = glm::scale(model, glm::vec3(colliderData->objectComponent->colliderRadius, colliderData->objectComponent->colliderRadius, 1.0f));

				std::memcpy(dynamicData[i].modelMatrix,
					glm::value_ptr(model),
					sizeof(dynamicData[i].modelMatrix));

				if (colliderData->objectComponent->mode == ColliderMode::TRIGGER)
					std::memcpy(dynamicData[i].color,
						glm::value_ptr(L2DE_DEBUG_SHOW_COLLIDER_TRIGGER_COLOR),
						sizeof(dynamicData[i].color));
				else
					std::memcpy(dynamicData[i].color,
						glm::value_ptr(L2DE_DEBUG_SHOW_COLLIDER_COLOR),
						sizeof(dynamicData[i].color));
			}

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(BaseColorDynamicData) * debugRenderData[rendererMode][layer].size(), dynamicData);
			glDrawArraysInstanced(
				GL_LINE_LOOP,
				0,
				DebugCircleColliderRenderer::CircleSegment,
				static_cast<GLsizei>(debugRenderData[rendererMode][layer].size())
			);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}
}