/*
  ============================================================================
  ANIMAZIONE: BIANCO FISSO
  ============================================================================
  Mantiene tutti i LED accesi con luce bianca costante
  ============================================================================
*/

#ifndef ANIMATION_WHITE_H
#define ANIMATION_WHITE_H

#include "config.h"

// Nessuna variabile di stato necessaria per questa animazione

void initWhiteAnimation() {
  // Nessuna inizializzazione necessaria
}

void whiteAnimation() {
  // Imposta tutti i LED al colore bianco personalizzato
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(WHITE_R, WHITE_G, WHITE_B));
  }
  
  pixels.show();
}

#endif
