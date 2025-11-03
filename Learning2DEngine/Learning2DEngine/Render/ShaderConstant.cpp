#include "ShaderConstant.h"

#include <string>

#include "RenderManager.h"

namespace Learning2DEngine
{
	namespace Render
	{
		//Simple Sprite

		const std::string ShaderConstant::SIMPLE_SPRITE_SHADER_NAME = "L2DE_SimpleSprite";

		const char* ShaderConstant::GetSimpleSpriteVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			uniform mat4 model;
			uniform mat4 view;
			uniform mat4 projection;
			uniform bool useView;

			void main()
			{
				if (useView)
				{
					gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
				}
				else
				{
					gl_Position = projection * model * vec4(position, 0.0, 1.0);
				}
				TextureCoords = textureCoords;
			})";

			return shader;
		}

		const char* ShaderConstant::GetSimpleSpriteFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform bool isUseTexture;
			uniform sampler2D spriteTexture;
			uniform vec4 spriteColor;

			void main()
			{
				color = spriteColor;
				if (isUseTexture)
				{
					color *= texture(spriteTexture, TextureCoords);
				}
			})";

			return shader;
		}

		//Sprite (Multi instancing support)

		const std::string ShaderConstant::SPRITE_SHADER_NAME = "L2DE_Sprite";

		const char* ShaderConstant::GetSpriteVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 uv0;
			layout(location = 2) in vec2 uv1;
			layout(location = 3) in vec2 uv2;
			layout(location = 4) in vec2 uv3;
			layout(location = 5) in mat4 model;
			layout(location = 9) in vec4 spriteColor;
			layout(location = 10) in float spriteTextureId;
			layout(location = 11) in float useView;

			out vec2 TextureCoords;
			out vec4 SpriteColor;
			out float SpriteTextureId;

			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				if(bool(useView))
				{
					gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
				}
				else
				{
					gl_Position = projection * model * vec4(position, 0.0, 1.0);
				}
				
				vec2 textureCoords[4] = vec2[4](uv0, uv1, uv2, uv3);
				TextureCoords = textureCoords[gl_VertexID % 4];
				SpriteColor = spriteColor;
				SpriteTextureId = spriteTextureId;
			})";

			return shader;
		}

		const char* ShaderConstant::GetSpriteFragmentShader()
		{
			GLint maxUnitNumber = RenderManager::GetInstance().GetMaxTextureUnits();

			static std::string shader = R"(
			#version 330 core
			in vec2 TextureCoords;
			in vec4 SpriteColor;
			in float SpriteTextureId;

			out vec4 color;

			uniform sampler2D[)" + std::to_string(maxUnitNumber) + R"(] spriteTextures;

			void main()
			{
				color = SpriteColor;

				int textureId = int(SpriteTextureId);	
				if (textureId>=0 && textureId <)" + std::to_string(maxUnitNumber) + R"() 
				{
					color *= texture(spriteTextures[textureId], TextureCoords);
				}
			})";

			return shader.c_str();
		}

		//Simple Text2D

		const std::string ShaderConstant::SIMPLE_TEXT2D_SHADER_NAME = "L2DE_SimpleText2D";

		const char* ShaderConstant::GetSimpleText2DVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			uniform mat4 view;
			uniform mat4 projection;
			uniform bool useView;

			void main()
			{
				if (useView)
				{
					gl_Position = projection * view * vec4(position, 0.0, 1.0);
				}
				else
				{
					gl_Position = projection * vec4(position, 0.0, 1.0);
				}
				TextureCoords = textureCoords;
			})";

			return shader;
		}

		const char* ShaderConstant::GetSimpleText2DFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D characterTexture;
			uniform vec4 characterColor;

			void main()
			{
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTexture, TextureCoords).r);
				color = characterColor * sampled;
			})";

			return shader;
		}

		//Text2D (Multi instancing support)

		const std::string ShaderConstant::TEXT2D_SHADER_NAME = "L2DE_Text2D";

		const char* ShaderConstant::GetText2DVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 textureCoords;
			layout(location = 1) in vec2 position0;
			layout(location = 2) in vec2 position1;
			layout(location = 3) in vec2 position2;
			layout(location = 4) in vec2 position3;
			layout(location = 5) in vec4 characterColor;
			layout(location = 6) in float characterTextureId;
			layout(location = 7) in float useView;

			out vec2 TextureCoords;
			out vec4 CharacterColor;
			out float CharacterTextureId;

			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				vec2 positions[4] = vec2[4](position0, position1, position2, position3);
				if(bool(useView))
				{
					gl_Position = projection * view * vec4(positions[gl_VertexID % 4], 0.0, 1.0);
				}
				else
				{
					gl_Position = projection * vec4(positions[gl_VertexID % 4], 0.0, 1.0);
				}
				TextureCoords = textureCoords;
				CharacterColor = characterColor;
				CharacterTextureId = characterTextureId;
			})";

			return shader;
		}

		const char* ShaderConstant::GetText2DFragmentShader()
		{
			GLint maxUnitNumber = RenderManager::GetInstance().GetMaxTextureUnits();

			static std::string shader = R"(
			#version 330 core
			in vec2 TextureCoords;
			in vec4 CharacterColor;
			in float CharacterTextureId;

			out vec4 color;

			uniform sampler2D[)" + std::to_string(maxUnitNumber) + R"(] characterTextures;

			void main()
			{
				int textureId = int(CharacterTextureId);
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(characterTextures[textureId], TextureCoords).r);
				color = CharacterColor * sampled;
			})";

			return shader.c_str();
		}

		//PostProcessEffect

		const std::string ShaderConstant::DEFAULT_POSTPROCESS_EFFECT_NAME = "L2DE_DefaultPostProcessEffect";

		const char* ShaderConstant::GetDefaultPostprocessVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in vec2 textureCoords;

			out vec2 TextureCoords;

			void main()
			{
				gl_Position = vec4(position, 0.0, 1.0);
				TextureCoords = textureCoords;
			})";

			return shader;
		}

		const char* ShaderConstant::GetDefaultPostprocessFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec2 TextureCoords;

			out vec4 color;

			uniform sampler2D scene;

			void main()
			{
				color = texture(scene, TextureCoords);
			})";

			return shader;
		}

		//Base Color

		const std::string ShaderConstant::BASE_COLOR_NAME = "L2DE_BaseColor";

		const char* ShaderConstant::GetBaseColorVertexShader()
		{
			static const char* shader = R"(
			#version 330 core
			layout(location = 0) in vec2 position;
			layout(location = 1) in mat4 model;
			layout(location = 5) in vec4 inColor;

			out vec4 color;

			uniform mat4 view;
			uniform mat4 projection;

			void main()
			{
				gl_Position = projection * view * model * vec4(position, 0.0, 1.0);
				color = inColor;
			})";

			return shader;
		}

		const char* ShaderConstant::GetBaseColorFragmentShader()
		{
			static const char* shader = R"(
			#version 330 core
			in vec4 color;

			out vec4 FragColor;

			void main()
			{
				FragColor = color;
			})";

			return shader;
		}
	}
}