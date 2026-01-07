# 🌈 Lampada NeoPixel WiFi - Wemos/NodeMCU ESP8266

Lampada RGB intelligente con controllo WiFi e pulsanti fisici.

---

## ✨ Caratteristiche

- 🎨 **9 animazioni** RGB colorate
- 📱 **Controllo WiFi** da smartphone/PC
- 🎮 **2 pulsanti fisici** per controllo locale
- 💾 **Memoria persistente** (ricorda WiFi, animazione, luminosità)
- ⚙️ **Configurazione WiFi** via Access Point

---

## 🔌 Collegamenti Hardware

### Schema Base
```
Wemos/NodeMCU    →    Destinazione
────────────────────────────────────
D4 (GPIO2)       →    DIN NeoPixel
D2 (GPIO4)       →    TB1 (Pulsante Sinistra)
D1 (GPIO5)       →    TB2 (Pulsante Destra)
VIN o 5V         →    VCC NeoPixel
GND              →    GND comune
```

### NeoPixel Ring
```
NeoPixel         Wemos
────────────────────────
DIN           →  D4 (GPIO2)
VCC           →  VIN (5V)
GND           →  GND
```

### Pulsanti
```
TB1           →  D2 (GPIO4) + GND
TB2           →  D1 (GPIO5) + GND
```

---

## 🎮 Controlli

### Pulsante TB1 (Sinistra - D2)
- **Click breve**: ⬅️ Animazione precedente
- **Pressione lunga** (>1 secondo): 🔴/🟢 ON/OFF

### Pulsante TB2 (Destra - D1)
- **Click breve**: ➡️ Animazione successiva
- **Pressione lunga** (>1 secondo): ☀️/🌙 Aumenta/Diminuisci luminosità

### Web App (WiFi)
- http://192.168.x.xxx
- Pulsanti grandi touch-friendly
- Controllo completo da smartphone

---

## 🎨 9 Animazioni

0. ⚪ Bianco fisso
1. 🎲 Colori casuali
2. 🌈 Rainbow classico
3. 🌊 Rainbow Wave
4. 💫 Onda di luci
5. 🔄 Onda inversa
6. 🎨 Onda graduale
7. ⚡ Onda velocità variabile
8. 🎪 Pattern Rainbow

---

## 📱 Configurazione WiFi

### Prima Volta

1. **Accendi** la lampada
2. LED lampeggiano **BLU** (5 volte)
3. Cerca rete WiFi **"LampadaSetup"**
4. **Connetti** (password: `12345678`)
5. Browser apre automaticamente (o vai a `http://192.168.4.1`)
6. **Inserisci** SSID e password della tua rete
7. Click **"Salva"**
8. Lampada **riavvia** e si connette

### Dopo Configurazione

1. LED lampeggiano **VERDE** (3 volte) = Connessa!
2. Trova IP nel **Serial Monitor** (115200 baud)
3. Apri browser: `http://192.168.x.xxx`

### Riconfigurare WiFi

Tieni **TB1 premuto** durante accensione (3 secondi) per entrare in modalità configurazione.

---

## 🛠️ Installazione Arduino IDE

### 1. Aggiungi Board ESP8266

