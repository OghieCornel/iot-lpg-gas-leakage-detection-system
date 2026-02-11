code - IOT.txt
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiManager.h>  
#include "config.h"



#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

// === OLED ===
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define MQ6_PIN A0
#define buzzer D5

String name = "LPG/Butane Detector";

// Gas level thresholds in PPM
const int SAFE_LEVEL = 600;     // Clean air
const int WARNING_LEVEL = 1000; // Mild leak
const int DANGER_LEVEL = 2000;  // Serious leak

unsigned long warmUpStartTime;
const unsigned long warmUpDuration = 60000; 

WiFiManager wm;  

char auth[] = BLYNK_AUTH_TOKEN;

// === IFTTT Webhook ===
const String IFTTT_EVENT = "gas_alert";

// Gas level categories for IFTTT logic
enum GasLevelCategory { LEVEL_SAFE, LEVEL_DETECTED, LEVEL_WARNING, LEVEL_DANGER };
GasLevelCategory lastLevel = LEVEL_SAFE;

void setup() {
WiFi.mode(WIFI_STA); 
Serial.begin(115200);
pinMode(buzzer, OUTPUT);
digitalWrite(buzzer, LOW);

if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
Serial.println(F("SSD1306 allocation failed"));
while (true); 
}

// Show "Starting AP Mode..." on OLED
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(10, 0);
display.println("Welcome, Starting up...");
display.display();

WiFi.mode(WIFI_STA);
delay(500);
WiFi.hostname("GAS DETECTOR");
wm.setConfigPortalBlocking(false);
//wm.setConfigPortalTimeout(60);

if(wm.autoConnect("GAS || CONFIG PORTAL")){
    Serial.println("connected...yeey :)");
}
else {
    Serial.println("Configportal running");
}

Blynk.config(auth);
Blynk.connect();

// Once the Wi-Fi configuration is done, show the appropriate message
display.clearDisplay();
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(10, 0);
display.println("OTA Configured!");
display.display();

warmUpStartTime = millis(); 
display.clearDisplay();
printTitle();  
display.display();
}

void loop() {
wm.process();  // Handle WiFiManager process
Blynk.run();

int rawVal = analogRead(MQ6_PIN);  // Read MQ6 sensor value
int ppm = map(rawVal, 0, 1023, 0, 10000);  // Map value to PPM 

// Print raw value and PPM to Serial Monitor for debugging
Serial.print("Raw: ");
Serial.print(rawVal);
Serial.print(" | Approx PPM: ");
Serial.println(ppm);

// Clear the display and re-print title
display.clearDisplay();
printTitle();

unsigned long currentTime = millis();
if (currentTime - warmUpStartTime < warmUpDuration) {  
printWarming(ppm);
digitalWrite(buzzer, LOW);
} else {  
printGasValue(ppm);
printGasLevel(ppm);
updateBlynk(ppm);    
sendIFTTT(ppm);      
}

display.display();  
delay(500); 
}

// === Display Functions ===

void printTitle() {
display.setTextSize(1);
display.setTextColor(WHITE);
display.setCursor(22, 0);
display.println("Gas Leak Detector");

display.setCursor(5, 15);
display.println(name);
}

void printWarming(int ppm) {
display.setTextSize(1);
display.setCursor(0, 35);
display.println("Warming up...");

display.setCursor(0, 50);
display.print("Sensor: ");
display.print(ppm);
display.print(" ppm");
}

void printGasValue(int ppm) {
display.setTextSize(2);
display.setCursor(20, 25);
display.print(ppm);
display.print(" ppm");
}

void printGasLevel(int ppm) {
display.setTextSize(1);
display.setCursor(10, 55);

if (ppm < SAFE_LEVEL) {
display.println("Air is clean.");
digitalWrite(buzzer, LOW);  // No alarm for safe levels
} else if (ppm < WARNING_LEVEL) {
display.println("Gas Detected!");
beepBuzzer();  // Short beep for detected gas
} else if (ppm < DANGER_LEVEL) {
display.println("Warning! Gas High");
beepBuzzer();  // Short beep for warning levels
} else {
display.println("DANGER! Gas Leak!");
continuousBeep();  // Continuous beep for dangerous levels
}
}

// === Buzzer Functions ===

void beepBuzzer() {
digitalWrite(buzzer, HIGH);
delay(100);  // Short beep
digitalWrite(buzzer, LOW);
}

void continuousBeep() {
digitalWrite(buzzer, HIGH);  // Continuous beep
}

// === Blynk Update ===

void updateBlynk(int ppm) {
Blynk.virtualWrite(V0, ppm);
}

// === IFTTT Webhook ===

void sendIFTTT(int ppm) {
if (WiFi.status() != WL_CONNECTED) return;

GasLevelCategory currentLevel;

if (ppm < SAFE_LEVEL) {
currentLevel = LEVEL_SAFE;
} else if (ppm < WARNING_LEVEL) {
currentLevel = LEVEL_DETECTED;
} else if (ppm < DANGER_LEVEL) {
currentLevel = LEVEL_WARNING;
} else {
currentLevel = LEVEL_DANGER;
}

if (currentLevel != lastLevel) { 
HTTPClient http;
String url = "http://maker.ifttt.com/trigger/" + IFTTT_EVENT + "/with/key/" + IFTTT_KEY;
String value1, value2 = String(ppm), value3;

switch (currentLevel) {
    case LEVEL_SAFE:
    value1 = "Safe Air";
    value3 = "Air is clean. No action needed.";
    break;
    case LEVEL_DETECTED:
    value1 = "Gas Detected!";
    value3 = "Ventilate the room.";
    break;
    case LEVEL_WARNING:
    value1 = "WARNING!";
    value3 = "Evacuate area. Avoid sparks.";
    break;
    case LEVEL_DANGER:
    value1 = "DANGER!";
    value3 = "Major leak! Call emergency services.";
    break;
}
 
String payload = "{\"value1\":\"" + value1 + "\",\"value2\":\"" + value2 + "\",\"value3\":\"" + value3 + "\"}";
http.begin(url);
http.addHeader("Content-Type", "application/json");
int httpResponseCode = http.POST(payload);
Serial.print("IFTTT Response: ");
Serial.println(httpResponseCode);
http.end();

lastLevel = currentLevel;
}
}