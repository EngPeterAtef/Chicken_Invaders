#pragma once

class Sound
{
  private:
    unsigned int channel;
    static bool isInit;

  public:
    Sound(const char *path, bool loop);
    ~Sound();
    void play(int restart = 0);
    void stop();
    // Volume is an integer value ranges from 0 to 100
    void changeVolume(int volume);
    void pause();
};