**File** → **Preferences** → **Additional Board Manager URLs**:
```
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

**Tools** → **Board Manager** → Cerca "**esp8266**" → **Install**

### 2. Installa Libreria

**Sketch** → **Include Library** → **Manage Libraries**

Cerca: **Adafruit NeoPixel** → **Install**

### 3. Configurazione Board

```
Board: Generic ESP8266 Module (o NodeMCU 1.0)
Upload Speed: 115200
CPU Frequency: 80 MHz  
Flash Size: 4MB (FS:2MB OTA:~1019KB)
Flash Mode: DIO
```

### 4. Carica Sketch

1. Collega Wemos via USB
2. Seleziona **porta COM** corretta
3. Click **Upload** (→)
4. Se errore: tieni **RESET**, click Upload, rilascia quando vedi "Connecting..."

---

## 💾 Memoria Persistente

La lampada ricorda:
- ✅ **WiFi** (SSID e password)
- ✅ **Animazione** attuale
- ✅ **Luminosità** impostata

Anche dopo spegnimento/riavvio! Dati salvati in EEPROM.

### Reset WiFi

**Metodo 1 - Pulsante TB1**:
1. Tieni TB1 premuto durante accensione (3 sec)
2. LED blu lampeggiano
3. Riconfigura WiFi

**Metodo 2 - Codice**:
Vedi sezione "Reset EEPROM" in `TROUBLESHOOTING.md`

---

## 🐛 Troubleshooting

### LED Non Si Accendono
- Verifica collegamento D4 → DIN
- Controlla alimentazione 5V (min 2A per 16 LED)
- GND comune collegato?

### WiFi Non Si Connette
- Leggi **TROUBLESHOOTING.md**
- Apri Serial Monitor (115200 baud)
- Prova riconfigurazione (TB1 durante boot)
- Verifica WiFi 2.4GHz (non 5GHz!)

### Upload Fallisce
- Tieni RESET premuto
- Click Upload
- Rilascia RESET quando vedi "Connecting..."

### Pulsanti Non Rispondono
- Verifica collegamenti D2 (TB1) e D1 (TB2)
- Pulsanti verso GND (pull-up interno attivo)

**Per problemi dettagliati**: Leggi `TROUBLESHOOTING.md`

---

## 📊 Specifiche Tecniche

**Microcontrollore**: ESP8266 (80MHz, 80KB RAM)  
**WiFi**: 802.11 b/g/n 2.4GHz  
**LED**: WS2812B (NeoPixel) su GPIO2  
**Pulsanti**: GPIO4, GPIO5 (pull-up interni)  
**Memoria**: EEPROM 512 bytes  
**Alimentazione**: 5V, 2A minimo (per 16 LED)  
**Consumo**: ~80mA (WiFi) + ~960mA (16 LED bianchi max)

---

## 📁 File Progetto

```
Lampada_WiFi_Wemos/
├── Lampada_WiFi_Wemos.ino    ← Sketch principale
├── config.h                   ← Configurazione pin e costanti
├── animation_*.h (x9)         ← File animazioni
├── README.md                  ← Questo file
├── QUICK_START.md             ← Avvio rapido 5 minuti
└── TROUBLESHOOTING.md         ← Risoluzione problemi completa
```

---

## 🎯 Quick Start

1. **Hardware**: Collega NeoPixel (D4), pulsanti (D2, D1), alimentazione
2. **Software**: Installa libreria Adafruit NeoPixel
3. **Upload**: Carica sketch su Wemos
4. **WiFi**: Connetti a "LampadaSetup" → Configura
5. **Usa**: Controlla da pulsanti o web app!

---

## ⚡ Consumo Energetico

```
Componente         Consumo
────────────────────────────
Wemos WiFi         ~80mA
16 LED (rosso)     ~320mA
16 LED (bianco)    ~960mA
────────────────────────────
Totale max         ~1040mA
```

**Alimentatore consigliato**: 5V 2A

---

## 🔗 Compatibilità

✅ Wemos D1 Mini  
✅ NodeMCU v2/v3  
✅ ESP-12E/F  
✅ Qualsiasi ESP8266 con almeno 4MB flash

---

## 📝 Note

- ⚠️ **LED Blu Onboard**: GPIO2 ha LED blu che lampeggia con NeoPixel (normale!)
- 💡 **Numero LED**: Modifica `NUM_LEDS` in `config.h` se diverso da 16
- 🎨 **Colore Bianco**: Calibra `WHITE_R/G/B` in `config.h` per bianco perfetto
- 🔋 **Alimentazione**: USB PC OK per test, usa alimentatore esterno per uso normale

---

**Buon divertimento con la tua lampada WiFi! 🌈✨**
