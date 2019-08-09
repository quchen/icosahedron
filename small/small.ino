#include <FastLED.h>

#define LED_PIN 4
#define CLOCK_PIN 5
#define COLOR_ORDER BGR

#define NUM_LEDS 144

CRGB leds[NUM_LEDS];
CRGB leds2[NUM_LEDS];

void setup() {
    Serial.begin(115200);
    while(!Serial) { delay(10); }
    Serial.println(); // Separate from the initial rubbish sent at low baud rate
    Serial.println("Serial communication set up");

    FastLED.addLeds<DOTSTAR, LED_PIN, CLOCK_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(32);
    pinMode(LED_BUILTIN, OUTPUT);
}

int pos = 0;
int hue = 0;

void calibrate() {
    leds[0] = CRGB::Red;
    leds[1] = CRGB::Green;
    leds[2] = CRGB::Blue;
}

void runningRainbow() {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    ++pos;
    pos %= NUM_LEDS;
    hue += 2;
    leds[pos] = CHSV(hue, 200, 255);
}

void confetti() {
    hue += 1;
    leds[random8(NUM_LEDS)] = CHSV(hue, 200, 255);
    fadeToBlackBy(leds, NUM_LEDS, 20);
}

void lhc() {
    fade_raw(leds, NUM_LEDS, 20);
    leds[pos] += CRGB::Red;
    leds[NUM_LEDS-pos] += CRGB::Blue;
    ++pos;
    pos %= NUM_LEDS;
}

int p(int pos) {
    return (pos + NUM_LEDS) % NUM_LEDS;
}

void diffusion() {
    // for(int i = 0; i < NUM_LEDS; ++i) {
    //     leds2[p(i)] = (leds2[p(i-1)] + leds2[p(i+1)]) / 2;
    // }
    // for(int i = 0; i < NUM_LEDS; ++i) {
    //     leds[i] = leds2[i];
    // }
    blur1d(leds, NUM_LEDS, 250);
    if(random8(255) >= 240) {
        leds[p(random8(NUM_LEDS))] |= CHSV(random8(0, 255), 255, 255);
    }
}

void pair_annihilation() {
    int center = NUM_LEDS/2;

    int brightness = 0;
    while(brightness < 255) {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        ++brightness;
        leds[center] = CHSV(0, 0, brightness);
        FastLED.show();
        delay(10);
    }
    for(int i = 0; i < 70; ++i) {
        fadeToBlackBy(leds, NUM_LEDS, 20);
        leds[center+i] = CRGB::Red;
        leds[center-i] = CRGB::Blue;
        FastLED.show();
        delay(10);
    }
}

#define NUM_PARTICLES 10
int particle_pos[NUM_PARTICLES] = {74, 74, 74, 74, 74, 74, 74, 74, 74, 74};
void randomWalk() {
    fadeToBlackBy(leds, NUM_LEDS, 10);
    for(int i = 0; i < NUM_PARTICLES; ++i) {
        particle_pos[i] += random8(0,2) > 0 ? -1 : 1;
        particle_pos[i] = p(particle_pos[i]);
        leds[particle_pos[i]] |= CHSV(i*24, 255, 255);
    }
}

void loop() {
    randomWalk();
    FastLED.show();
    delay(20);
}
