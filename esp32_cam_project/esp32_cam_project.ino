#include <WiFi.h>
#include "config.h"
#include "camera_module.h"
#include "web_server.h"

// Biến debounce nút bấm
static unsigned long lastDebounceTime = 0;
static bool lastButtonState = HIGH;

//-----------------------------------------------------------------------------------------//
void setup() 
{
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  //check config camera
  if (!initCamera()) 
  {
    return;
  }
  
  //connect to wifi
  WiFi.begin(ssid, password);
  Serial.print("Dang ket noi WiFi");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Da ket noi! Truy cap web tai: http://");
  Serial.println(WiFi.localIP());

  //web server
  setupWebServer();
}

//-----------------------------------------------------------------------------------------//
void loop() 
{
  handleWebServer();

  bool reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState) 
  {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) 
  {
    if (reading == LOW) 
    {
      capturePhoto();
      delay(300);
    }
  }

  lastButtonState = reading;
}