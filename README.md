# ⚙️ Panduan Konfigurasi Parameter ESP32 & ThingSpeak

> ⚠️ **PENTING: Konfigurasi Kredensial Sebelum Menjalankan Program**  
> Sesuaikan parameter jaringan Wi-Fi dan kredensial ThingSpeak pada potongan kode berikut agar ESP32 dapat terhubung ke internet serta mengirimkan data telemetri dengan sukses.

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
```

---

## 📋 Tabel Referensi Parameter

| Nama Variabel | Tipe Data | Format Penulisan | Contoh Nilai | Deskripsi & Lokasi Sumber |
| :--- | :--- | :--- | :--- | :--- |
| `ssid` | `const char*` | Teks diapit tanda kutip dua (`"..."`) | `"Rumah_WiFi_2.4G"` | Nama SSID Wi-Fi atau Hotspot HP Anda. |
| `password` | `const char*` | Teks diapit tanda kutip dua (`"..."`) | `"rahasia123"` | Kata sandi jaringan Wi-Fi yang digunakan. |
| `myChannelNumber` | `unsigned long` | Angka murni **tanpa** tanda kutip | `3497061` | ID unik channel dari ThingSpeak. |
| `myWriteAPIKey` | `const char*` | Teks diapit tanda kutip dua (`"..."`) | `"CZKQROMMYOKIXFXU"` | Kunci otorisasi pengiriman data (Write API Key). |

---

## 🔍 Langkah Mendapatkan Kredensial ThingSpeak

Jika Anda belum memiliki atau ingin memeriksa kembali *Channel ID* dan *Write API Key*, ikuti langkah berikut:

### 1. Mendapatkan `myChannelNumber` (Channel ID)
1. Buka dan masuk ke akun Anda di [ThingSpeak.com](https://thingspeak.com/).
2. Klik menu **Channels** di bilah navigasi atas, lalu pilih **My Channels**.
3. Klik channel monitoring suhu yang telah Anda buat.
4. Perhatikan bagian atas halaman tepat di bawah judul channel:
   * Terdapat tulisan **Channel ID: XXXXXXX** (misal: `3497061`).
   * Salin angka tersebut dan masukkan ke variabel `myChannelNumber` (tanpa tanda kutip).

### 2. Mendapatkan `myWriteAPIKey` (Write API Key)
1. Pada halaman channel yang sama, klik tab **API Keys**.
2. Perhatikan kolom **Write API Key**:
   * Akan tertera 16 karakter alfanumerik (kombinasi huruf besar dan angka).
   * Klik ikon salin (*copy*) atau tandai teks tersebut.
   * Tempelkan ke variabel `myWriteAPIKey` dengan tetap menyertakan tanda kutip dua (`"..."`).

---

## 🌐 Pengaturan Mode Jaringan Wi-Fi

### A. Jika Menggunakan Perangkat Fisik ESP32
* **Wajib Frekuensi 2.4 GHz:** Pastikan router Wi-Fi atau hotspot ponsel diatur ke pita frekuensi **2.4 GHz**, karena modul ESP32 standar tidak mendukung jaringan **5 GHz**.
* **Contoh Kode:**
  ```cpp
  const char* ssid     = "Hotspot-Ponsel-Saya";
  const char* password = "password12345";
  ```

### B. Jika Menggunakan Simulator Wokwi
* Simulator Wokwi menyediakan gateway virtual internet bawaan bernama **`Wokwi-GUEST`** tanpa kata sandi.
* **Contoh Kode:**
  ```cpp
  const char* ssid     = "Wokwi-GUEST";
  const char* password = "";
  ```

---

## 💡 Hal Penting & Troubleshooting

* ⏱️ **Batas Waktu Pengiriman (Rate Limit):** ThingSpeak akun gratis membatasi frekuensi pembaruan data minimal **15 detik sekali**. Pengiriman data lebih cepat dari 15 detik akan otomatis diabaikan (*ignored*) oleh server.
* 🔠 **Sensitivitas Huruf (Case-Sensitive):** Nama Wi-Fi, password, dan API Key membedakan huruf besar dan kecil (*case-sensitive*). Pastikan tidak ada spasi yang tidak sengaja terbawa di awal atau akhir teks.
* 🚫 **Jangan Tertukar dengan Read API Key:** Gunakan **Write API Key** untuk mengirim/menulis data ke server, bukan *Read API Key*.
