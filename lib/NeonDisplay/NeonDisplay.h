#ifndef NEONDISPLAY_H
#define NEONDISPLAY_H

#include <map>
#include <array>
#include <FastLED.h>

#define DATA_PIN 16
#define NUM_LEDS 158

class NeonDisplay {
public:
    void Init();
    void Show(char digits[5]);
    void ShowTest();
    void Off();
    
private:
    CRGB leds[NUM_LEDS];
    int DigitLEDs[37];
    int groupSizes[7] = {5, 5, 6, 5, 5, 6, 5};
    std::map<char, std::array<int, 7>> Segments = {
        {'0', {1, 1, 1, 1, 1, 1, 0}},
        {'1', {1, 1, 0, 0, 0, 0, 0}},
        {'2', {0, 1, 1, 0, 1, 1, 1}},
        {'3', {1, 1, 1, 0, 0, 1, 1}},
        {'4', {1, 1, 0, 1, 0, 0, 1}},
        {'5', {1, 0, 1, 1, 0, 1, 1}},
        {'6', {1, 0, 1, 1, 1, 1, 1}},
        {'7', {1, 1, 1, 0, 0, 0, 0}},
        {'8', {1, 1, 1, 1, 1, 1, 1}},
        {'9', {1, 1, 1, 1, 0, 1, 1}},
        {' ', {0, 0, 0, 0, 0, 0, 0}}
    };  
};

#endif // NEONDISPLAY_H