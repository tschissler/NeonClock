#include "AudioModule.h"

AudioModule::AudioModule() : audioTaskHandle(NULL) {
    audio.setPinout(MAX98357A_I2S_BCLK, MAX98357A_I2S_LRC, MAX98357A_I2S_DOUT);
}

void AudioModule::playAudio(String fileName, int volume)
{
    audio.setVolume(volume);
    audio.connecttoSD(fileName.c_str());
    while (audio.isRunning())
    {
        audio.loop();
    }
}

void AudioModule::playAudioAsync(String fileName, int volume)
{
    // Create a structure to hold the parameters
    AudioTaskParams *params = new AudioTaskParams();
    params->volume = volume;
    params->fileName = fileName; // Set the desired file name
    params->audioModule = this;

    // Create a task to run audio.loop() on core 1
    xTaskCreatePinnedToCore(
        audioLoopTask,    // Task function
        "Audio Loop",     // Name of the task
        20000,            // Stack size (in words)
        params,           // Task input parameter
        1,                // Priority of the task
        &audioTaskHandle, // Task handle
        1                 // Core to run the task on (0 or 1)
    );
}

void AudioModule::audioLoopTask(void *parameter)
{
    AudioTaskParams *params = (AudioTaskParams *)parameter;
    AudioModule *audioModule = params->audioModule;

    audioModule->audio.setVolume(params->volume);
    audioModule->audio.connecttoSD(params->fileName.c_str());

    while (audioModule->audio.isRunning())
    {
        audioModule->audio.loop();
    }
    vTaskDelete(NULL); // Delete the task when done
}