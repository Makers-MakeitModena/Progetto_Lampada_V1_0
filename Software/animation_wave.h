/*
  ============================================================================
  ANIMAZIONE: ONDA DI LUCI
  ============================================================================
  LED pari e dispari si alternano in luminosità creando un effetto onda.
  Il colore cambia ad ogni ciclo completo attraversando l'arcobaleno.
  
  - LED PARI (0,2,4,6,8,10): vanno da 10 a 255 e tornano a 10
  - LED DISPARI (1,3,5,7,9,11): vanno da 255 a 10 e tornano a 255
  ============================================================================
*/

#ifndef ANIMATION_WAVE_H
#define ANIMATION_WAVE_H
#include "config.h"


// Variabili di stato per l'animazione
static int waveBrightnessEven = 10;
static int waveBrightnessOdd = 255;
static bool waveIncreasing = true;
static byte waveColorIndex = 0;

void initWaveAnimation() {
  waveBrightnessEven = 10;
  waveBrightnessOdd = 255;
  waveIncreasing = true;
  waveColorIndex = 0;
}

void waveAnimation() {
  if (millis() - lastAnimationUpdate >= WAVE_UPDATE_DELAY) {
    
    // Ottieni il colore corrente dall'arcobaleno
    uint32_t currentColor = Wheel(waveColorIndex);
    
    // Applica la luminosità e il colore a tutti i LED
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2 == 0) {
        // LED PARI
        uint32_t dimmedColor = dimColor(currentColor, waveBrightnessEven);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        // LED DISPARI
        uint32_t dimmedColor = dimColor(currentColor, waveBrightnessOdd);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Aggiorna le luminosità
    if (waveIncreasing) {
      waveBrightnessEven += WAVE_BRIGHTNESS_STEP;
      waveBrightnessOdd -= WAVE_BRIGHTNESS_STEP;
      
      if (waveBrightnessEven >= 255) {
        waveBrightnessEven = 255;
        waveBrightnessOdd = 10;
        waveIncreasing = false;
      }
    } else {
      waveBrightnessEven -= WAVE_BRIGHTNESS_STEP;
      waveBrightnessOdd += WAVE_BRIGHTNESS_STEP;
      
      if (waveBrightnessEven <= 10) {
        waveBrightnessEven = 10;
        waveBrightnessOdd = 255;
        waveIncreasing = true;
        
        // Ciclo completato! Cambia colore
        waveColorIndex += 36;
        if (waveColorIndex >= 256) {
          waveColorIndex = 0;
        }
      }
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
