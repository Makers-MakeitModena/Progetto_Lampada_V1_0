# 🔧 Troubleshooting WiFi - Lampada Wemos/NodeMCU

Guida completa per risolvere problemi di connessione WiFi.

---

## ❌ Problema: "Connessione FALLITA"

Vedi questo messaggio nel Serial Monitor:
```
>>> Connessione a: MiaReteWiFi
....................
>>> Connessione FALLITA
>>> Avvio Access Point
```

---

## 🔍 Diagnosi Rapida

### Cosa Leggere nel Serial Monitor

Apri Serial Monitor (115200 baud) e leggi i messaggi:

```
>>> Connessione a: MiaReteWiFi
>>> Tentativo 1...
>>> Tentativo 2...
>>> Tentativo 3...
>>> Connessione FALLITA
>>> Avvio Access Point "LampadaSetup"
```

**Problema**: ESP8266 non riesce a connettersi alla rete

---

## 🛠️ Soluzioni Comuni

### 1️⃣ Verifica SSID (Nome Rete)

**SSID è case-sensitive** (maiuscole ≠ minuscole)!

#### Controllo
1. **Smartphone** → Impostazioni → WiFi
2. Guarda il nome **ESATTO** della rete
3. Confronta carattere per carattere

#### Esempi Errori Comuni
```
✅ Corretto:   MiaReteWiFi
❌ Sbagliato:  miaretewifi     (minuscole)
❌ Sbagliato:  MIARETEWIFI     (tutto maiuscolo)
❌ Sbagliato:  Mia_Rete_WiFi   (underscore invece di spazi)
```

**Anche uno spazio o trattino sbagliato blocca tutto!**

---

### 2️⃣ Verifica Password

#### Controlli
- ✅ Password corretta (case-sensitive)
- ✅ Nessuno spazio prima/dopo
- ✅ Caratteri speciali scritti giusti

#### Dove Trovare Password
```
1. Etichetta sotto/dietro il router
2. App del provider internet
3. Pannello admin router (192.168.1.1 o 192.168.0.1)
```

#### Test
Prova a connettere **altro dispositivo** con stessa password:
- Funziona → Password corretta ✅
- Non funziona → Password sbagliata ❌

---

### 3️⃣ Frequenza 2.4GHz vs 5GHz

**CRITICO**: ESP8266 funziona **SOLO con 2.4GHz**!

#### Router Moderni
Molti router hanno **due reti**:
```
📶 NomeRete          → 2.4GHz  ✅ ESP8266 OK
📶 NomeRete-5G       → 5GHz    ❌ ESP8266 NON vede!
📶 NomeRete_5G       → 5GHz    ❌ ESP8266 NON vede!
```

#### Come Verificare
1. **Router Admin Panel**:
   - Browser → `192.168.1.1` o `192.168.0.1`
   - Login (solitamente admin/admin o scritto sul router)
   - Cerca "WiFi" o "Wireless Settings"
   - Controlla frequenza reti

2. **Smartphone**:
   - Alcune app mostrano banda WiFi
   - 2.4GHz = compatibile ✅
   - 5GHz = incompatibile ❌

#### Soluzione
- Connetti ESP8266 alla rete **2.4GHz**
- Se router ha solo 5GHz → Abilita 2.4GHz nelle impostazioni

---

### 4️⃣ Segnale WiFi Debole

#### Sintomi
```
Serial Monitor:
>>> Tentativo 1... (timeout)
>>> Tentativo 2... (timeout)
>>> RSSI: -85 dBm  ← Segnale debole!
```

#### RSSI Spiegato
```
-30 a -50 dBm   → Eccellente ⭐⭐⭐⭐⭐
-50 a -60 dBm   → Buono ⭐⭐⭐⭐
-60 a -70 dBm   → Discreto ⭐⭐⭐
-70 a -80 dBm   → Debole ⭐⭐
-80 a -90 dBm   → Molto debole ⭐
Sotto -90 dBm   → Inutilizzabile ❌
```

