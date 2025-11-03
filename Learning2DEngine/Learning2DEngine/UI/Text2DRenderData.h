#pragma once

#include <string>
#include <map>
#include <vector>

#include <glm/glm.hpp>

#include "../Render/RenderData.h"
#include "FontSizePair.h"

namespace Learning2DEngine
{
    namespace UI
    {
        class Text2DRenderData : public Render::RenderData
        {
        protected:
            FontSizePair fontSizePair;
            std::string text;
            bool shouldRecalcSize;
            bool shouldRecalcVertices;
            glm::mat4 previousModelMatrix;
            std::map<char, std::vector<glm::mat4x2>> characterVertices;
            glm::vec2 textSize;
            float lineSpacing;

            glm::vec2 CalculateTextSize() const;
            std::map<char, std::vector<glm::mat4x2>> CalculateCharacterVertices() const;
            glm::mat2 GetRotationMatrix() const;
        public:
            glm::vec4 color;
            bool isUseCameraView;

            Text2DRenderData(const System::Component* component, const FontSizePair& fontSizePair, glm::vec4 color = glm::vec4(1.0f));

            Text2DRenderData(
                const System::Component* component,
                const FontSizePair& fontSizePair,
                const std::string& text,
                glm::vec4 color = glm::vec4(1.0f));

            Text2DRenderData(
                const System::Component* component,
                const FontSizePair& fontSizePair,
                const std::string& text,
                bool isUseCameraView,
                glm::vec4 color = glm::vec4(1.0f));

            inline const std::string& GetText() const
            {
                return text;
            }

            void SetText(const std::string& text);
            void SetText(std::string&& text);

            inline const FontSizePair& GetFontSizePair() const
            {
                return fontSizePair;
            }

            void SetFontSizePair(const FontSizePair& fontSizePair);

            inline float GetLineSpacing() const
            {
                return lineSpacing;
            }

            void SetLineSpacing(float lineSpacing);

            const std::map<char, std::vector<glm::mat4x2>>& GetCharacterVertices();
            std::map<char, std::vector<glm::mat4x2>> GetCharacterVertices() const;

            glm::vec2 GetTextSize();
            glm::vec2 GetTextSize() const;
        };
    }
}