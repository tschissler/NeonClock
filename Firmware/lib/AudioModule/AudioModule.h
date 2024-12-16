#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <Arduino.h>
#include <Audio.h>
#include <map>

#define MAX98357A_I2S_DOUT 25
#define MAX98357A_I2S_BCLK 26
#define MAX98357A_I2S_LRC 27

class AudioModule {
public:
    AudioModule();
    void playAudio(String fileName, int volume);
    void playAudioAsync(String fileName, int volume);

private:
    static void audioLoopTask(void *parameter);
    struct AudioTaskParams {
        int volume;
        String fileName;
        AudioModule *audioModule;
    };

    Audio audio;
    TaskHandle_t audioTaskHandle;
};

#endif // AUDIOMODULE_H