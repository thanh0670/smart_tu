#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>



#ifndef APSSID
#define APSSID "Free"  
#define APPSK "12345678" 
#define SS_PIN D8   // Chân chọn cho slave (SDA cho RFID)
#define RST_PIN D3  // Chân reset cho RFID
#endif

const char *ssid = APSSID;
const char *password = APPSK;
const char *URL = "http://192.168.208.56:8000/setCard"; 
const char *DeniedURL = "http://192.168.208.56:8000/setDeniedCard"; 

// Khởi tạo các đối tượng
WiFiClient client;
HTTPClient http;



MFRC522 mfrc522(SS_PIN, RST_PIN);  // Khởi tạo đối tượng đầu đọc thẻ RFID
LiquidCrystal_I2C lcd(0x27, 16, 2); // Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F)

// Các UID mặc định để mở khóa
const String defaultUID1 = "5350A4E2";
const String defaultUID2 = "E31874E4";
const String defaultUID3 = "A71E0963";
const String defaultUID4 = "B72F956C";



void setup() {
  Serial.begin(9600);        // Khởi tạo giao tiếp nối tiếp
  SPI.begin();               // Khởi tạo giao tiếp SPI cho RFID
  mfrc522.PCD_Init();        // Khởi tạo đầu đọc thẻ MFRC522
  
  lcd.begin();               // Khởi tạo màn hình LCD
  lcd.backlight();           // Bật đèn nền LCD
  lcd.print("RFID Ready");
  delay(2000);               // Chờ để người dùng thấy thông báo khởi động

  Serial.print("Kết nối đến mạng Wifi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nĐã kết nối với WiFi.");

  lcd.clear();               // Xóa màn hình sau thông báo khởi động
}

void loop() {
  // Kiểm tra xem có thẻ mới nào không
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Hiển thị UID của thẻ trên Serial Monitor
  Serial.print("UID của thẻ: ");
  String rfidUID = "";
  
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    // In từng byte của UID dưới dạng hex
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    rfidUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    rfidUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Chuyển đổi UID thành chữ in hoa
  rfidUID.toUpperCase();

  // So sánh UID đã đọc với các UID mặc định
  lcd.clear();
  if (rfidUID == defaultUID1 || rfidUID == defaultUID2 || rfidUID == defaultUID3 ) {
    lcd.setCursor(0, 0); // Đặt con trỏ tại đầu dòng đầu tiên
    lcd.print("Access Granted");
    lcd.setCursor(0, 1);
    lcd.print(rfidUID);
    Serial.println("Access Granted");
    postJsonData(rfidUID);
  } else {
    lcd.setCursor(0, 0); // Đặt con trỏ tại đầu dòng đầu tiên
    lcd.print("Access Denied");
    lcd.setCursor(0, 1); // Đặt con trỏ tại đầu dòng thứ hai
    Serial.println("Access Denied");
    lcd.print(rfidUID);  // Hiển thị UID bị từ chối
    postDenied(rfidUID);
  }

  // Ngắt kết nối thẻ đã quét và ngừng mã hóa
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();

  delay(2000);
}



void postDenied(String data) {
  if (http.begin(client, DeniedURL)) {
    DynamicJsonDocument doc(1024);
    doc["UID"] = data; 

    String output;
    serializeJson(doc, output);

    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(output);

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("respone from server:");
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("cannot HTTP connect");
  }
}

void postJsonData(String data) {
  if (http.begin(client, URL)) {
    DynamicJsonDocument doc(1024);
    doc["UID"] = data; 

    String output;
    serializeJson(doc, output);

    http.addHeader("Content-Type", "application/json");
    int httpCode = http.POST(output);

    if (httpCode > 0) {
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      String payload = http.getString();
      Serial.println("respone from server:");
      Serial.println(payload);
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("cannot HTTP connect");
  }
}