#### Soluzioni
**A. Avvicina Lampada**
- Metti ESP8266 più vicino al router
- Evita muri spessi, metalli, microonde

**B. Ripetitore WiFi**
- Compra range extender WiFi 2.4GHz
- Posizionalo tra router e lampada

**C. Cambia Canale Router**
- Router admin → WiFi settings
- Cambia canale (prova 1, 6, 11)
- Interferenze vicini possono disturbare

---

### 5️⃣ Filtri MAC Address Attivi

Alcuni router bloccano **dispositivi nuovi** per sicurezza.

#### Verifica
1. **Router Admin** → Security/Sicurezza
2. Cerca "MAC Filtering" o "Controllo Accessi"
3. Se attivo → Aggiungi ESP8266

#### Trova MAC Address ESP8266
Serial Monitor mostra il MAC all'avvio, oppure controlla nel router:
```
Admin panel → Dispositivi connessi
Cerca "Espressif" o "ESP8266"
```

#### Aggiungi alla Whitelist
```
Router → MAC Filtering
→ Aggiungi → [MAC ESP8266]
→ Permetti accesso
```

---

### 6️⃣ SSID Nascosto

Se la rete **non compare** nell'elenco WiFi:

#### Nel Router
```
Admin panel → WiFi settings
→ "Broadcast SSID" o "SSID Visibility"
→ Abilita (ON)
```

---

## 🧪 Test Diagnostici

### Test 1: Hotspot Smartphone

**Scopo**: Capire se problema è router o ESP8266

#### Procedura
1. **Smartphone** → Hotspot personale ON
2. Nome: `TestESP` Password: `12345678`
3. Connetti lampada a `TestESP`

#### Risultato
- ✅ Funziona → Problema è il router
- ❌ Non funziona → Problema è l'ESP8266

---

### Test 2: Altri Dispositivi

Prova a connettere **laptop o tablet** con stesse credenziali:
- Funziona → Credenziali OK, problema ESP8266
- Non funziona → Credenziali sbagliate

---

## 🔄 Reset Configurazione WiFi

### Metodo 1: Pulsante TB1
1. **Spegni** lampada
2. **Tieni premuto TB1**
3. **Accendi** lampada (mantieni TB1 premuto 3 secondi)
4. LED blu lampeggiano
5. WiFi "LampadaSetup" disponibile
6. Riconfigura

### Metodo 2: Cancellazione EEPROM

Se vuoi cancellare WiFi salvato manualmente:

```cpp
// Sketch temporaneo per reset WiFi
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  // Cancella WiFi (SSID + Password)
  for (int i = 0; i < 96; i++) {
    EEPROM.write(i, 0);
  }
  
  EEPROM.commit();  // ⚠️ FONDAMENTALE!
  
  Serial.println("✅ WiFi cancellato!");
}

void loop() {}
```

**Procedura**:
1. Copia codice sopra in nuovo sketch
2. Carica su Wemos
3. Serial Monitor mostra "✅ WiFi cancellato!"
4. Ricarica sketch lampada
5. Riconfigura WiFi

### Reset Completo EEPROM

Per cancellare **tutto** (WiFi + animazione + luminosità):

```cpp
#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  
  // Cancella TUTTA la EEPROM
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  
  EEPROM.commit();  // ⚠️ FONDAMENTALE!
  
  Serial.println("✅ EEPROM completamente cancellata!");
}

void loop() {}
```

**⚠️ IMPORTANTE**: `EEPROM.commit()` è **obbligatorio**! 
- Senza `commit()` = modifiche NON salvate
- ESP8266 lavora in RAM, `commit()` salva in Flash

---

## 💾 Struttura EEPROM

| Indirizzo | Bytes | Contenuto |
|-----------|-------|-----------|
| 0-31 | 32 | SSID WiFi |
| 32-95 | 64 | Password WiFi |
| 96 | 1 | Animazione (0-8) |
| 97 | 1 | Luminosità (10-255) |

---

## 📋 Checklist Completa

### Hardware
- [ ] ESP8266 alimentato (LED acceso)
- [ ] USB collegato correttamente
- [ ] Alimentazione sufficiente (5V 500mA min)

