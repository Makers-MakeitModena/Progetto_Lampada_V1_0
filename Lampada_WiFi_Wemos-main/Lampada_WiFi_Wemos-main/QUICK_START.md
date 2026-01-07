# 🚀 Quick Start - 5 Minuti

## 📦 Cosa Serve

- ✅ Wemos D1 Mini o NodeMCU
- ✅ Anello NeoPixel 16 LED  
- ✅ 2 Pulsanti
- ✅ Alimentatore 5V 2A
- ✅ Cavi dupont

---

## 🔌 Collegamenti Veloci

```
Wemos       →    Cosa
─────────────────────────
D4 (GPIO2)  →  NeoPixel DIN
D2 (GPIO4)  →  TB1 (Pulsante)
D1 (GPIO5)  →  TB2 (Pulsante)
VIN         →  5V
GND         →  GND comune
```

**3 Fili + Alimentazione = Fatto!** ✅

---

## 💻 Arduino IDE - Setup Rapido

### 1. Board ESP8266
```
File → Preferences → URLs:
http://arduino.esp8266.com/stable/package_esp8266com_index.json

Tools → Board Manager → "esp8266" → Install
```

### 2. Libreria NeoPixel
```
Sketch → Include Library → Manage Libraries
Cerca: "Adafruit NeoPixel" → Install
```

### 3. Board Settings
```
Board: Generic ESP8266 Module
Upload Speed: 115200
Flash Size: 4MB
```

---

## 📤 Upload

1. Collega Wemos USB
2. Seleziona porta COM
3. Click Upload (→)

**Se errore**: Tieni RESET, click Upload, rilascia RESET quando vedi "Connecting..."

---

## 📱 Primo Avvio

### 1. LED Blu Lampeggiano (5 volte)
Lampada in modalità configurazione!

### 2. Smartphone → WiFi
Cerca rete: **LampadaSetup**  
Password: **12345678**

### 3. Browser
Si apre automaticamente (o vai a: **http://192.168.4.1**)

### 4. Inserisci WiFi
- SSID: tua rete WiFi
- Password: password rete
- Click **Salva**

### 5. LED Verde Lampeggiano (3 volte)
**Connessa!** ✅

---

## 🎮 Uso Quotidiano

### Pulsanti Fisici

**TB1 (Sinistra)**:
- Click = ⬅️ Animazione precedente
- Lungo = 🔴/🟢 ON/OFF

**TB2 (Destra)**:
- Click = ➡️ Animazione successiva  
- Lungo = ☀️/🌙 Luminosità +/-

### Web App

1. Serial Monitor → Leggi IP (es: 192.168.1.100)
2. Browser → http://192.168.1.100
3. Controlli grandi e facili! 📱

---

## 🎨 Animazioni

Naviga con pulsanti o web app:

0. Bianco
1. Casuali
2. Rainbow
3. Rainbow Wave
4. Onda
5. Onda Inversa
6. Onda Graduale
7. Onda Variabile
8. Pattern

---

## 💡 Tips

### Trova IP Velocemente
```
Arduino IDE → Tools → Serial Monitor
Baud: 115200
Vedi: ">>> IP: 192.168.x.xxx"
```

### Riconfigura WiFi
Tieni **TB1 premuto** durante accensione (3 sec)

### Regola Luminosità
TB2 pressione lunga (toggle aumenta/diminuisci)

### LED Blu Onboard
Normale! GPIO2 ha LED blu che lampeggia. Non è errore! 💙

---

## ⚡ Alimentazione

**Test**: USB PC (OK per provare)  
**Uso normale**: Alimentatore 5V 2A

16 LED bianchi = ~1A! USB potrebbe non bastare.

---

## 🐛 Problemi Comuni

### LED Non Si Accendono
✅ D4 → DIN?  
✅ Alimentazione 5V?  
✅ GND collegato?

### WiFi Non Funziona
✅ Serial Monitor aperto (115200)?  
✅ SSID/password corretti?  
✅ Leggi TROUBLESHOOTING_WIFI.md

### Upload Fallisce
✅ Tieni RESET durante upload  
✅ Porta COM corretta?  
✅ Driver CH340 installato?

---

## 🎯 Checklist Completa

- [ ] Hardware collegato (D4, D2, D1, 5V, GND)
- [ ] Board ESP8266 installata
- [ ] Libreria NeoPixel installata
- [ ] Sketch caricato senza errori
- [ ] Serial Monitor aperto (115200)
- [ ] WiFi configurato
- [ ] IP annotato
- [ ] Web app funzionante
- [ ] Pulsanti testati

**Tutto OK? Divertiti! 🎉**

---

## 📚 Documentazione

- **README.md** - Guida completa
- **TROUBLESHOOTING_WIFI.md** - Problemi WiFi
- **config.h** - Personalizzazione

---

**5 minuti e sei pronto! ⚡✨**
