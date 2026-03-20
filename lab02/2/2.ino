const int ledPin = 26;       // LED连接的引脚
unsigned long previousMillis = 0;  // 记录上一次状态切换的时间
int currentState = 0;        // 当前SOS状态机的步骤

// 定义SOS的每个阶段：[亮/灭, 持续时间(ms)]
// 0:短亮,1:短灭,2:短亮,3:短灭,4:短亮,5:短灭,
// 6:长亮,7:长灭,8:长亮,9:长灭,10:长亮,11:长灭,
// 12:短亮,13:短灭,14:短亮,15:短灭,16:短亮,17:长灭(序列结束停顿)
int sosSequence[18][2] = {
  {HIGH, 500},  {LOW, 500},  {HIGH, 500},  {LOW, 500},
  {HIGH, 500},  {LOW, 500},  {HIGH, 1500}, {LOW, 500},
  {HIGH, 1500}, {LOW, 500},  {HIGH, 1500}, {LOW, 500},
  {HIGH, 500},  {LOW, 500},  {HIGH, 500}, {LOW, 500},
  {HIGH, 500},  {LOW, 3000}
};

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); // 初始熄灭
}

void loop() {
  unsigned long currentMillis = millis();

  // 检查是否到达当前阶段的切换时间
  if (currentMillis - previousMillis >= sosSequence[currentState][1]) {
    // 更新时间戳
    previousMillis = currentMillis;
    
    // 执行当前阶段的LED状态
    digitalWrite(ledPin, sosSequence[currentState][0]);
    
    // 串口输出当前状态（方便调试）
    Serial.print("状态");
    Serial.print(currentState);
    Serial.print(": ");
    Serial.println(sosSequence[currentState][0] ? "LED ON" : "LED OFF");
    
    // 切换到下一个状态，循环执行SOS
    currentState++;
    if (currentState >= 18) { // 走完所有18个阶段后重置
      currentState = 0;
      Serial.println("=== SOS序列结束，等待后重复 ===");
    }
  }

  // 这里可以添加其他代码，不会被SOS闪烁阻塞
  // 例如：Serial.println("程序正常运行...");
}