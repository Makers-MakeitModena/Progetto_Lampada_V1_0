/*
  ============================================================================
  ANIMAZIONE: ONDA VELOCITÀ VARIABILE
  ============================================================================
  Come l'onda normale ma la velocità cambia nel tempo:
  - Inizia lenta
  - Accelera gradualmente
  - Arriva al massimo
  - Rallenta di nuovo
  Crea un effetto di "respiro" molto dinamico.
  ============================================================================
*/

#ifndef ANIMATION_WAVE_VARIABLE_H
#define ANIMATION_WAVE_VARIABLE_H
#include "config.h"


// Variabili di stato per l'animazione
static int waveVarBrightnessEven = 10;
static int waveVarBrightnessOdd = 255;
static bool waveVarIncreasing = true;
static byte waveVarColorIndex = 0;
static int waveVarSpeed = 1;  // Velocità variabile (1-5)
static bool waveVarSpeedIncreasing = true;


void initWaveVariableAnimation() {
  waveVarBrightnessEven = 10;
  waveVarBrightnessOdd = 255;
  waveVarIncreasing = true;
  waveVarColorIndex = 0;
  waveVarSpeed = 1;
  waveVarSpeedIncreasing = true;
}

void waveVariableAnimation() {
  if (millis() - lastAnimationUpdate >= WAVE_VAR_UPDATE_DELAY) {
    
    uint32_t currentColor = Wheel(waveVarColorIndex);
    
    for (int i = 0; i < NUM_LEDS; i++) {
      if (i % 2 == 0) {
        uint32_t dimmedColor = dimColor(currentColor, waveVarBrightnessEven);
        pixels.setPixelColor(i, dimmedColor);
      } else {
        uint32_t dimmedColor = dimColor(currentColor, waveVarBrightnessOdd);
        pixels.setPixelColor(i, dimmedColor);
      }
    }
    
    pixels.show();
    
    // Aggiorna le luminosità usando velocità variabile
    if (waveVarIncreasing) {
      waveVarBrightnessEven += waveVarSpeed;
      waveVarBrightnessOdd -= waveVarSpeed;
      
      if (waveVarBrightnessEven >= 255) {
        waveVarBrightnessEven = 255;
        waveVarBrightnessOdd = 10;
        waveVarIncreasing = false;
      }
    } else {
      waveVarBrightnessEven -= waveVarSpeed;
      waveVarBrightnessOdd += waveVarSpeed;
      
      if (waveVarBrightnessEven <= 10) {
        waveVarBrightnessEven = 10;
        waveVarBrightnessOdd = 255;
        waveVarIncreasing = true;
        
        // Ciclo completato! Cambia colore
        waveVarColorIndex += 36;
        if (waveVarColorIndex >= 256) {
          waveVarColorIndex = 0;
        }
      }
    }
    
    // Varia la velocità nel tempo (effetto respiro)
    static int speedChangeCounter = 0;
    speedChangeCounter++;
    
    if (speedChangeCounter >= 30) {  // Cambia velocità ogni 30 frame
      speedChangeCounter = 0;
      
      if (waveVarSpeedIncreasing) {
        waveVarSpeed++;
        if (waveVarSpeed >= WAVE_VAR_MAX_SPEED) {
          waveVarSpeed = WAVE_VAR_MAX_SPEED;
          waveVarSpeedIncreasing = false;
        }
      } else {
        waveVarSpeed--;
        if (waveVarSpeed <= WAVE_VAR_MIN_SPEED) {
          waveVarSpeed = WAVE_VAR_MIN_SPEED;
          waveVarSpeedIncreasing = true;
        }
      }
    }
    
    lastAnimationUpdate = millis();
  }
}

#endif
