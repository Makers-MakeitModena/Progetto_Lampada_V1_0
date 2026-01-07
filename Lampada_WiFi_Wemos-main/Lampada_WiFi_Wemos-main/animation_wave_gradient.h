/*
  ============================================================================
  ANIMAZIONE: ONDA GRADUALE
  ============================================================================
  Come l'onda normale ma il colore cambia gradualmente ad ogni frame
  invece che a scatti dopo ogni ciclo completo.
  Crea un effetto molto fluido e ipnotico.
  ============================================================================
*/

#ifndef ANIMATION_WAVE_GRADIENT_H
#define ANIMATION_WAVE_GRADIENT_H
#include "config.h"


// Variabili di stato per l'animazione
static int waveGradBrightnessEven = 10;
static int waveGradBrightnessOdd = 255;
static bool waveGradIncreasing = true;
static byte waveGradColorIndex = 0;


void initWaveGradientAnimation() {
  waveGradBrightnessEven = 10;
  waveGradBrightnessOdd = 255;
  waveGradIncreasing = true;
  waveGradColorIndex = 0;
}

void waveGradientAnimation() {
  if (millis() - lastAnimationUpdate >= WAVE_GRAD_UPDATE_DELAY) {
    
    uint32_t currentColor = Wheel(waveGradColorIndex);
    
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2 == 0) {
        uint32_t dimmedColor = dimColor(currentColor, waveGradBrightnessEven);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        uint32_t dimmedColor = dimColor(currentColor, waveGradBrightnessOdd);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Aggiorna le luminosità
    if (waveGradIncreasing) {
      waveGradBrightnessEven += WAVE_GRAD_BRIGHTNESS_STEP;
      waveGradBrightnessOdd -= WAVE_GRAD_BRIGHTNESS_STEP;
      
      if (waveGradBrightnessEven >= 255) {
        waveGradBrightnessEven = 255;
        waveGradBrightnessOdd = 10;
        waveGradIncreasing = false;
      }
    } else {
      waveGradBrightnessEven -= WAVE_GRAD_BRIGHTNESS_STEP;
      waveGradBrightnessOdd += WAVE_GRAD_BRIGHTNESS_STEP;
      
      if (waveGradBrightnessEven <= 10) {
        waveGradBrightnessEven = 10;
        waveGradBrightnessOdd = 255;
        waveGradIncreasing = true;
      }
    }
    
    // Cambia colore gradualmente ad ogni frame
    waveGradColorIndex += WAVE_GRAD_COLOR_STEP;
    if (waveGradColorIndex >= 256) {
      waveGradColorIndex = 0;
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
