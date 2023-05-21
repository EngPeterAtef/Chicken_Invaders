#include "sound.hpp"
#include <bass.h>
#include <iostream>
bool Sound::isInit = false;
Sound::Sound(const char *path, bool loop)
{
    // If the sound system is not initialized, initialize it only once
    if (!isInit)
    {
        // -1 means default device
        // 44100 is the sample rate
        // 0 is the flags
        // 0 is the window handle
        // NULL is the callback
        if (!BASS_Init(-1, 44100, 0, 0, NULL))
        {
            std::cout << "Can't initialize device" << std::endl;
        }
        else
        {
            isInit = true;
        }
    }
    // Create a stream from the file
    // false means the file is in storage (not in memory)
    // path is the path to the file
    // 0 is the offset
    // 0 is the length (0 means the whole file)
    // loop is a flag to indicate whether the sound should be repeated or not
    channel = BASS_StreamCreateFile(false, path, 0, 0, loop ? BASS_SAMPLE_LOOP : BASS_SAMPLE_FLOAT);
    if (!channel)
    {
        std::cout << "Can't load sample" << std::endl;
    }
}
void Sound::changeVolume(int volume)
{
    // volume ranges from 0 to 100
    BASS_SetConfig(BASS_CONFIG_GVOL_STREAM, volume * 100);
}
void Sound::play(int restart)
{
    BASS_ChannelPlay(channel, restart);
}
void Sound::pause()
{
    BASS_ChannelPause(channel);
}
void Sound::stop()
{
    BASS_ChannelStop(channel);
}
Sound::~Sound()
{
    // Free the sound system
    BASS_Free();
}
