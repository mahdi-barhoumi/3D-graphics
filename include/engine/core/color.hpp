#pragma once
#include <string>
#include <algorithm>
#include <glm/glm.hpp>

namespace Engine
{
    struct Color
    {
        unsigned char r = 0, g = 0, b = 0, a = 255;

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
                r = (value >> 16) & 0xFF;
                g = (value >> 8) & 0xFF;
                b = value & 0xFF;
                a = 255;
            } 
            else if (hex.size() == 8)
            {
                r = (value >> 24) & 0xFF;
                g = (value >> 16) & 0xFF;
                b = (value >> 8) & 0xFF;
                a = value & 0xFF;
            }
        }
        constexpr Color(float r, float g, float b, float a = 1.0f) 
            : r(static_cast<unsigned char>(std::clamp(r, 0.0f, 1.0f) * 255.0f))
            , g(static_cast<unsigned char>(std::clamp(g, 0.0f, 1.0f) * 255.0f))
            , b(static_cast<unsigned char>(std::clamp(b, 0.0f, 1.0f) * 255.0f))
            , a(static_cast<unsigned char>(std::clamp(a, 0.0f, 1.0f) * 255.0f)) {}
        constexpr Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255) : r(r), g(g), b(b), a(a) {}
        
        operator glm::vec3() const { return glm::vec3(r / 255.0f, g / 255.0f, b / 255.0f); }
        operator glm::vec4() const { return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); }

        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Black;
        static const Color White;
        static const Color Transparent;
    };

    inline const Color Color::Red           {1.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::Green         {0.0f, 1.0f, 0.0f, 1.0f};
    inline const Color Color::Blue          {0.0f, 0.0f, 1.0f, 1.0f};
    inline const Color Color::Black         {0.0f, 0.0f, 0.0f, 1.0f};
    inline const Color Color::White         {1.0f, 1.0f, 1.0f, 1.0f};
    inline const Color Color::Transparent   {0.0f, 0.0f, 0.0f, 0.0f};
}