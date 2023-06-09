#include <WiFi.h>
  #include <FirebaseESP32.h>
  #include <DHT.h>
  
  #define WIFI_SSID "nama-wifi"
  #define WIFI_PASSWORD "password-wifi"
  
  #define FIREBASE_HOST "alamat-database.firebaseio.com"
  #define FIREBASE_AUTH "token-autentikasi"
  
  #define DHTPIN 2     // Pin data sensor DHT11 terhubung ke pin GPIO2 (D2)
  #define DHTTYPE DHT11   // Tipe sensor DHT11
  
  DHT dht(DHTPIN, DHTTYPE);
  FirebaseData firebaseData;
  
  void setup() {
    Serial.begin(9600);
    Serial.println("Menggunakan sensor DHT11...");
  
    // Koneksi ke WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Tidak terhubung ke WiFi...");
    }
    Serial.println("Terhubung ke WiFi...");
  
    // Inisialisasi Firebase
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Serial.println("Terhubung ke Firebase...");
    
    dht.begin();
  }
  
  void loop() {
    delay(2000);  // Tunggu 2 detik antara pembacaan
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();
    // Periksa jika membaca sukses
    if (isnan(humidity) || isnan(temperature)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      return;
    }
  
    // Tampilkan hasil pembacaan suhu dan kelembaban
    Serial.print("Kelembaban: ");
    Serial.print(humidity);
    Serial.print("%\t");
    Serial.print("Suhu: ");
    Serial.print(temperature);
    Serial.print("°C\t");
  
    // Kirim data ke Firebase
    firebaseData.setFloat("kelembaban", humidity);
    firebaseData.setFloat("suhu", temperature);
    firebaseData.setFloat("suhuF", temperatureF);
    if (Firebase.pushFloat(firebaseData, "/dht11")) {
      Serial.println("Data berhasil dikirim ke Firebase.");
    } else {
      Serial.println("Gagal mengirim data ke Firebase!");
      Serial.println(firebaseData.errorReason());
    }
  }
