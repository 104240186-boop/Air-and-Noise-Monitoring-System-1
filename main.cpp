/******************************************************** * SMART AIR QUALITY & NOISE MONITORING SYSTEM 
 * ESP32 + MQ135 + MKE-S06 + Blynk
 ********************************************************/

//================ BLYNK ==================
#define BLYNK_TEMPLATE_ID "TMPL6qg2_fIwA"
#define BLYNK_TEMPLATE_NAME "portable air and noise device"
#define BLYNK_AUTH_TOKEN "ou6mqSVs8e3ML2enrSauLh6Kaw4YvETb"
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <math.h>

//================ WIFI ==================
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "VGU_Student_Guest";
char pass[] = "";

//================ SENSOR PINS & CONSTANTS ==================
#define GAS_SENSOR_PIN     35
#define NOISE_SENSOR_PIN   34

#define RL 10.0
#define R0 76.63       
#define MQ_M -0.417      
#define MQ_B 0.858      

//================ BLYNK VIRTUAL PINS ==================
#define V0_GAS   V0
#define V1_NOISE V1

//================ VARIABLES ==================
float co2_ppm = 0;
float noise_level_db = 0;

//================ ALERT THRESHOLDS ==================
const float CO2_ALERT_THRESHOLD = 1000;
const float NOISE_ALERT_THRESHOLD = 85;
bool gas_alert_sent = false;
bool noise_alert_sent = false;

BlynkTimer timer;

//====================================================
// MQ135 GAS SENSOR FUNCTION (Logarithmic Formula)
//====================================================
float readMQ135() {

  int adc = 0;

for (int i = 0; i < 20; i++) {
    adc += analogRead(GAS_SENSOR_PIN);
    delay(5);
}

adc /= 20;
  if (adc <= 0) adc = 1;

  float voltage = adc * (3.3 / 4095.0);

  if (voltage < 0.1) voltage = 0.1;

  float Rs = ((3.3 - voltage) / voltage) * RL;

  float ratio = Rs / R0;
  float ppm = 116.6020682 * pow(ratio, -2.769034857);

  ppm *= 90;
  return ppm;
}
//====================================================
// NOISE SENSOR FUNCTION (dB estimation)
//====================================================
float readNoise() {
  int signalMax = 0;
  int signalMin = 4095;
  unsigned long startMillis = millis();
  
  while (millis() - startMillis < 100) {
    int sample = analogRead(NOISE_SENSOR_PIN);
    if (sample > signalMax) signalMax = sample;
    if (sample < signalMin) signalMin = sample;
  }
  
  int peakToPeak = signalMax - signalMin;
  if (peakToPeak < 1) peakToPeak = 1;
  
  float db = 20 * log10((float)peakToPeak);
  return db;
}

//====================================================
// MAIN SENSOR TASK
//====================================================
void readSensorData() {
  Serial.println("--------------------------------");
  
  // Đọc dữ liệu từ các cảm biến
  co2_ppm = readMQ135();
  noise_level_db = readNoise();

  //================ DISPLAY TO SERIAL ==================
  Serial.print("CO2 (MQ135): ");
  Serial.print(co2_ppm);
  Serial.print(" ppm");
  
  if (co2_ppm < 800) Serial.println(" -> Good Air");
  else if (co2_ppm < 1200) Serial.println(" -> Moderate Air");
  else if (co2_ppm < 2000) Serial.println(" -> Poor Air");
  else Serial.println(" -> Dangerous Air");
  
  Serial.print("Noise Level: ");
  Serial.print(noise_level_db);
  Serial.println(" dB");

  //================ BLYNK SEND ==================
  if (Blynk.connected()) {
    Blynk.virtualWrite(V0, co2_ppm);
    Blynk.virtualWrite(V1, noise_level_db);
  }

  //================ CO2 ALERT ==================
  if (co2_ppm > CO2_ALERT_THRESHOLD && !gas_alert_sent) {
    Serial.println("WARNING: High CO2 Level!");
    gas_alert_sent = true;
  }
  if (co2_ppm < 900) gas_alert_sent = false;

  //================ NOISE ALERT ==================
  if (noise_level_db > NOISE_ALERT_THRESHOLD && !noise_alert_sent) {
    Serial.println("WARNING: High Noise Level!");
    noise_alert_sent = true;
  }
  if (noise_level_db < 80) noise_alert_sent = false;
  
  Serial.println("--------------------------------");
}

//====================================================
// SETUP
//====================================================
void setup() {
  Serial.begin(115200);
  
  analogReadResolution(12);
  
  pinMode(GAS_SENSOR_PIN, INPUT);
  pinMode(NOISE_SENSOR_PIN, INPUT);
  
  Serial.println("SMART AIR & NOISE SYSTEM STARTED");
  
  Blynk.begin(auth, ssid, pass);
  
  timer.setInterval(500L, readSensorData);
}

//====================================================
// LOOP
//====================================================
void loop() {
  Blynk.run();
  timer.run();
}