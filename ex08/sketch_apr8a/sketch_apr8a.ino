#include <WiFi.h>
#include <WebServer.h>

// ====================== 请修改这里的WiFi ======================
const char* ssid = "R";
const char* password = "12345678";
// ==============================================================

WebServer server(80);

const int LED_PIN = 2;
const int TOUCH_PIN = T0;  // D4 触摸引脚

bool isArmed = false;
bool isAlarming = false;

const char* html = R"HTML(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>ESP32 安防报警器</title>
<style>
body{text-align:center;font-size:22px;margin-top:50px;}
button{
  width:200px;height:70px;font-size:24px;margin:20px;
  border:none;border-radius:10px;cursor:pointer;
}
.btn-arm{background:#ff4444;color:white;}
.btn-disarm{background:#00C851;color:white;}
.status{font-size:26px;margin:30px 0;}
</style>
</head>
<body>
  <h1>ESP32 物联网安防报警器</h1>
  <div class="status">
    布防状态：%STATE%<br>
    报警状态：%ALARM%
  </div>
  <a href="/arm"><button class="btn-arm">布防 (Arm)</button></a>
  <a href="/disarm"><button class="btn-disarm">撤防 (Disarm)</button></a>
</body>
</html>
)HTML";

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi 连接成功！");
  Serial.print("IP：");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", armSystem);
  server.on("/disarm", disarmSystem);
  server.begin();
}

void loop() {
  server.handleClient();

  if (isArmed && !isAlarming) {
    if (touchRead(TOUCH_PIN) < 30) {
      isAlarming = true;
      Serial.println("触发报警！");
    }
  }

  if (isAlarming) {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void handleRoot() {
  String page = html;
  page.replace("%STATE%", isArmed ? "已布防" : "未布防");
  page.replace("%ALARM%", isAlarming ? "⚠报警中" : "正常");
  server.send(200, "text/html", page);
}

// 布防（修复版）
void armSystem() {
  isArmed = true;
  isAlarming = false;
  server.sendHeader("Location", "/");
  server.send(302);
}

// 撤防（修复版）
void disarmSystem() {
  isArmed = false;
  isAlarming = false;
  server.sendHeader("Location", "/");
  server.send(302);
}