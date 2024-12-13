#include "NeonDisplay.h"

void NeonDisplay::Init() {
    FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.setBrightness(255);
    ShowTest();
    Off();
}

void NeonDisplay::ShowTest() {
    for (int j=0; j<10; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
        int index = i+j;
        if (index >= 300)
            index -= 300;
        leds[index] =  ColorFromPalette(RainbowColors_p, i * 255/50);
        }
        FastLED.show();
        delay(100);
    }
}

void NeonDisplay::Show(char digits[5]) {
    int ledIndex = 0;
    for (int digitIndex = 4; digitIndex >= 0; digitIndex--) {
        // Serial.println("digitIndex = " + String(digitIndex));
        if (digitIndex == 2) {
            if (digits[2] == ':') {
                for (int x = 0; x < 10; x++) {
                    if (x == 2 || x == 3 || x == 6 || x == 7) {
                        leds[ledIndex] = CRGB::Green;
                    } else {
                        leds[ledIndex] = CRGB::Black;
                    }
                    ledIndex++;
                }
            }
            else if (digits[2] >= '0' && digits[2] <= '9') {
                int numLedsToShow = digits[2] - '0'; // Convert char to int
                for (int x = 0; x < numLedsToShow; x++) {
                    leds[ledIndex] = CRGB::Green; // Show Green LEDs
                    ledIndex++;
                }
                for (int x = numLedsToShow; x < 10; x++) {
                    leds[ledIndex] = CRGB::Black; // Turn off remaining LEDs
                    ledIndex++;
                }
            }
            else {
                for (int x = 0; x < 10; x++) {
                    leds[ledIndex] = CRGB::Black;
                    ledIndex++;
                }
            }
        }
        else {
            for (int segmentIndex = 0; segmentIndex < 7; segmentIndex++) {
                // Serial.println("segmentIndex = " + String(segmentIndex));
                for (int x = 0; x < groupSizes[segmentIndex]; x++)
                {
                    // Serial.println("digitIndex = " + String(digitIndex) + " groupSizes[segmentIndex] = " + String(groupSizes[segmentIndex]) + " ledIndex = " + String(ledIndex));
                    // Serial.println("Segments[digits[digitIndex]][segmentIndex] = " + String(Segments[digits[digitIndex]][segmentIndex]));
                    if (Segments[digits[digitIndex]][segmentIndex] == 1) {
                        leds[ledIndex] = CRGB::Green;
                    } else {
                        leds[ledIndex] = CRGB::Black;
                    }
                    ledIndex++;
                }
            }
        }
    }
    FastLED.show();
}

void NeonDisplay::Off() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
}