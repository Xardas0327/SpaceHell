#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Learning2DEngine
{
	namespace System
	{
		class Transform
		{
		private:
			glm::vec2 position;
			glm::vec2 scale;
			float rotation;
			bool isModified;
			glm::mat4 modelMatrix;

			glm::mat4 CalculateModelMatrix() const
			{
				glm::mat4 model = glm::mat4(1.0f);
				// first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
				model = glm::translate(model, glm::vec3(position, 0.0f));
				// move origin of rotation to center of quad
				model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
				// then rotate
				model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
				// move origin back
				model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
				// then rotate
				model = glm::scale(model, glm::vec3(scale, 1.0f)); // last scale

				return model;
			}
		public:
			Transform(glm::vec2 position = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f, 1.0f), float rotation = 0.0f)
				: position(position), scale(scale), rotation(rotation), isModified(true), modelMatrix()
			{

			}

			inline glm::vec2 GetPosition() const
			{
				return position;
			}

			void SetPosition(const glm::vec2& newPosition)
			{
				position = newPosition;
				isModified = true;
			}

			void AddPosition(const glm::vec2& deltaPosition)
			{
				position += deltaPosition;
				isModified = true;
			}

			inline glm::vec2 GetScale() const
			{
				return scale;
			}

			void SetScale(const glm::vec2& newScale)
			{
				scale = newScale;
				isModified = true;
			}

			void AddScale(const glm::vec2& deltaScale)
			{
				scale += deltaScale;
				isModified = true;
			}

			inline float GetRotation() const
			{
				return rotation;
			}

			void SetRotation(float newRotation)
			{
				rotation = newRotation;
				isModified = true;
			}

			void AddRotation(float deltaRotation)
			{
				rotation += deltaRotation;
				isModified = true;
			}

			const glm::mat4& GetModelMatrix()
			{
				if (isModified)
				{
					modelMatrix = CalculateModelMatrix();
					isModified = false;
				}

				return modelMatrix;
			}

			glm::mat4 GetModelMatrix() const
			{
				if (isModified)
					return CalculateModelMatrix();

				return modelMatrix;
			}
		};
	}
}

