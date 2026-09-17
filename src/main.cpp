#include <Arduino.h>  // Wajib jika menggunakan PlatformIO di VS Code
#include <WiFi.h>
#include "DHT.h"
#include "ThingSpeak.h"

// ==========================================
// 1. Konfigurasi Pin Komponen
// ==========================================
#define DHTPIN 4
#define DHTTYPE DHT22
#define LED_MERAH 18
#define LED_HIJAU 19
#define BUZZER 23

// // ==========================================
// // 2. Konfigurasi Wi-Fi
// // ==========================================
const char* ssid = "";
const char* password = "";

// ==========================================
// 3. Konfigurasi ThingSpeak
// ==========================================
WiFiClient client;
unsigned long myChannelNumber = 12345678;  // Ganti dengan nomor channel ThingSpeak Anda  
const char* myWriteAPIKey = "XXXXXXXXX";  // Ganti dengan Write API Key ThingSpeak Anda

// Inisialisasi Sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// ==========================================
// 4. Variabel Timer Non-Blocking (millis)
// ==========================================
unsigned long previousMillisThingSpeak = 0;
const unsigned long intervalThingSpeak = 15000; // 15 detik sekali

unsigned long previousMillisAlarm = 0;
const unsigned long intervalAlarm = 250; // Tempo kedip & bunyi (250 ms)
bool alarmState = false;

// Variabel penampung nilai sensor
float temperature = 0.0;
float humidity = 0.0;

// Fungsi Menghubungkan ke Wi-Fi Wokwi
void setupWiFi() {
  Serial.println();
  Serial.print("Connecting to Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);

  // Atur mode pin output
  pinMode(LED_MERAH, OUTPUT);
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Pastikan kondisi awal mati menggunakan digitalWrite
  digitalWrite(LED_MERAH, LOW);
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(BUZZER, LOW);

  // Inisialisasi DHT22
  dht.begin();

  // Koneksi ke Wi-Fi
  setupWiFi();

  // Inisialisasi ThingSpeak
  ThingSpeak.begin(client);
  
  Serial.println("\n--- Sistem Siap & Monitoring Berjalan ---");
}

void loop() {
  unsigned long currentMillis = millis();

  // ----------------------------------------------------
  // A. Pembacaan Sensor DHT22
  // ----------------------------------------------------
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Cek apakah pembacaan valid (bukan NaN)
  if (!isnan(t) && !isnan(h)) {
    temperature = t;
    humidity = h;
  }

  // ----------------------------------------------------
  // B. Logika Indikator Suhu (Bahaya vs Aman)
  // ----------------------------------------------------
  if (temperature > 30.0) {
    // KONDISI BAHAYA (> 30°C):
    digitalWrite(LED_HIJAU, LOW); // LED Hijau padam

    // Bunyi Buzzer dan Kedip LED Merah Intermiten
    if (currentMillis - previousMillisAlarm >= intervalAlarm) {
      previousMillisAlarm = currentMillis;
      alarmState = !alarmState;

      digitalWrite(LED_MERAH, alarmState ? HIGH : LOW);
      digitalWrite(BUZZER, alarmState ? HIGH : LOW); // Menggunakan sinyal digital murni
    }
  } else {
    // KONDISI AMAN (<= 30°C):
    digitalWrite(LED_MERAH, LOW);
    digitalWrite(BUZZER, LOW);    // Matikan buzzer tanpa memicu error LEDC
    digitalWrite(LED_HIJAU, HIGH); // LED Hijau menyala
  }

  // ----------------------------------------------------
  // C. Cetak ke Serial Monitor & Kirim ke ThingSpeak
  //    (Dijalankan rutin setiap 15 detik)
  // ----------------------------------------------------
  if (currentMillis - previousMillisThingSpeak >= intervalThingSpeak) {
    previousMillisThingSpeak = currentMillis;

    // Output ke Serial Monitor
    Serial.println("\n----------------------------------------");
    Serial.print("Suhu        : ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Kelembapan  : ");
    Serial.print(humidity);
    Serial.println(" %");

    if (temperature > 30.0) {
      Serial.println("Status      : [PERINGATAN] Suhu Panas! Kipas Aktif!");
    } else {
      Serial.println("Status      : [NORMAL] Suhu Aman.");
    }

    // Set nilai untuk ThingSpeak (Field 1: Suhu, Field 2: Kelembapan)
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);

    // Kirim data ke Cloud ThingSpeak
    int responseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (responseCode == 200) {
      Serial.println(">> Status IoT : Data berhasil terkirim ke ThingSpeak!");
    } else {
      Serial.print(">> Status IoT : Pengiriman data gagal (Kode HTTP: ");
      Serial.print(responseCode);
      Serial.println(")");
    }
  }
}