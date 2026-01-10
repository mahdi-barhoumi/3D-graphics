#include <engine/core/audio.hpp>

namespace Engine
{
    // // 1. Initialize OpenAL
    // ALCdevice* device = alcOpenDevice(NULL); // Open default device
    // ALCcontext* context = alcCreateContext(device, NULL);
    // alcMakeContextCurrent(context);

    // // 2. Create a simple Sine Wave (Mono, 16-bit, 44.1kHz)
    // const int freq = 44100;
    // const int seconds = 2;
    // const int count = freq * seconds;
    // std::vector<short> data(count);
    // for (int i = 0; i < count; ++i) {
    //     data[i] = 32760 * sin(2 * 3.14 * 440 * i / freq); // 440Hz Tone
    // }

    // // 3. Buffer the data
    // ALuint buffer;
    // alGenBuffers(1, &buffer);
    // alBufferData(buffer, AL_FORMAT_MONO16, data.data(), data.size() * sizeof(short), freq);

    // // 4. Create Source and Play
    // ALuint source;
    // alGenSources(1, &source);
    // alSourcei(source, AL_BUFFER, buffer);
    // alSourcePlay(source);

    // std::cout << "Playing 440Hz tone for 2 seconds..." << std::endl;

    // // Wait for playback to finish
    // ALint state;
    // do {
    //     alGetSourcei(source, AL_SOURCE_STATE, &state);
    // } while (state == AL_PLAYING);

    // // 5. Cleanup
    // alDeleteSources(1, &source);
    // alDeleteBuffers(1, &buffer);
    // alcMakeContextCurrent(NULL);
    // alcDestroyContext(context);
    // alcCloseDevice(device);
}