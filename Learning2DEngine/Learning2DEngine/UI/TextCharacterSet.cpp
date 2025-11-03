#include "TextCharacterSet.h"

#include "../DebugTool/Log.h"

namespace Learning2DEngine
{
    namespace UI
    {
        TextCharacterSet::TextCharacterSet()
            : characters()
        {

        }

        CharacterMap& TextCharacterSet::operator[](const FontSizePair& fontSizePair)
        {
            Load(fontSizePair);
            return characters.at(fontSizePair);
        }

        const CharacterMap& TextCharacterSet::operator[](const FontSizePair& fontSizePair) const
        {
            return characters.at(fontSizePair);
        }

        void TextCharacterSet::Load(const std::string& font, unsigned int fontSize)
        {
            Load(FontSizePair(font, fontSize));
        }

        void TextCharacterSet::Load(const FontSizePair& fontSizePair)
        {
            if (characters.count(fontSizePair))
                return;

            FT_Library ft;
            if (FT_Init_FreeType(&ft))
            {
                L2DE_LOG_ERROR("FREETYPE: Could not init FreeType Library");
                return;
            }

            FT_Face face;

            if (FT_New_Face(ft, fontSizePair.first.c_str(), 0, &face))
            {
                L2DE_LOG_ERROR("FREETYPE: Failed to load font: " + fontSizePair.first);
                return;
            }

            FT_Set_Pixel_Sizes(face, 0, fontSizePair.second);
            characters.insert(std::pair<FontSizePair, CharacterMap>(fontSizePair, CharacterMap()));

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            for (GLubyte ch = 0; ch < TextCharacterSet::FontNumber; ++ch)
            {
                if (FT_Load_Char(face, ch, FT_LOAD_RENDER))
                {
                    L2DE_LOG_WARNING("FREETYTPE: Failed to load Glyph: " + ch);
                    continue;
                }

                unsigned int textureId;
                glGenTextures(1, &textureId);
                glBindTexture(GL_TEXTURE_2D, textureId);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                FreeTypeCharacter character = {
                    textureId,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    face->glyph->advance.x
                };
                characters[fontSizePair].insert(std::pair<char, FreeTypeCharacter>(ch, character));
            }
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
            glBindTexture(GL_TEXTURE_2D, 0);

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        void TextCharacterSet::Unload(const std::string& font, unsigned int fontSize)
        {
            Unload(FontSizePair(font, fontSize));
        }

        void TextCharacterSet::Unload(const FontSizePair& fontSizePair)
        {
            if (!characters.count(fontSizePair))
                return;

            const CharacterMap& characterMap = characters[fontSizePair];
            unsigned int* textureIds = new unsigned int[characterMap.size()];
            unsigned int index = 0;
            for (auto i = characterMap.begin(); i != characterMap.end(); ++i)
            {
                textureIds[index] = i->second.textureId;
                ++index;
            }

            glDeleteTextures(index, textureIds);
            delete[] textureIds;

            characters.erase(fontSizePair);
        }

        void TextCharacterSet::Clear()
        {
            for (auto i = characters.begin(); i != characters.end(); ++i)
            {
                unsigned int* textureIds = new unsigned int[i->second.size()];
                unsigned int index = 0;
                for (auto characterPair = i->second.begin(); characterPair != i->second.end(); ++characterPair)
                {
                    textureIds[index] = characterPair->second.textureId;
                    ++index;
                }

                glDeleteTextures(index, textureIds);
                delete[] textureIds;
            }
            characters.clear();
        }
    }
}