#include "sound.hpp"
#include <bass.h>
#include <iostream>
bool Sound::isInit = false;
Sound::Sound(const char *path, bool loop)
{
    if (!isInit)
    {
        if (!BASS_Init(-1, 44100, 0, 0, NULL))
        {
            std::cout << "Can't initialize device" << std::endl;
        }
        else
        {
            isInit = true;
        }
    }
    // HSAMPLE sample = BASS_SampleLoad(FALSE, "assets/sounds/rocket.wav", 0, 0, 3, BASS_SAMPLE_OVER_POS);
    // if (!sample)
    // {
    //     std::cout << "Can't load sample" << std::endl;
    // }
    // else
    // {
    //     HCHANNEL channel = BASS_SampleGetChannel(sample, FALSE);
    //     BASS_ChannelPlay(channel, FALSE);
    // }
    channel = BASS_StreamCreateFile(false, path, 0, 0, loop ? BASS_SAMPLE_LOOP : BASS_SAMPLE_FLOAT);
    if (!channel)
    {
        std::cout << "Can't load sample" << std::endl;
    }
}
void Sound::changeVolume(int volume)
{

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
    BASS_Free();
}
