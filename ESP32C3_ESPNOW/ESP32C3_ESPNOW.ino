#include <esp_now.h>
#include <WiFi.h>

typedef struct {
  int  leftX;
  int  leftY;
  int  rightX;
  int  rightY;
  int  triggerL;
  int  triggerR;
  bool a, b, x, y;
  bool l1, r1, l2, r2;
  bool thumbL, thumbR;
  uint8_t dpad;
} GamepadData;

GamepadData receivedData;
GamepadData prevData;
const int deadzone = 20;

#define ON_PRESS(btn, label, code) \
  if (receivedData.btn && !prevData.btn) { \
    Serial.println("Pressing : " label); \
    code \
  }

#define ON_RELEASE(btn, label, code) \
  if (!receivedData.btn && prevData.btn) { \
    Serial.println("Releasing: " label); \
    code \
  }

#define ON_PRESS_DPAD(val, label, code) \
  if (receivedData.dpad == val && prevData.dpad != val) { \
    Serial.println("Pressing : " label); \
    code \
  }

#define ON_RELEASE_DPAD(val, label, code) \
  if (receivedData.dpad != val && prevData.dpad == val) { \
    Serial.println("Releasing: " label); \
    code \
  }

void processGamepad() {

  ON_PRESS  (a, "A", {})
  ON_RELEASE(a, "A", {})

  ON_PRESS  (b, "B", {})
  ON_RELEASE(b, "B", {})

  ON_PRESS  (x, "X", {})
  ON_RELEASE(x, "X", {})

  ON_PRESS  (y, "Y", {})
  ON_RELEASE(y, "Y", {})

  ON_PRESS  (l1, "LB", {})
  ON_RELEASE(l1, "LB", {})

  ON_PRESS  (r1, "RB", {})
  ON_RELEASE(r1, "RB", {})

  ON_PRESS  (l2, "LT", {})
  ON_RELEASE(l2, "LT", {})

  ON_PRESS  (r2, "RT", {})
  ON_RELEASE(r2, "RT", {})

  ON_PRESS  (thumbL, "L3", {})
  ON_RELEASE(thumbL, "L3", {})

  ON_PRESS  (thumbR, "R3", {})
  ON_RELEASE(thumbR, "R3", {})

  ON_PRESS_DPAD  (0x01, "D-Pad Atas", {})
  ON_RELEASE_DPAD(0x01, "D-Pad Atas", {})

  ON_PRESS_DPAD  (0x02, "D-Pad Bawah", {})
  ON_RELEASE_DPAD(0x02, "D-Pad Bawah", {})

  ON_PRESS_DPAD  (0x04, "D-Pad Kanan", {})
  ON_RELEASE_DPAD(0x04, "D-Pad Kanan", {})

  ON_PRESS_DPAD  (0x08, "D-Pad Kiri", {})
  ON_RELEASE_DPAD(0x08, "D-Pad Kiri", {})

  if (abs(receivedData.leftX) > deadzone || abs(receivedData.leftY) > deadzone) {
    Serial.printf("Analog Kiri  -> X: %4d, Y: %4d\n", receivedData.leftX, receivedData.leftY);
  }

  if (abs(receivedData.rightX) > deadzone || abs(receivedData.rightY) > deadzone) {
    Serial.printf("Analog Kanan -> X: %4d, Y: %4d\n", receivedData.rightX, receivedData.rightY);
  }

  if (receivedData.triggerL > deadzone) {
    Serial.printf("Trigger L2   -> %4d\n", receivedData.triggerL);
  }

  if (receivedData.triggerR > deadzone) {
    Serial.printf("Trigger R2   -> %4d\n", receivedData.triggerR);
  }
}

void onDataReceived(const uint8_t *mac_addr, const uint8_t *data, int len) {
  prevData = receivedData;
  memcpy(&receivedData, data, sizeof(receivedData));
  processGamepad();
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  memset(&prevData, 0, sizeof(prevData));

  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address C3: ");
  Serial.println(WiFi.macAddress());

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init gagal!");
    return;
  }

  esp_now_register_recv_cb(onDataReceived);
  Serial.println("ESP32-C3 siap menerima data gamepad!");
}

void loop() {}