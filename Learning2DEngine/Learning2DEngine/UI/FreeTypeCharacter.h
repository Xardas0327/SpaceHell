#pragma once

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace Learning2DEngine
{
    namespace UI
    {
        // Please check the documentation of FreeType for bearing and advance.
        struct FreeTypeCharacter
        {
            unsigned int textureId;
            glm::ivec2   size;
            glm::ivec2   bearing;
            FT_Pos advance;
        };
    }
}