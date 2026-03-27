// 两个LED引脚（你要的 D2、D4）
const int ledA = 2;
const int ledB = 4;

int brightness = 0;
int step = 3;

void setup() {
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
}

void loop() {
  // 亮度渐变
  brightness += step;

  // 边界反转
  if (brightness >= 255 || brightness <= 0) {
    step = -step;
  }

  // 核心：反相呼吸
  analogWrite(ledA, brightness);       // 灯A 变亮
  analogWrite(ledB, 255 - brightness); // 灯B 同时变暗

  delay(10);
}