#include <WiFi.h>
#include <WebServer.h>

// ==================== 修改你的WiFi ====================
const char* ssid = "R";
const char* password = "12345678";
// ======================================================

WebServer server(80);

// 触摸引脚 D4 = T0
const int TOUCH_PIN = T0;

// 主页面（带AJAX实时刷新）
const char* htmlPage = R"HTML(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>实时传感器Web仪表盘</title>
    <style>
        body {
            text-align: center;
            font-family: Arial;
            background: #1a1a1a;
            color: white;
            margin-top: 100px;
        }
        .title {
            font-size: 32px;
            margin-bottom: 40px;
            color: #00eaff;
        }
        .value {
            font-size: 80px;
            font-weight: bold;
            color: #ff9500;
        }
        .tip {
            margin-top: 30px;
            font-size: 18px;
            color: #ccc;
        }
    </style>
</head>
<body>
    <div class="title">ESP32 实时触摸传感器数值</div>
    <div class="value" id="touchValue">0</div>
    <div class="tip">手指靠近 → 数值变小 | 手指离开 → 数值恢复</div>

    <script>
        // AJAX 每 100ms 请求一次传感器数据
        function updateValue() {
            var xhr = new XMLHttpRequest();
            xhr.open('GET', '/sensor', true);
            xhr.onreadystatechange = function() {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    document.getElementById('touchValue').innerText = xhr.responseText;
                }
            };
            xhr.send();
        }
        // 定时刷新
        setInterval(updateValue, 100);
    </script>
</body>
</html>
)HTML";

void setup() {
  Serial.begin(115200);
  
  // 连接WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi 连接成功！");
  Serial.print("访问地址：");
  Serial.println(WiFi.localIP());

  // 路由
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  // 实时返回传感器数值（供AJAX获取）
  server.on("/sensor", []() {
    int val = touchRead(TOUCH_PIN);
    server.send(200, "text/plain", String(val));
  });

  server.begin();
}

void loop() {
  server.handleClient();
}