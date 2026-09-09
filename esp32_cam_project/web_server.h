#ifndef WEB_SERVER_H
#define WEB_SERVER_H

// Khởi tạo route và bắt đầu web server (gọi trong setup(), sau khi đã có WiFi)
void setupWebServer();

// Xử lý client (gọi liên tục trong loop())
void handleWebServer();

#endif // WEB_SERVER_H