#pragma once

class Sound
{
private:
  // channel is a variable to store the sound channel
  unsigned int channel;
  // isInit is a static variable to indicate whether the sound system is initialized or not
  // It is used to initialize the sound system only once
  static bool isInit;

public:
  // The constructor takes:
  // path: the path to the sound file
  // loop: boolean value to indicate whether the sound should be repeated or not
  Sound(const char *path, bool loop);
  ~Sound();
  // restart: value to indicate whether the sound should be restarted or not
  // 0: don't restart
  // 1: restart
  void play(int restart = 0);
  // stop the sound
  void stop();
  // Volume is an integer value ranges from 0 to 100
  void changeVolume(int volume);
  // pause the sound
  void pause();
};