### Router
- [ ] Router acceso
- [ ] WiFi 2.4GHz attivo (non solo 5GHz)
- [ ] SSID broadcast attivo (non nascosto)
- [ ] Filtri MAC disabilitati (o ESP8266 in whitelist)

### Credenziali
- [ ] SSID esatto (maiuscole/minuscole)
- [ ] Password esatta (case-sensitive)
- [ ] Nessuno spazio extra
- [ ] Caratteri speciali corretti

### Software
- [ ] Serial Monitor 115200 baud
- [ ] Sketch caricato correttamente
- [ ] EEPROM non corrotta (prova reset)

### Segnale
- [ ] ESP8266 vicino al router (<10m)
- [ ] Nessun ostacolo (muri spessi)
- [ ] R
---

## 💡 Problemi LED NeoPixel

### LED Non Si Accendono

**Cause**:
- ✗ Collegamento DIN errato
- ✗ Alimentazione insufficiente
- ✗ LED difettosi

**Soluzioni**:

#### 1. Verifica Collegamenti
```
D4 (GPIO2) → DIN NeoPixel  ✅
VIN (5V)   → VCC NeoPixel  ✅
GND        → GND NeoPixel  ✅
```

#### 2. Alimentazione
- USB PC = OK per test (max 500mA)
- Uso normale = Alimentatore 5V 2A
- 16 LED bianchi = ~960mA!

#### 3. Test Sketch Semplice
```cpp
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(16, 2, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
  pixels.fill(pixels.Color(255, 0, 0));  // Rosso
  pixels.show();
}

void loop() {}
```

---

### LED Colori Sbagliati

**Soluzione**: Cambia in `config.h`:
```cpp
NEO_GRB + NEO_KHZ800  // Default
NEO_RGB + NEO_KHZ800  // Se colori invertiti
```

---

### LED Blu Onboard Lampeggia

**Normale!** GPIO2 ha LED blu integrato. Se disturba: copri con nastro nero.

---

## 🎮 Problemi Pulsanti

### Pulsanti Non Rispondono

**Verifica Collegamenti**:
```
TB1 → D2 (GPIO4) + GND  ✅
TB2 → D1 (GPIO5) + GND  ✅
```

**Test Serial Monitor**:
```cpp
Serial.println(digitalRead(4));  // TB1
Serial.println(digitalRead(5));  // TB2
```

---

## 🔌 Problemi Alimentazione

### Lampada Si Riavvia

**Causa**: Alimentazione insufficiente

**Soluzione**: Alimentatore 5V 2A minimo

**Consumo**:
```
Wemos:        ~80mA
16 LED bianchi: ~960mA
Totale:       ~1040mA
```

---

## 📤 Problemi Upload

### Upload Si Blocca

**Soluzione**:
1. Tieni **RESET** premuto
2. Click **Upload**
3. Rilascia RESET quando vedi "Connecting..."

---

### Driver Mancanti

**Windows**: Installa driver CH340 o CP2102

---

## 🐛 Altri Problemi

### Web App Non Carica

1. Verifica IP (Serial Monitor)
2. Stesso WiFi smartphone/PC?
3. Prova altro browser

---

### Disconnessioni Casuali

- Segnale WiFi debole (< -70 dBm)
- Alimentazione instabile
- Avvicina router o usa range extender

---

## 📋 Checklist Debug

- [ ] Serial Monitor 115200
- [ ] LED si accendono
- [ ] Pulsanti rispondono
- [ ] WiFi 2.4GHz
- [ ] Alimentatore 2A
- [ ] Collegamenti corretti

---

## 🎯 Problemi Più Comuni

| Problema | Soluzione |
|----------|-----------|
| LED spenti | Verifica D4→DIN, alimentazione |
| WiFi no | SSID esatto, 2.4GHz, password |
| Upload no | RESET durante upload |
| Riavvii | Alimentatore 2A |
| Pulsanti no | D2/D1 a GND |

---

**90% problemi = alimentazione + WiFi 2.4GHz + SSID/password!** ✅
