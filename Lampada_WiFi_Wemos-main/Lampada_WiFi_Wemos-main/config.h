/*
  ============================================================================
  FILE DI CONFIGURAZIONE CONDIVISO
  ============================================================================
  
  Questo file contiene tutte le costanti e definizioni condivise tra
  il file principale e tutti i file delle animazioni.
  
  MODIFICA QUESTO FILE per personalizzare la tua lampada!
  
  ============================================================================
*/

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// CONFIGURAZIONE HARDWARE
// ============================================================================

// Numero di LED nell'anello NeoPixel
// MODIFICA QUESTO VALORE se hai un anello con un numero diverso di LED
#define NUM_LEDS 16

// Pin di collegamento per Wemos/NodeMCU ESP8266
#define TOUCH1_PIN 4       // GPIO4 (D2 su NodeMCU) per pulsante TB1
#define TOUCH2_PIN 5       // GPIO5 (D1 su NodeMCU) per pulsante TB2
#define DATA_PIN 2         // GPIO2 (D4 su NodeMCU) per NeoPixel

// ============================================================================
// CALIBRAZIONE COLORE BIANCO
// ============================================================================
// Modifica questi valori per ottenere il bianco perfetto:
// - Valori più bassi di WHITE_R rendono il bianco meno rossastro
// - Valori più alti di WHITE_B rendono il bianco più bluastro/freddo
//
// Presets consigliati:
// Bianco freddo:        150, 230, 255 (default)
// Bianco neutro:        200, 255, 255
// Bianco caldo:         220, 255, 240

#define WHITE_R 150   // Componente ROSSO del bianco (0-255)
#define WHITE_G 230   // Componente VERDE del bianco (0-255)
#define WHITE_B 255   // Componente BLU del bianco (0-255)

// ============================================================================
// CONFIGURAZIONE ANIMAZIONI
// ============================================================================

#define NUM_ANIMATIONS 9   // Numero totale di animazioni disponibili

// ============================================================================
// CONFIGURAZIONE CONTROLLI
// ============================================================================

// Tempo minimo (millisecondi) per considerare una pressione come "lunga"
#define LONG_PRESS_TIME 700    // 700ms per evitare pressione lunga accidentale

// Tempo minimo tra due letture del pulsante per evitare rimbalzi (debouncing)
#define DEBOUNCE_TIME 50       // Ignora letture entro 50ms dalla precedente

// Parametri controllo luminosità
#define BRIGHTNESS_STEP 5      // Quanto cambia la luminosità ad ogni step
#define BRIGHTNESS_DELAY 50    // Millisecondi tra un cambio e l'altro

// ============================================================================
// CONFIGURAZIONE VELOCITÀ ANIMAZIONI
// ============================================================================
// Modifica questi valori per cambiare la velocità delle animazioni

// Animazione Colori Casuali
#define RANDOM_DELAY 100       // Millisecondi tra un LED e l'altro

// Animazione Rainbow
#define RAINBOW_DELAY 20       // Millisecondi per frame

// Animazione Rainbow Wave (arcobaleno con onda)
#define RAINBOW_WAVE_UPDATE_DELAY 15        // Millisecondi per frame
#define RAINBOW_WAVE_BRIGHTNESS_STEP 3      // Velocità cambio luminosità

// Animazioni Onda
#define WAVE_BRIGHTNESS_STEP 3     // Velocità cambio luminosità (1-10)
#define WAVE_UPDATE_DELAY 15       // Millisecondi per frame

// Animazione Onda Graduale
#define WAVE_GRAD_BRIGHTNESS_STEP 3
#define WAVE_GRAD_UPDATE_DELAY 15
#define WAVE_GRAD_COLOR_STEP 1     // Velocità cambio colore

// Animazione Onda Inversa
#define WAVE_INV_BRIGHTNESS_STEP 3
#define WAVE_INV_UPDATE_DELAY 15

// Animazione Onda Velocità Variabile
#define WAVE_VAR_UPDATE_DELAY 15
#define WAVE_VAR_MIN_SPEED 1
#define WAVE_VAR_MAX_SPEED 6

// Animazione Pattern 3 LED
#define PATTERN3_UPDATE_DELAY 15
#define PATTERN3_BRIGHTNESS_STEP 5

// ============================================================================
// VARIABILI GLOBALI CONDIVISE
// ============================================================================
// Queste variabili sono definite nel file principale e usate dalle animazioni

extern unsigned long lastAnimationUpdate;  // Timestamp ultimo aggiornamento animazione
extern Adafruit_NeoPixel pixels;           // Oggetto NeoPixel

// ============================================================================
// FUNZIONI GLOBALI CONDIVISE
// ============================================================================
// Queste funzioni sono definite nel file principale

extern uint32_t Wheel(byte WheelPos);      // Converte 0-255 in colore arcobaleno
extern uint32_t dimColor(uint32_t color, byte brightness);  // Riduce luminosità colore

#endif
