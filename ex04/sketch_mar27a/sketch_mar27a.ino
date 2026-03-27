// —————— 你的引脚 ——————
const int TOUCH_PIN = 4;   // 触摸接 D4
const int LED_PIN   = 2;   // LED 接 D2

// —————— 状态变量 ——————
bool ledState = false;
int lastTouchState = 0;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(TOUCH_PIN, INPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  int currentTouch = digitalRead(TOUCH_PIN);
  unsigned long currentTime = millis();

  if (currentTouch != lastTouchState && (currentTime - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = currentTime;

    // ======================
    // 这里改成 LOW 就不闪了！
    // ======================
    if (currentTouch == LOW) {  // 👈 把 HIGH 改成 LOW
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }

  lastTouchState = currentTouch;
}