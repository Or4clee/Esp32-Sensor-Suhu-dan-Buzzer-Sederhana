Jangan Lupa Untuk Mengkonfigurasi Kode Berikut


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
