> ⚠️ **PENTING: Konfigurasi Kredensial Sebelum Menjalankan Program**
> 
> Sesuaikan parameter jaringan dan kredensial ThingSpeak pada potongan kode berikut agar ESP32 dapat terhubung ke internet serta mengirimkan data telemetri dengan sukses.

```cpp
// ==========================================
// 1. Konfigurasi Jaringan Wi-Fi
// ==========================================
const char* ssid     = "NAMA_WIFI_ANDA";     // Nama SSID Wi-Fi / Hotspot
const char* password = "PASSWORD_WIFI_ANDA"; // Kata sandi Wi-Fi

// ==========================================
// 2. Konfigurasi Server ThingSpeak
// ==========================================
WiFiClient client;
unsigned long myChannelNumber = 12345678;        // Nomor Channel ID ThingSpeak Anda
const char* myWriteAPIKey     = "XXXXXXXXXXXX";  // Write API Key dari ThingSpeak
