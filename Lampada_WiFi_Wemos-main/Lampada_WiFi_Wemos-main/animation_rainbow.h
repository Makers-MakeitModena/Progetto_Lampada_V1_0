/*
  ============================================================================
  ANIMAZIONE: RAINBOW (ARCOBALENO)
  ============================================================================
  Crea un effetto arcobaleno che ruota continuamente attorno all'anello
  ============================================================================
*/

#ifndef ANIMATION_RAINBOW_H
#define ANIMATION_RAINBOW_H

#include "config.h"

// Variabili di stato per l'animazione
static uint16_t rainbowJ = 0;

void initRainbowAnimation() {
  rainbowJ = 0;
}

void rainbowAnimation() {
  // Aggiorna ogni 20ms
  if (millis() - lastAnimationUpdate >= RAINBOW_DELAY) {
    
    // Per ogni LED dell'anello
    for (int i = 0; i < NUM_LEDS; i++) {
      // Calcola il colore in base alla posizione del LED e allo step corrente
      pixels.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + rainbowJ) & 255));
    }
    
    pixels.show();
    
    // Incrementa il contatore per far ruotare l'arcobaleno
    rainbowJ++;
    
    // Reset del contatore quando raggiunge 256*5
    if (rainbowJ >= 256 * 5) {
      rainbowJ = 0;
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
