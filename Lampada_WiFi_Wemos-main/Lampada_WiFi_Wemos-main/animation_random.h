/*
  ============================================================================
  ANIMAZIONE: COLORI CASUALI
  ============================================================================
  Accende i LED uno alla volta con colori casuali
  ============================================================================
*/

#ifndef ANIMATION_RANDOM_H
#define ANIMATION_RANDOM_H

#include "config.h"

// Variabili di stato per l'animazione
static int randomAnimationStep = 0;

void initRandomAnimation() {
  randomAnimationStep = 0;
}

void randomAnimation() {
  if (millis() - lastAnimationUpdate >= RANDOM_DELAY) {
    
    // Accende il LED corrente con un colore casuale
    pixels.setPixelColor(
      randomAnimationStep,
      pixels.Color(random(256), random(256), random(256))
    );
    
    pixels.show();
    
    // Passa al LED successivo
    randomAnimationStep++;
    
    // Se ha completato il giro dell'anello, ricomincia
    if (randomAnimationStep >= NUM_LEDS) {
      randomAnimationStep = 0;
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
