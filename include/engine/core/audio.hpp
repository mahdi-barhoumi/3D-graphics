#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <al/al.hpp>
#include <al/alc.hpp>
#include <engine/core/component.hpp>

namespace Engine
{
    class Audio : public Component
    {
        private:

        struct Allocation final
        {
            ALuint buffer;
            ALuint source;
            ALenum format;

            Allocation(unsigned int channels, unsigned int rate, std::vector<short>& pcm);
            ~Allocation();
        };

        std::shared_ptr<Allocation> mp_Allocation = nullptr;

        static ALCdevice* s_Device;
        static ALCdevice* Initiliaze();
        static std::unordered_map<std::string, std::weak_ptr<Allocation>> s_Cache;

        inline ALuint GetBackendBuffer() const { return mp_Allocation->buffer; }
        inline ALuint GetBackendSource() const { return mp_Allocation->source; }
        inline ALenum GetBackendFormat() const { return mp_Allocation->format; }

        public:
        
        Audio() = default;
        Audio(const Audio& other) = default;
        Audio(const std::string& path);
        ~Audio() = default;

        void Play();
        bool IsPlaying();

        static size_t GetAllocationCount();

    };
}