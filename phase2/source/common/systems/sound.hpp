class Sound
{
private:
    unsigned int channel;
    static bool isInit;

public:
    Sound(const char *path, bool loop);
    ~Sound();
    void play();
    void stop();
    void pause();
};
