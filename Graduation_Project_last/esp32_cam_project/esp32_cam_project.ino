#include <WiFi.h>
#include "config.h"
#include "camera_module.h"
#include "web_server.h"

// Biến debounce nút bấm
static unsigned long lastDebounceTime = 0;
static bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP); // nut noi xuong GND khi bam
  // ----- Cấu hình camera -----
  if (!initCamera()) {
    return;
  }
  // ----- Kết nối WiFi -----
  WiFi.begin(ssid, password);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Da ket noi! Truy cap web tai: http://");
  Serial.println(WiFi.localIP());

  // ----- Khởi động web server -----
  setupWebServer();
}

void loop() {
  handleWebServer();

  // Đọc nút bấm với debounce
  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading == LOW) { // nút bấm nối GND khi nhấn
      capturePhoto();
      delay(300); // tránh chụp liên tục khi giữ nút
    }
  }
  lastButtonState = reading;
}