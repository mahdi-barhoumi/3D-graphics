#pragma once
#include <string>
#include <algorithm>
#include <glm/glm.hpp>

namespace Engine
{
    struct Color
    {
        float r = 0.0f, g = 0.0f, b = 0.0f, a = 1.0f;

        constexpr Color() = default;
        constexpr Color(std::string_view hex)
        {
            if (!hex.empty() && hex[0] == '#') hex.remove_prefix(1);

            size_t value = 0;
            
            for (size_t i = 0; i < hex.size() && i < 8; ++i)
            {
                size_t digit = 0;
                char character = hex[i];
                
                if ('0' <= character && character <= '9') digit = character - '0';
                else if ('a' <= character && character <= 'f') digit = character - 'a' + 10;
                else if ('A' <= character && character <= 'F') digit = character - 'A' + 10;
                else break;
                
                value = (value << 4) | digit;
            }
            
            if (hex.size() == 6)
            {
                r = ((value >> 16) & 0xFF) / 255.0f;
                g = ((value >> 8) & 0xFF) / 255.0f;
                b = (value & 0xFF) / 255.0f;
                a = 1.0f;
            } 
            else if (hex.size() == 8)
            {
                r = ((value >> 24) & 0xFF) / 255.0f;
                g = ((value >> 16) & 0xFF) / 255.0f;
                b = ((value >> 8) & 0xFF) / 255.0f;
                a = (value & 0xFF) / 255.0f;
            }
        }
        constexpr Color(float r, float g, float b, float a = 1.0f) : r(std::clamp(r, 0.0f, 1.0f)), g(std::clamp(g, 0.0f, 1.0f)), b(std::clamp(b, 0.0f, 1.0f)), a(std::clamp(a, 0.0f, 1.0f)) {}
        constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}
        
        operator glm::vec3() const { return glm::vec3(r, g, b); }
        operator glm::vec4() const { return glm::vec4(r, g, b, a); }

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Black;
        static const Color White;
        static const Color Transparent;
    };

    inline const Color Color::Red{1.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::Green{0.0f, 1.0f, 0.0f, 1.0f};
    inline const Color Color::Blue{0.0f, 0.0f, 1.0f, 1.0f};
    inline const Color Color::Black{0.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::White{1.0f, 1.0f, 1.0f, 1.0f};
    inline const Color Color::Transparent{0.0f, 0.0f, 0.0f, 0.0f};
}