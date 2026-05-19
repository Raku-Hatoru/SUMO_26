#include <esp_now.h>
#include <WiFi.h>
// --- Pin Motor A ---
#define PWMA  0   // PWM kecepatan motor A
#define AIN1  3   // Arah motor A - input 1
#define AIN2  1   // Arah motor A - input 2

// --- Pin Motor B ---
#define PWMB  10  // PWM kecepatan motor B
#define BIN1  7   // Arah motor B - input 1
#define BIN2  6   // Arah motor B - input 2

// --- Pin Standby ---
#define STBY  8   // Standby TB6612FNG (HIGH = aktif, LOW = sleep)

// --- Konfigurasi PWM (ESP32-C3 pakai ledcAttach) ---
#define PWM_FREQ     1000   // Frekuensi PWM 1kHz
#define PWM_RESOLUTION  8  // Resolusi 8-bit (0-255)
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
const int deadzone = 20;
int speed = 200;

void onDataReceived(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&receivedData, data, sizeof(receivedData));
  processGamepad();
}

// =============================================
// Fungsi Motor A
// =============================================
void motorA_maju(int kecepatan) {
  // kecepatan: 0-255
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWMA, kecepatan);
  Serial.printf("Motor A: MAJU, kecepatan=%d\n", kecepatan);
}

void motorA_mundur(int kecepatan) {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWMA, kecepatan);
  Serial.printf("Motor A: MUNDUR, kecepatan=%d\n", kecepatan);
}

void motorA_berhenti() {
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  ledcWrite(PWMA, 0);
  Serial.println("Motor A: BERHENTI");
}

void motorA_rem() {
  // Rem aktif (short brake)
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  ledcWrite(PWMA, 255);
  Serial.println("Motor A: REM");
}

// =============================================
// Fungsi Motor B
// =============================================  analogWrite(pwm2, 0);
void motorB_maju(int kecepatan) {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWMB, kecepatan);
  Serial.printf("Motor B: MAJU, kecepatan=%d\n", kecepatan);
}

void motorB_mundur(int kecepatan) {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  ledcWrite(PWMB, kecepatan);
  Serial.printf("Motor B: MUNDUR, kecepatan=%d\n", kecepatan);
}

void motorB_berhenti() {
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
  ledcWrite(PWMB, 0);
  Serial.println("Motor B: BERHENTI");
}

void motorB_rem() {
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);
  ledcWrite(PWMB, 255);
  Serial.println("Motor B: REM");
}

// =============================================
// Fungsi Gerak Robot
// =============================================
void maju(int kecepatan) {
  Serial.println(">> MAJU");
  motorA_maju(kecepatan);
  motorB_maju(kecepatan);
}

void mundur(int kecepatan) {
  Serial.println(">> MUNDUR");
  motorA_mundur(kecepatan);
  motorB_mundur(kecepatan);
}

void belok_kanan_1(int kecepatan) {
  Serial.println(">> BELOK KANAN");
  motorA_maju(kecepatan/2);   // Motor A lebih cepat
  motorB_maju(kecepatan); // Motor B lebih lambat
}

void belok_kiri_1(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan); // Motor A lebih lambat
  motorB_maju(kecepatan/2);     // Motor B lebih cepat
}

void belok_kanan_2(int kecepatan) {
  Serial.println(">> BELOK KANAN");
  motorA_maju(kecepatan); // Motor A lebih lambat
  motorB_berhenti();     // Motor B lebih cepat
}

void belok_kiri_2(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_berhenti();   // Motor A lebih cepat
  motorB_maju(kecepatan); // Motor B lebih lambat

}

void belok_kanan_3(int kecepatan) {
  Serial.println(">> BELOK KANAN");
  motorA_mundur(kecepatan);   // Motor A lebih cepat
  motorB_maju(kecepatan); // Motor B lebih lambat
}

void belok_kiri_3(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan); // Motor A lebih lambat
  motorB_mundur(kecepatan);     // Motor B lebih cepat
}

void putar_kanan(int kecepatan) {
  Serial.println(">> PUTAR KANAN (di tempat)");
  motorA_maju(kecepatan);
  motorB_mundur(kecepatan);
}

