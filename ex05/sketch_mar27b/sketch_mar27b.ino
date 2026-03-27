// 引脚定义
const int ledPin = 2;
const int touchPin = 4;

// 呼吸灯变量
int brightness = 0;
int step = 5;
bool dir = true;

// 多档位变量
int gear = 1;
const int maxGear = 3;

// 触摸防抖
int lastTouch = HIGH;
unsigned long lastDebounce = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(touchPin, INPUT);
}

void loop() {
  // ======================
  // 触摸切换档位
  // ======================
  int val = digitalRead(touchPin);
  unsigned long now = millis();

  if (val != lastTouch && now - lastDebounce > debounceDelay) {
    lastDebounce = now;
    if (val == LOW) {
      gear++;
      if (gear > maxGear) gear = 1;
    }
  }
  lastTouch = val;

  // ======================
  // 根据档位设置速度
  // ======================
  if (gear == 1) step = 2;   // 慢
  if (gear == 2) step = 5;   // 中
  if (gear == 3) step = 8;   // 快

  // ======================
  // PWM呼吸灯主逻辑
  // ======================
  if (dir) {
    brightness += step;
    if (brightness >= 255) dir = false;
  } else {
    brightness -= step;
    if (brightness <= 0) dir = true;
  }

  analogWrite(ledPin, brightness);
  delay(10);
}