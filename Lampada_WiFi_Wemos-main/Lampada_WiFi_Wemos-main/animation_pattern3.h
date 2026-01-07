/*
  ============================================================================
  ANIMAZIONE: PATTERN 3-4 LED CON ARCOBALENO
  ============================================================================
  I LED sono divisi in 4 gruppi:
  - Per anello da 12 LED: 4 gruppi di 3 LED
  - Per anello da 16 LED: 4 gruppi di 4 LED
  
  I gruppi si accendono in sequenza con colori arcobaleno che ruotano.
  Ogni gruppo ha un colore diverso dell'arcobaleno.
  ============================================================================
*/

#ifndef ANIMATION_PATTERN3_H
#define ANIMATION_PATTERN3_H
#include "config.h"


// Variabili di stato per l'animazione
static int pattern3ActiveGroup = 0;  // Gruppo attualmente acceso (0-3)
static uint16_t pattern3RainbowJ = 0; // Posizione nell'arcobaleno
static int pattern3Brightness = 10;
static bool pattern3Increasing = true;


void initPattern3Animation() {
  pattern3ActiveGroup = 0;
  pattern3RainbowJ = 0;
  pattern3Brightness = 10;
  pattern3Increasing = true;
}

void pattern3Animation() {
  if (millis() - lastAnimationUpdate >= PATTERN3_UPDATE_DELAY) {
    
    // Calcola quanti LED per gruppo in base al numero totale
    int ledsPerGroup = NUM_LEDS / 4;  // 4 gruppi
    
    // Accendi tutti i LED con colori arcobaleno in base al gruppo
    for (int i = 0; i < NUM_LEDS; i++) {
      int ledGroup = i / ledsPerGroup;  // Determina a quale gruppo appartiene il LED (0-3)
      
      // Calcola il colore arcobaleno per questo gruppo
      // Ogni gruppo ha un colore diverso (offset di 64 = 256/4)
      uint32_t groupColor = Wheel((ledGroup * 64 + pattern3RainbowJ) & 255);
      
      if (ledGroup == pattern3ActiveGroup) {
        // Gruppo attivo: usa luminosità crescente
        uint32_t dimmedColor = dimColor(groupColor, pattern3Brightness);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        // Altri gruppi: luminosità minima
        uint32_t dimmedColor = dimColor(groupColor, 10);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Anima la luminosità del gruppo attivo
    if (pattern3Increasing) {
      pattern3Brightness += PATTERN3_BRIGHTNESS_STEP;
      
      if (pattern3Brightness >= 255) {
        pattern3Brightness = 255;
        pattern3Increasing = false;
      }
    } else {
      pattern3Brightness -= PATTERN3_BRIGHTNESS_STEP;
      
      if (pattern3Brightness <= 10) {
        pattern3Brightness = 10;
        pattern3Increasing = true;
        
        // Passa al gruppo successivo
        pattern3ActiveGroup++;
        
        // Se ha completato tutti i gruppi, ricomincia e ruota i colori
        if (pattern3ActiveGroup >= 4) {
          pattern3ActiveGroup = 0;
          
          // Ruota i colori arcobaleno
          pattern3RainbowJ += 4;
          if (pattern3RainbowJ >= 256) {
            pattern3RainbowJ = 0;
          }
        }
      }
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
