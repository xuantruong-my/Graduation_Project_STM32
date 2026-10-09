#include "web_server.h"
#include <WebServer.h>
#include "camera_module.h"

//HTTP Web Server on port 80, example: https://192.168.1.100 = https://192.168.1.100:80
static WebServer server(80);

//-----------------------------------------------------------------------------------------//
static void handleRoot() 
{
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

  if (getLastPhotoLen() > 0)     //check whether there is an image
  {
    html += "<img src='/photo?" + String(millis()) + "'>";        //get img from /photo?... and display on screen
  } 
  else
  {
    html += "<p>Chua co anh nao. Bam nut de chup.</p>";
  }

  html += "<script>setTimeout(function(){location.reload();}, 3000);</script>";   //reload page every 3s
  html += "</body></html>";

  server.send(200, "text/html", html);
}

//-----------------------------------------------------------------------------------------//
static void handlePhoto()
{
  if (getLastPhotoLen() == 0)     //check whether there isn't an image
  {
    server.send(404, "text/plain", "Chua co anh");        //404 not found, print text "Chua co anh"
    return;
  }

  //200 -> success(request)
  //image/jpeg -> image(JPEG, standard of ESP32-CAM library)
  //server.send_P (HTTP response code, type, start_addr, length)
  server.send_P(200, "image/jpeg", (const char *)getLastPhoto(), getLastPhotoLen());
}

//-----------------------------------------------------------------------------------------//
void setupWebServer() 
{
  server.on("/", handleRoot);                 //server.on("path", function);
  server.on("/photo", handlePhoto);
  server.begin();                             //run server
}

//-----------------------------------------------------------------------------------------//
void handleWebServer() 
{
  server.handleClient();        //handle 2 request, such as: GET / and GET /photo
}