/*
  ============================================================================
  LAMPADA NEOPIXEL CON CONTROLLO WiFi - WEMOS/NODEMCU ESP8266
  ============================================================================
  
  Lampada intelligente con 16 LED NeoPixel controllabile tramite:
  - Pulsanti fisici (TB1 e TB2)
  - Web App via WiFi (da browser smartphone/PC)
  
  MODALITÀ DI CONTROLLO:
  
  1. PULSANTI FISICI:
     TB1 (Pin D2 / GPIO4) - Navigazione indietro e ON/OFF:
       - Click breve: Animazione PRECEDENTE
       - Pressione lunga: ON/OFF
     
     TB2 (Pin D1 / GPIO5) - Navigazione avanti e luminosità:
       - Click breve: Animazione SUCCESSIVA
       - Pressione lunga: Varia luminosità
  
  2. WEB APP (via WiFi):
     - Pulsante ON/OFF (verde/rosso)
     - Pulsante animazione successiva (»)
     - Pulsante animazione precedente («)
     - Pulsante aumenta luminosità (↑)
     - Pulsante diminuisci luminosità (↓)
     - Configurazione WiFi integrata
  
  PRIMA CONFIGURAZIONE WiFi:
  1. La lampada crea un Access Point "LampadaSetup"
  2. Connettiti con smartphone/PC (password: 12345678)
  3. Apri browser su http://192.168.4.1
  4. Inserisci SSID e password della tua rete WiFi
  5. La lampada si riconnette alla tua rete
  6. Controlla la lampada dal browser
  
  CAMBIO RETE WiFi:
  - Tieni premuto TB1 per 3 secondi all'accensione
  - La lampada torna in modalità Access Point
  - Riconfigura la rete come sopra
  
  Hardware:
  - Wemos D1 Mini o NodeMCU (ESP8266)
  - Anello NeoPixel 16 LED (GPIO2 / D4)
  - Pulsanti TB1 (GPIO4 / D2) e TB2 (GPIO5 / D1)
  
  ============================================================================
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

// Includi configurazione e animazioni
#include "config.h"
#include "animation_white.h"
#include "animation_random.h"
#include "animation_rainbow.h"
#include "animation_rainbow_wave.h"
#include "animation_wave.h"
#include "animation_wave_inverse.h"
#include "animation_wave_gradient.h"
#include "animation_wave_variable.h"
#include "animation_pattern3.h"

// ============================================================================
// OGGETTI
// ============================================================================

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WebServer server(80);

// Indirizzi EEPROM
#define EEPROM_SIZE 512
#define ADDR_SSID 0
#define ADDR_PASSWORD 32
#define ADDR_ANIMATION 96
#define ADDR_BRIGHTNESS 97

// ============================================================================
// VARIABILI GLOBALI
// ============================================================================

// WiFi
String ssid = "";
String password = "";
bool wifiConfigMode = false;
bool wifiConnected = false;

// Pulsanti
bool TB1_pressed = false;
bool TB1_aux = false;
unsigned long TB1_pressTime = 0;

bool TB2_pressed = false;
bool TB2_aux = false;
unsigned long TB2_pressTime = 0;
bool TB2_longPressActive = false;
unsigned long TB2_lastPressTime = 0;

// Stato lampada
bool lampOn = false;
int currentAnimation = 0;
int brightness = 255;
bool brightnessIncreasing = false;
unsigned long lastBrightnessChange = 0;
unsigned long lastAnimationUpdate = 0;

// ============================================================================
// DICHIARAZIONI FORWARD
// ============================================================================

void handleTB1();
void handleTB2();
void runCurrentAnimation();
void setupWiFi();
void startAccessPoint();
void handleRoot();
void handleControl();
void handleSaveWiFi();
void handleNotFound();

// ============================================================================
// FUNZIONI MEMORIA PERSISTENTE
// ============================================================================

void saveWiFiCredentials(String ssid, String pass) {
  for (int i = 0; i < 32; i++) {
    EEPROM.write(ADDR_SSID + i, (i < ssid.length()) ? ssid[i] : 0);
  }
  for (int i = 0; i < 64; i++) {
    EEPROM.write(ADDR_PASSWORD + i, (i < pass.length()) ? pass[i] : 0);
  }
  EEPROM.commit();
  Serial.println(">>> WiFi salvato: " + ssid);
}

void loadWiFiCredentials() {
  char ssidBuf[33];
  for (int i = 0; i < 32; i++) {
    ssidBuf[i] = EEPROM.read(ADDR_SSID + i);
  }
  ssidBuf[32] = '\0';
  ssid = String(ssidBuf);
  ssid.trim();
  
  char passBuf[65];
  for (int i = 0; i < 64; i++) {
    passBuf[i] = EEPROM.read(ADDR_PASSWORD + i);
  }
  passBuf[64] = '\0';
  password = String(passBuf);
  password.trim();
  
  Serial.println(">>> WiFi caricato: " + ssid);
}

void saveAnimation() {
  EEPROM.write(ADDR_ANIMATION, currentAnimation);
  EEPROM.commit();
}

int loadAnimation() {
  int saved = EEPROM.read(ADDR_ANIMATION);
  if (saved >= 0 && saved < NUM_ANIMATIONS) {
    return saved;
  }
  return 0;
}

void saveBrightness() {
  EEPROM.write(ADDR_BRIGHTNESS, brightness);
  EEPROM.commit();
}

int loadBrightness() {
  int saved = EEPROM.read(ADDR_BRIGHTNESS);
  if (saved >= 10 && saved <= 255) {
    return saved;
  }
  return 255;
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n====================================");
  Serial.println("   LAMPADA NEOPIXEL WiFi - AVVIO");
  Serial.println("   Wemos/NodeMCU ESP8266");
  Serial.println("====================================\n");
  
  // Inizializza EEPROM
  EEPROM.begin(EEPROM_SIZE);
  
  // Carica impostazioni
  currentAnimation = loadAnimation();
  brightness = loadBrightness();
  loadWiFiCredentials();
  
  // Inizializza NeoPixel
  pixels.begin();
  pixels.setBrightness(brightness);
  pixels.clear();
  pixels.show();
  
  // Configura pulsanti
  pinMode(TOUCH1_PIN, INPUT);
  pinMode(TOUCH2_PIN, INPUT);
  
  // Controlla se entrare in modalità configurazione
  // (TB1 premuto per 3 secondi all'avvio)
  Serial.println("Tieni premuto TB1 per 3 secondi per configurare WiFi...");
  unsigned long startTime = millis();
  bool configRequested = false;
  
  while (millis() - startTime < 3000) {
    if (digitalRead(TOUCH1_PIN) == HIGH) {
      configRequested = true;
    } else {
      configRequested = false;
    }
    delay(10);
  }
  
  if (configRequested || ssid == "") {
    Serial.println(">>> Modalità CONFIGURAZIONE WiFi");
    wifiConfigMode = true;
    startAccessPoint();
  } else {
    Serial.println(">>> Modalità NORMALE - Connessione WiFi");
    setupWiFi();
  }
  
  // Setup web server
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.on("/savewifi", handleSaveWiFi);
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println(">>> Server web avviato");
  Serial.println("====================================\n");
}

// ============================================================================
// LOOP PRINCIPALE
// ============================================================================

void loop() {
  server.handleClient();
  
  handleTB1();
  
  if (lampOn) {
    handleTB2();
    runCurrentAnimation();
  } else {
    pixels.clear();
    pixels.show();
  }
  
  delay(10);
}

// ============================================================================
// GESTIONE WiFi
// ============================================================================

void setupWiFi() {
  if (ssid == "") {
    Serial.println(">>> Nessuna rete configurata");
    startAccessPoint();
    return;
  }
  
  Serial.println("\n=== DEBUG CONNESSIONE WiFi ===");
  Serial.print(">>> SSID: ");
  Serial.println(ssid);
  Serial.print(">>> Password length: ");
  Serial.println(password.length());
  Serial.println(">>> Password (primi 3 caratteri): " + password.substring(0, 3) + "...");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());
  
  Serial.println(">>> Tentativo connessione in corso...");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    
    // Mostra stato ogni 5 tentativi
    if (attempts % 5 == 4) {
      Serial.println();
      Serial.print("   Status WiFi: ");
      switch(WiFi.status()) {
        case WL_IDLE_STATUS:
          Serial.println("IDLE");
          break;
        case WL_NO_SSID_AVAIL:
          Serial.println("RETE NON TROVATA!");
          break;
        case WL_CONNECT_FAILED:
          Serial.println("CONNESSIONE FALLITA (password errata?)");
          break;
        case WL_DISCONNECTED:
          Serial.println("DISCONNESSO");
          break;
        default:
          Serial.println(WiFi.status());
      }
      Serial.print("   RSSI: ");
      Serial.println(WiFi.RSSI());
    }
    
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    wifiConnected = true;
    Serial.println("\n>>> WiFi CONNESSO! ✅");
    Serial.print(">>> IP: ");
    Serial.println(WiFi.localIP());
    Serial.print(">>> Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print(">>> RSSI (segnale): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    
    // LED lampeggiano verde per conferma
    for (int i = 0; i < 3; i++) {
      pixels.fill(pixels.Color(0, 255, 0));
      pixels.show();
      delay(200);
      pixels.clear();
      pixels.show();
      delay(200);
    }
  } else {
    Serial.println("\n>>> Connessione FALLITA ❌");
    Serial.print(">>> Status finale: ");
    switch(WiFi.status()) {
      case WL_NO_SSID_AVAIL:
        Serial.println("RETE NON TROVATA");
        Serial.println(">>> Possibili cause:");
        Serial.println("    - SSID scritto male");
        Serial.println("    - Router spento");
        Serial.println("    - Fuori portata");
        break;
      case WL_CONNECT_FAILED:
        Serial.println("PASSWORD ERRATA");
        Serial.println(">>> Verifica la password WiFi!");
        break;
      case WL_DISCONNECTED:
        Serial.println("DISCONNESSO");
        Serial.println(">>> Possibili cause:");
        Serial.println("    - Segnale troppo debole");
        Serial.println("    - WiFi 5GHz (serve 2.4GHz)");
        Serial.println("    - Router sovraccarico");
        break;
      default:
        Serial.print("CODICE ERRORE: ");
        Serial.println(WiFi.status());
    }
    Serial.println("\n>>> Avvio Access Point");
    startAccessPoint();
  }
}

void startAccessPoint() {
  wifiConfigMode = true;
  
  Serial.println(">>> Creazione Access Point: LampadaSetup");
  
  WiFi.mode(WIFI_AP);
  WiFi.softAP("LampadaSetup", "12345678");
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print(">>> IP Access Point: ");
  Serial.println(IP);
  Serial.println(">>> Connettiti alla rete 'LampadaSetup'");
  Serial.println(">>> Password: 12345678");
  Serial.println(">>> Apri browser su: http://192.168.4.1");
  
  // LED lampeggiano blu per indicare modalità configurazione
  for (int i = 0; i < 5; i++) {
    pixels.fill(pixels.Color(0, 0, 255));
    pixels.show();
    delay(300);
    pixels.clear();
    pixels.show();
    delay(300);
  }
}

// ============================================================================
// WEB SERVER - HANDLERS
// ============================================================================

void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Lampada</title>";
  
  // Icona PNG 180x180 - Lampadina gialla su sfondo nero
  html += "<link rel='apple-touch-icon' href='data:image/png;base64,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'>";
  html += "<meta name='apple-mobile-web-app-capable' content='yes'>";
  html += "<meta name='apple-mobile-web-app-title' content='Lampada'>";
  html += "<meta name='apple-mobile-web-app-status-bar-style' content='black'>";
  
  html += "<style>";
  html += "body { font-family: Arial; text-align: center; padding: 20px; background: #1a1a1a; color: #fff; }";
  html += "h1 { color: #00ffff; margin-bottom: 30px; }";
  html += ".container { max-width: 500px; margin: 0 auto; }";
  html += ".status { padding: 15px; margin: 20px 0; border-radius: 10px; font-size: 18px; }";
  html += ".status.on { background: #00ff00; color: #000; }";
  html += ".status.off { background: #ff0000; color: #fff; }";
  html += ".controls { margin: 30px 0; }";
  html += ".btn { padding: 20px; margin: 10px; font-size: 24px; border: none; border-radius: 10px; cursor: pointer; min-width: 80px; }";
  html += ".btn-power { background: ";
  html += lampOn ? "#00ff00; color: #000;" : "#ff0000; color: #fff;";
  html += " font-size: 20px; padding: 25px 40px; }";
  html += ".btn-nav { background: #007bff; color: #fff; }";
  html += ".btn-bright { background: #ffc107; color: #000; }";
  html += ".info { margin-top: 30px; padding: 15px; background: #333; border-radius: 10px; }";
  html += ".wifi-config { margin-top: 40px; padding: 20px; background: #2a2a2a; border-radius: 10px; }";
  html += "input { padding: 10px; margin: 10px; width: 80%; font-size: 16px; border-radius: 5px; }";
  html += ".btn-save { background: #28a745; color: #fff; padding: 15px 30px; margin-top: 15px; }";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<h1>🌈 Lampada NeoPixel</h1>";
  
  // Status
  html += "<div class='status ";
  html += lampOn ? "on'>✅ ACCESA" : "off'>⭕ SPENTA";
  html += "</div>";
  
  if (wifiConfigMode) {
    // Modalità configurazione WiFi
    html += "<div class='wifi-config'>";
    html += "<h2>⚙️ Configurazione WiFi</h2>";
    html += "<p>Inserisci i dati della tua rete WiFi:</p>";
    html += "<form action='/savewifi' method='POST'>";
    html += "<input type='text' name='ssid' placeholder='Nome rete (SSID)' required><br>";
    html += "<input type='password' name='password' placeholder='Password' required><br>";
    html += "<button type='submit' class='btn btn-save'>💾 Salva e Connetti</button>";
    html += "</form>";
    html += "<p style='margin-top: 20px; font-size: 14px; color: #888;'>";
    html += "Dopo il salvataggio, la lampada si riavvierà<br>e si connetterà alla rete WiFi.</p>";
    html += "</div>";
  } else {
    // Controlli lampada
    html += "<div class='controls'>";
    
    // Power
    html += "<div><button class='btn btn-power' onclick='control(\"power\")'>";
    html += lampOn ? "🔴 SPEGNI" : "🟢 ACCENDI";
    html += "</button></div>";
    
    // Navigazione animazioni
    html += "<div style='margin: 20px 0;'>";
    html += "<button class='btn btn-nav' onclick='control(\"prev\")'>« PRECEDENTE</button>";
    html += "<button class='btn btn-nav' onclick='control(\"next\")'>SUCCESSIVA »</button>";
    html += "</div>";
    
    // Luminosità
    html += "<div>";
    html += "<button class='btn btn-bright' onclick='control(\"brightup\")'>🔆 AUMENTA</button>";
    html += "<button class='btn btn-bright' onclick='control(\"brightdown\")'>🔅 DIMINUISCI</button>";
    html += "</div>";
    
    html += "</div>";
    
    // Info
    html += "<div class='info'>";
    html += "<p><strong>Animazione:</strong> ";
    html += String(currentAnimation);
    html += " - ";
    switch(currentAnimation) {
      case 0: html += "Bianco fisso"; break;
      case 1: html += "Colori casuali"; break;
      case 2: html += "Rainbow"; break;
      case 3: html += "Rainbow Wave"; break;
      case 4: html += "Onda di luci"; break;
      case 5: html += "Onda inversa Rainbow"; break;
      case 6: html += "Onda graduale"; break;
      case 7: html += "Onda velocità variabile"; break;
      case 8: html += "Pattern Rainbow"; break;
    }
    html += "</p>";
    html += "<p><strong>Luminosità:</strong> " + String((brightness * 100) / 255) + "%</p>";
    html += "<p><strong>WiFi:</strong> " + String(ssid) + "</p>";
    html += "<p><strong>IP:</strong> " + WiFi.localIP().toString() + "</p>";
    html += "</div>";
    
    // Link riconfigurazione
    html += "<div style='margin-top: 30px;'>";
    html += "<p style='font-size: 14px; color: #888;'>Per cambiare rete WiFi, tieni premuto TB1<br>per 3 secondi all'accensione della lampada.</p>";
    html += "</div>";
  }
  
  html += "</div>";
  
  // JavaScript
  html += "<script>";
  html += "function control(cmd) {";
  html += "  fetch('/control?cmd=' + cmd).then(() => location.reload());";
  html += "}";
  html += "</script>";
  
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleControl() {
  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");
    Serial.println(">>> Comando ricevuto: " + cmd);
    
    if (cmd == "power") {
      lampOn = !lampOn;
      if (!lampOn) saveBrightness();
      Serial.println(">>> Lampada: " + String(lampOn ? "ON" : "OFF"));
      
    } else if (cmd == "next" && lampOn) {
      currentAnimation = (currentAnimation + 1) % NUM_ANIMATIONS;
      saveAnimation();
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      Serial.println(">>> Animazione successiva: " + String(currentAnimation));
      
    } else if (cmd == "prev" && lampOn) {
      currentAnimation--;
      if (currentAnimation < 0) currentAnimation = NUM_ANIMATIONS - 1;
      saveAnimation();
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      Serial.println(">>> Animazione precedente: " + String(currentAnimation));
      
    } else if (cmd == "brightup" && lampOn) {
      brightness += 25;
      if (brightness > 255) brightness = 255;
      pixels.setBrightness(brightness);
      saveBrightness();
      Serial.println(">>> Luminosità aumentata: " + String((brightness * 100) / 255) + "%");
      
    } else if (cmd == "brightdown" && lampOn) {
      brightness -= 25;
      if (brightness < 10) brightness = 10;
      pixels.setBrightness(brightness);
      saveBrightness();
      Serial.println(">>> Luminosità diminuita: " + String((brightness * 100) / 255) + "%");
    }
  }
  
  server.send(200, "text/plain", "OK");
}

void handleSaveWiFi() {
  if (server.hasArg("ssid") && server.hasArg("password")) {
    String newSSID = server.arg("ssid");
    String newPassword = server.arg("password");
    
    saveWiFiCredentials(newSSID, newPassword);
    
    String html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<style>body{font-family:Arial;text-align:center;padding:50px;background:#1a1a1a;color:#fff;}</style>";
    html += "</head><body>";
    html += "<h1>✅ WiFi Salvato!</h1>";
    html += "<p>La lampada si sta riavviando...</p>";
    html += "<p>Connettiti alla rete <strong>" + newSSID + "</strong></p>";
    html += "<p>Poi cerca l'IP della lampada nel tuo router</p>";
    html += "</body></html>";
    
    server.send(200, "text/html", html);
    
    delay(2000);
    ESP.restart();
  } else {
    server.send(400, "text/plain", "Parametri mancanti");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404 - Pagina non trovata");
}

// ============================================================================
// GESTIONE PULSANTI (come prima)
// ============================================================================

void handleTB1() {
  TB1_pressed = digitalRead(TOUCH1_PIN);
  
  if (TB1_pressed && !TB1_aux) {
    TB1_pressTime = millis();
    Serial.println("TB1 premuto");
  }
  
  if (!TB1_pressed && TB1_aux) {
    unsigned long pressDuration = millis() - TB1_pressTime;
    
    if (pressDuration >= LONG_PRESS_TIME) {
      lampOn = !lampOn;
      
      if (lampOn) {
        pixels.setBrightness(brightness);
        Serial.print(">>> Lampada ACCESA - Animazione: ");
        switch(currentAnimation) {
          case 0: Serial.println("Bianco fisso"); initWhiteAnimation(); break;
          case 1: Serial.println("Colori casuali"); initRandomAnimation(); break;
          case 2: Serial.println("Rainbow"); initRainbowAnimation(); break;
          case 3: Serial.println("Rainbow Wave"); initRainbowWaveAnimation(); break;
          case 4: Serial.println("Onda di luci"); initWaveAnimation(); break;
          case 5: Serial.println("Onda inversa Rainbow"); initWaveInverseAnimation(); break;
          case 6: Serial.println("Onda graduale"); initWaveGradientAnimation(); break;
          case 7: Serial.println("Onda velocità variabile"); initWaveVariableAnimation(); break;
          case 8: Serial.println("Pattern Rainbow"); initPattern3Animation(); break;
        }
      } else {
        saveBrightness();
        Serial.println(">>> Lampada SPENTA");
      }
      
    } else if (lampOn) {
      currentAnimation--;
      if (currentAnimation < 0) {
        currentAnimation = NUM_ANIMATIONS - 1;
      }
      
      saveAnimation();
      
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      
      Serial.print(">>> Animazione PRECEDENTE: ");
      switch(currentAnimation) {
        case 0: Serial.println("Bianco fisso"); break;
        case 1: Serial.println("Colori casuali"); break;
        case 2: Serial.println("Rainbow"); break;
        case 3: Serial.println("Rainbow Wave"); break;
        case 4: Serial.println("Onda di luci"); break;
        case 5: Serial.println("Onda inversa Rainbow"); break;
        case 6: Serial.println("Onda graduale"); break;
        case 7: Serial.println("Onda velocità variabile"); break;
        case 8: Serial.println("Pattern Rainbow"); break;
      }
    }
  }
  
  TB1_aux = TB1_pressed;
}

void handleTB2() {
  TB2_pressed = digitalRead(TOUCH2_PIN);
  
  if (TB2_pressed && !TB2_aux) {
    unsigned long currentTime = millis();
    
    if (currentTime - TB2_lastPressTime < DEBOUNCE_TIME) {
      return;
    }
    
    TB2_lastPressTime = currentTime;
    TB2_pressTime = currentTime;
    TB2_longPressActive = false;
    Serial.println("TB2 premuto");
  }
  
  if (TB2_pressed && TB2_aux) {
    unsigned long pressDuration = millis() - TB2_pressTime;
    
    if (pressDuration >= LONG_PRESS_TIME && !TB2_longPressActive && lampOn) {
      TB2_longPressActive = true;
      
      if (brightness > 127) {
        brightnessIncreasing = false;
      } else {
        brightnessIncreasing = true;
      }
      
      Serial.print("Pressione lunga - Luminosità ");
      Serial.println(brightnessIncreasing ? "AUMENTA" : "DIMINUISCE");
    }
    
    if (TB2_longPressActive && lampOn) {
      if (millis() - lastBrightnessChange >= BRIGHTNESS_DELAY) {
        
        if (brightnessIncreasing) {
          brightness += BRIGHTNESS_STEP;
          if (brightness > 255) brightness = 255;
        } else {
          brightness -= BRIGHTNESS_STEP;
          if (brightness < 10) brightness = 10;
        }
        
        pixels.setBrightness(brightness);
        lastBrightnessChange = millis();
        
        Serial.print("Luminosità: ");
        Serial.print(brightness);
        Serial.print(" (");
        Serial.print((brightness * 100) / 255);
        Serial.println("%)");
      }
    }
  }
  
  if (!TB2_pressed && TB2_aux) {
    unsigned long pressDuration = millis() - TB2_pressTime;
    
    if (pressDuration < LONG_PRESS_TIME && lampOn) {
      currentAnimation = (currentAnimation + 1) % NUM_ANIMATIONS;
      
      saveAnimation();
      
      initWhiteAnimation();
      initRandomAnimation();
      initRainbowAnimation();
      initRainbowWaveAnimation();
      initWaveAnimation();
      initWaveInverseAnimation();
      initWaveGradientAnimation();
      initWaveVariableAnimation();
      initPattern3Animation();
      
      Serial.print(">>> Animazione SUCCESSIVA: ");
      switch(currentAnimation) {
        case 0: Serial.println("Bianco fisso"); break;
        case 1: Serial.println("Colori casuali"); break;
        case 2: Serial.println("Rainbow"); break;
        case 3: Serial.println("Rainbow Wave"); break;
        case 4: Serial.println("Onda di luci"); break;
        case 5: Serial.println("Onda inversa Rainbow"); break;
        case 6: Serial.println("Onda graduale"); break;
        case 7: Serial.println("Onda velocità variabile"); break;
        case 8: Serial.println("Pattern Rainbow"); break;
      }
    }
    
    if (TB2_longPressActive) {
      brightnessIncreasing = !brightnessIncreasing;
      Serial.print("Rilascio - direzione cambiata: ");
      Serial.println(brightnessIncreasing ? "AUMENTA" : "DIMINUISCE");
      TB2_longPressActive = false;
    }
  }
  
  TB2_aux = TB2_pressed;
}

// ============================================================================
// ESECUZIONE ANIMAZIONI
// ============================================================================

void runCurrentAnimation() {
  switch (currentAnimation) {
    case 0: whiteAnimation(); break;
    case 1: randomAnimation(); break;
    case 2: rainbowAnimation(); break;
    case 3: rainbowWaveAnimation(); break;
    case 4: waveAnimation(); break;
    case 5: waveInverseAnimation(); break;
    case 6: waveGradientAnimation(); break;
    case 7: waveVariableAnimation(); break;
    case 8: pattern3Animation(); break;
  }
}

// ============================================================================
// UTILITY - Wheel e dimColor
// ============================================================================

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

uint32_t dimColor(uint32_t color, byte brightness) {
  uint8_t r = (uint8_t)(color >> 16);
  uint8_t g = (uint8_t)(color >> 8);
  uint8_t b = (uint8_t)color;
  
  r = (r * brightness) / 255;
  g = (g * brightness) / 255;
  b = (b * brightness) / 255;
  
  return pixels.Color(r, g, b);
}
