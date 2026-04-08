#include <WiFi.h>
#include <WebServer.h>

// ====================== 请修改这里的 WiFi 信息 ======================
const char* ssid = "R";
const char* password = "12345678";
// ===================================================================

WebServer server(80);

// LED 接 D2
const int LED_PIN = 2;
int brightness = 0;

// 网页HTML
String getHTML() {
  String html = "<!DOCTYPE html>";
  html += "<html lang='zh-CN'>";
  html += "<head><meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP32 无极调光</title>";
  html += "<style>body{text-align:center; margin-top:50px;}input{width:80%; height:30px;}</style>";
  html += "</head><body>";
  html += "<h2>ESP32 LED 调光</h2>";
  html += "<input type='range' min='0' max='255' value='"+String(brightness)+"' oninput='setBrightness(this.value)'>";
  html += "<p>亮度：<span id='val'>"+String(brightness)+"</span></p>";
  html += "<script>";
  html += "function setBrightness(val){";
  html += "  document.getElementById('val').innerText=val;";
  html += "  fetch('/set?b='+val);";
  html += "}";
  html += "</script></body></html>";
  return html;
}

void handleRoot() {
  server.send(200, "text/html", getHTML());
}

void handleSet() {
  if (server.hasArg("b")) {
    brightness = server.arg("b").toInt();
    analogWrite(LED_PIN, brightness); // 直接用 analogWrite 兼容所有版本！
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  analogWrite(LED_PIN, 0);

  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 连接成功");
  Serial.println("IP: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/set", handleSet);
  server.begin();
}

void loop() {
  server.handleClient();
}