/*
  ============================================================================
  ANIMAZIONE: RAINBOW WAVE (ARCOBALENO CON ONDA)
  ============================================================================
  Combina l'effetto arcobaleno rotante con l'effetto onda alternato:
  - I colori ruotano come nell'animazione rainbow
  - La luminosità alterna tra LED pari e dispari creando un'onda
  
  Risultato: Un arcobaleno dinamico che "pulsa" con l'effetto onda
  ============================================================================
*/

#ifndef ANIMATION_RAINBOW_WAVE_H
#define ANIMATION_RAINBOW_WAVE_H

#include "config.h"

// Variabili di stato per l'animazione
static uint16_t rainbowWaveJ = 0;
static int rainbowWaveBrightnessEven = 10;
static int rainbowWaveBrightnessOdd = 255;
static bool rainbowWaveIncreasing = true;

void initRainbowWaveAnimation() {
  rainbowWaveJ = 0;
  rainbowWaveBrightnessEven = 10;
  rainbowWaveBrightnessOdd = 255;
  rainbowWaveIncreasing = true;
}

void rainbowWaveAnimation() {
  if (millis() - lastAnimationUpdate >= RAINBOW_WAVE_UPDATE_DELAY) {
    
    // Per ogni LED dell'anello
    for (int i = 0; i < NUM_LEDS; i++) {
      // Calcola il colore arcobaleno per questo LED
      uint32_t rainbowColor = Wheel(((i * 256 / NUM_LEDS) + rainbowWaveJ) & 255);
      
      // Applica la luminosità alternata (effetto onda)
      if (i % 2 == 0) {
        // LED PARI: usa luminosità pari
        uint32_t dimmedColor = dimColor(rainbowColor, rainbowWaveBrightnessEven);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        // LED DISPARI: usa luminosità dispari
        uint32_t dimmedColor = dimColor(rainbowColor, rainbowWaveBrightnessOdd);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Aggiorna la posizione dell'arcobaleno (rotazione)
    rainbowWaveJ++;
    if (rainbowWaveJ >= 256 * 5) {
      rainbowWaveJ = 0;
    }
    
    // Aggiorna le luminosità (effetto onda)
    if (rainbowWaveIncreasing) {
      rainbowWaveBrightnessEven += RAINBOW_WAVE_BRIGHTNESS_STEP;
      rainbowWaveBrightnessOdd -= RAINBOW_WAVE_BRIGHTNESS_STEP;
      
      if (rainbowWaveBrightnessEven >= 255) {
        rainbowWaveBrightnessEven = 255;
        rainbowWaveBrightnessOdd = 10;
        rainbowWaveIncreasing = false;
      }
    } else {
      rainbowWaveBrightnessEven -= RAINBOW_WAVE_BRIGHTNESS_STEP;
      rainbowWaveBrightnessOdd += RAINBOW_WAVE_BRIGHTNESS_STEP;
      
      if (rainbowWaveBrightnessEven <= 10) {
        rainbowWaveBrightnessEven = 10;
        rainbowWaveBrightnessOdd = 255;
        rainbowWaveIncreasing = true;
      }
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
