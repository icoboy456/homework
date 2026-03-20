const int ledPin = 26;       // LED连接的引脚
const unsigned long interval = 500;  // 亮/灭的持续时间（500ms，对应1Hz）
unsigned long previousMillis = 0;    // 记录上一次状态切换的时间
bool ledState = LOW;                // LED当前状态（初始熄灭）

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);   // 初始化LED状态
}

void loop() {
  // 获取当前时间（毫秒）
  unsigned long currentMillis = millis();

  // 检查是否达到状态切换的时间
  if (currentMillis - previousMillis >= interval) {
    // 更新上一次切换时间
    previousMillis = currentMillis;
    
    // 切换LED状态（亮→灭，灭→亮）
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    
    // 串口输出当前状态，方便调试
    if (ledState == HIGH) {
      Serial.println("LED ON");
    } else {
      Serial.println("LED OFF");
    }
  }

  // 这里可以加其他代码，不会被闪烁逻辑阻塞
  // 比如：Serial.println("程序正常运行中...");
}