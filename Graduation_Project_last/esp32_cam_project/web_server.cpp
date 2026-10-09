#include "web_server.h"
#include <WebServer.h>
#include "camera_module.h"

static WebServer server(80);

static void handleRoot() {
  String html = "<html>\
<head>\
<meta name='viewport' content='width=device-width, initial-scale=1'>\
<title>ESP32-CAM Photo</title>\
<style>\
body {font-family: Arial; text-align: center; background: #f4f4f4;}\
h2 {color: #333;}\
img {max-width: 95%; border: 2px solid #333; margin-top: 15px; border-radius: 6px;}\
</style>\
</head>\
<body>\
<h2>Anh chup gan nhat</h2>";

  if (getLastPhotoLen() > 0) {
    html += "<img src='/photo?" + String(millis()) + "'>";
  } else {
    html += "<p>Chua co anh nao. Bam nut de chup.</p>";
  }

  html += "<script>setTimeout(function(){location.reload();}, 3000);</script>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

static void handlePhoto() {
  if (getLastPhotoLen() == 0) {
    server.send(404, "text/plain", "Chua co anh");
    return;
  }
  server.send_P(200, "image/jpeg", (const char *)getLastPhoto(), getLastPhotoLen());
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/photo", handlePhoto);
  server.begin();
}

void handleWebServer() {
  server.handleClient();
}