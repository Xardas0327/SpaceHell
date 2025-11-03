#pragma once

#include <string>

namespace Learning2DEngine
{
	namespace Render
	{
		class ShaderConstant final
		{
		private:
			ShaderConstant() {}
		public:
			//Simple Sprite

			static const std::string SIMPLE_SPRITE_SHADER_NAME;
			static const char* GetSimpleSpriteVertexShader();
			static const char* GetSimpleSpriteFragmentShader();

			//Sprite (Multi instancing support)

			static const std::string SPRITE_SHADER_NAME;
			static const char* GetSpriteVertexShader();
			static const char* GetSpriteFragmentShader();

			//Simple Text2D

			static const std::string SIMPLE_TEXT2D_SHADER_NAME;
			static const char* GetSimpleText2DVertexShader();
			static const char* GetSimpleText2DFragmentShader();

			//Text2D (Multi instancing support)

			static const std::string TEXT2D_SHADER_NAME;
			static const char* GetText2DVertexShader();
			static const char* GetText2DFragmentShader();

			//PostProcessEffect

			static const std::string DEFAULT_POSTPROCESS_EFFECT_NAME;
			static const char* GetDefaultPostprocessVertexShader();
			static const char* GetDefaultPostprocessFragmentShader();

			//Base Color

			static const std::string BASE_COLOR_NAME;
			static const char* GetBaseColorVertexShader();
			static const char* GetBaseColorFragmentShader();
		};
	}
}