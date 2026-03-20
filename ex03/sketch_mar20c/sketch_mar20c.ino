// 定义两个LED引脚（26号渐亮/灭，25号反向）
const int ledPin_26 = 26;  
const int ledPin_25 = 25;  

// 设置PWM属性
const int freq = 5000;          // 频率 5000Hz
const int resolution = 8;       // 分辨率 8位 (0-255)

// 非阻塞相关变量
unsigned long previousMillis = 0;  // 记录上一次亮度变化的时间
const long interval = 10;          // 亮度变化间隔（毫秒）
int dutyCycle_26 = 0;              // 26号引脚当前亮度
bool fadingUp = true;              // 标记：26号变亮（true）/ 变暗（false）

void setup() {
  Serial.begin(115200);
  // 绑定两个引脚的PWM参数
  ledcAttach(ledPin_26, freq, resolution);
  ledcAttach(ledPin_25, freq, resolution);
}

void loop() {
  // 获取当前时间
  unsigned long currentMillis = millis();

  // 非阻塞亮度调整逻辑
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 调整26号引脚亮度
    if (fadingUp) {
      dutyCycle_26++;
      // 26号到最大值，切换为变暗阶段
      if (dutyCycle_26 >= 255) {
        fadingUp = false;
      }
    } else {
      dutyCycle_26--;
      // 26号到最小值，切换为变亮阶段，打印循环完成
      if (dutyCycle_26 <= 0) {
        fadingUp = true;
        Serial.println("Breathing cycle completed");
      }
    }

    // 核心：25号引脚亮度 = 255 - 26号引脚亮度（反向互补）
    int dutyCycle_25 = 255 - dutyCycle_26;
    
    // 更新两个LED的亮度
    ledcWrite(ledPin_26, dutyCycle_26);
    ledcWrite(ledPin_25, dutyCycle_25);
  }

  // 示例：非阻塞的串口打印（每秒显示一次亮度）
  static unsigned long serialPrevMillis = 0;
  if (currentMillis - serialPrevMillis >= 1000) {
    serialPrevMillis = currentMillis;
    Serial.print("26号亮度: ");
    Serial.print(dutyCycle_26);
    Serial.print(" | 25号亮度: ");
    Serial.println(255 - dutyCycle_26);
  }
}