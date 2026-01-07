/*
  ============================================================================
  ANIMAZIONE: ONDA INVERSA CON ARCOBALENO
  ============================================================================
  Onda invertita con colori arcobaleno rotanti:
  - LED PARI (0,2,4,6,8,10...): vanno da 255 a 10 e tornano a 255
  - LED DISPARI (1,3,5,7,9,11...): vanno da 10 a 255 e tornano a 10
  - I colori arcobaleno ruotano continuamente
  ============================================================================
*/

#ifndef ANIMATION_WAVE_INVERSE_H
#define ANIMATION_WAVE_INVERSE_H
#include "config.h"


// Variabili di stato per l'animazione
static int waveInvBrightnessEven = 255;
static int waveInvBrightnessOdd = 10;
static bool waveInvIncreasing = false;
static uint16_t waveInvRainbowJ = 0;


void initWaveInverseAnimation() {
  waveInvBrightnessEven = 255;
  waveInvBrightnessOdd = 10;
  waveInvIncreasing = false;
  waveInvRainbowJ = 0;
}

void waveInverseAnimation() {
  if (millis() - lastAnimationUpdate >= WAVE_INV_UPDATE_DELAY) {
    
    // Per ogni LED dell'anello
    for (int i = 0; i < NUM_LEDS; i++) {
      // Calcola il colore arcobaleno per questo LED
      uint32_t rainbowColor = Wheel(((i * 256 / NUM_LEDS) + waveInvRainbowJ) & 255);
      
      if (i % 2 == 0) {
        // LED PARI (iniziano alti)
        uint32_t dimmedColor = dimColor(rainbowColor, waveInvBrightnessEven);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        // LED DISPARI (iniziano bassi)
        uint32_t dimmedColor = dimColor(rainbowColor, waveInvBrightnessOdd);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Aggiorna la posizione dell'arcobaleno (rotazione)
    waveInvRainbowJ++;
    if (waveInvRainbowJ >= 256 * 5) {
      waveInvRainbowJ = 0;
    }
    
    // Aggiorna le luminosità (invertito rispetto all'onda normale)
    if (waveInvIncreasing) {
      waveInvBrightnessEven += WAVE_INV_BRIGHTNESS_STEP;
      waveInvBrightnessOdd -= WAVE_INV_BRIGHTNESS_STEP;
      
      if (waveInvBrightnessEven >= 255) {
        waveInvBrightnessEven = 255;
        waveInvBrightnessOdd = 10;
        waveInvIncreasing = false;
      }
    } else {
      waveInvBrightnessEven -= WAVE_INV_BRIGHTNESS_STEP;
      waveInvBrightnessOdd += WAVE_INV_BRIGHTNESS_STEP;
      
      if (waveInvBrightnessEven <= 10) {
        waveInvBrightnessEven = 10;
        waveInvBrightnessOdd = 255;
        waveInvIncreasing = true;
      }
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
