#include <stdexcept>
#include <engine/core/audio.hpp>
#include <engine/core/utilities.hpp>

namespace Engine
{
    std::unordered_map<std::string, std::weak_ptr<Audio::Allocation>> Audio::s_Cache;
    ALCdevice* Audio::s_Device = Audio::Initiliaze();
    ALCdevice* Audio::Initiliaze()
    {
        ALCdevice* device = alcOpenDevice(nullptr);
        if (!device) throw std::runtime_error("Could not create sound device.");
        ALCcontext* context = alcCreateContext(device, nullptr);
        alcMakeContextCurrent(context);
        return device;
    }
    Audio::Allocation::Allocation(unsigned int channels, unsigned int rate, std::vector<short>& pcm)
    {
        if (channels == 1) format = AL_FORMAT_MONO16;
        else format = AL_FORMAT_STEREO16;

        alGenBuffers(1, &buffer);
        if (!buffer) throw std::runtime_error("Could not generate buffer for audio.");
        alGenSources(1, &source);
        if (!source) throw std::runtime_error("Could not generate source for audio.");

        alBufferData(buffer, format, pcm.data(), pcm.size() * sizeof(short), rate);
        alSourcei(source, AL_BUFFER, buffer);
    }
    Audio::Allocation::~Allocation()
    {
        alDeleteSources(1, &source);
        alDeleteBuffers(1, &buffer);
    }
    Audio::Audio(const std::string& path)
    {
        auto iterator = s_Cache.find(path);
        if (iterator != s_Cache.end() && (mp_Allocation = iterator->second.lock())) return;

        unsigned int channels, rate;
        std::vector<short> pcm;
        if (!Utilities::LoadOGGFile(path, channels, rate, pcm)) throw std::runtime_error("Could not load audio file.");
        mp_Allocation = std::make_shared<Allocation>(channels, rate, pcm);
        s_Cache[path] = mp_Allocation;
    }
    void Audio::Play() { alSourcePlay(GetBackendSource()); }
    bool Audio::IsPlaying()
    {
        ALint state;
        alGetSourcei(GetBackendSource(), AL_SOURCE_STATE, &state);
        return state == AL_PLAYING;
    }
    size_t Audio::GetAllocationCount()
    { 
        std::erase_if(s_Cache, [](const auto& item) { return item.second.expired(); });
        return s_Cache.size();
    }
}