void putar_kiri(int kecepatan) {
  Serial.println(">> PUTAR KIRI (di tempat)");
  motorA_mundur(kecepatan);
  motorB_maju(kecepatan);
}

void berhenti() {
  Serial.println(">> BERHENTI");
  motorA_berhenti();
  motorB_berhenti();
}

//==========KODE============
void processGamepad() {

  if (receivedData.a) {
    Serial.println("Tombol A");
    speed = 255;
  }

  if (receivedData.b) {
    Serial.println("Tombol B");
    speed = 200;
  }

  if (receivedData.x) {
    Serial.println("Tombol X");
    speed = 100;
  }

  if (receivedData.y) {
    Serial.println("Tombol Y");
    speed = 150;
  }

  if (receivedData.l1) {
    Serial.println("Tombol LB");

  }

  if (receivedData.r1) {
    Serial.println("Tombol RB");

  }

  if (receivedData.l2) {
    Serial.println("Tombol LT");

  }

  if (receivedData.r2) {
    Serial.println("Tombol RT");

  }

  if (receivedData.thumbL) {
    Serial.println("Tombol L3");

  }

  if (receivedData.thumbR) {
    Serial.println("Tombol R3");

  }

  if (receivedData.dpad == 0x01) {
    Serial.println("D-Pad Atas");
    maju(speed);
    delay(5);
    berhenti();
  }

  if (receivedData.dpad == 0x02) {
    Serial.println("D-Pad Bawah");
    mundur(speed);
    delay(5);
    berhenti();
  }

  if (receivedData.dpad == 0x04) {
    Serial.println("D-Pad Kanan");
    belok_kanan_2(speed);
    delay(5);
    berhenti();
  }

  if (receivedData.dpad == 0x08) {
    Serial.println("D-Pad Kiri");
    belok_kiri_2(speed);
    delay(5);
    berhenti();
  }

  if (abs(receivedData.leftX) > deadzone || abs(receivedData.leftY) > deadzone) {
    Serial.printf("Analog Kiri -> X: %4d, Y: %4d\n", receivedData.leftX, receivedData.leftY);
    // //Atas
    // if (-380<receivedData.leftX<384 && deadzone<receivedData.leftY){
    //   maju(abs((receivedData.leftY-2)/2));
    // }
    // //bawah
    // else if (-380<receivedData.leftX<312 && deadzone>receivedData.leftY){
    //   mundur((receivedData.leftY-2)/2)
    // }
    // //kanan
    // else if(deadzone < receivedData.leftX){
    //   motorA_maju(abs((receivedData.leftY-2)/2)); 
    //   motorB_mundur(abs((receivedData.leftX-2)/2)); 
    // }
    // //kiri
    // else if (deadzone > receivedData.leftX){
    //   motorA_mundur(abs((receivedData.leftY-2)/2)); 
    //   motorB_maju(abs((receivedData.leftX-2)/2)); 
    // }
  }

  if (abs(receivedData.rightX) > deadzone || abs(receivedData.rightY) > deadzone) {
    Serial.printf("Analog Kanan -> X: %4d, Y: %4d\n", receivedData.rightX, receivedData.rightY);

  }

  if (receivedData.triggerL > deadzone) {
    Serial.printf("Trigger L2: %4d\n", receivedData.triggerL);
    putar_kiri(speed);
    delay(5);
    berhenti();
  }

  if (receivedData.triggerR > deadzone) {
    Serial.printf("Trigger R2: %4d\n", receivedData.triggerR);

  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.mode(WIFI_STA);
  Serial.print("MAC Address C3: ");
  Serial.println(WiFi.macAddress());


  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init gagal!");
    return;
  }
  // Inisialisasi pin arah sebagai OUTPUT
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  // Setup PWM menggunakan API baru ESP32 Arduino (v3.x)
  ledcAttach(PWMA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(PWMB, PWM_FREQ, PWM_RESOLUTION);

  // Aktifkan driver (keluar dari mode standby)
  digitalWrite(STBY, HIGH);
  maju(100);
  delay(1000);
  berhenti();
  esp_now_register_recv_cb(onDataReceived);
  Serial.println("ESP32-C3 siap menerima data gamepad!");
  Serial.println("ESP32-C3 + TB6612FNG Motor Driver");



}

void loop() {

}