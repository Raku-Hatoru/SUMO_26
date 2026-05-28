// =============================================
// ESP32-C3 SuperMini + TB6612FNG Motor Driver
// Kontrol 2 Motor DC (Motor A dan Motor B)
// =============================================
// ESP32-C3
// --- Pin Motor A ---
// #define PWMA  0   // PWM kecepatan motor A  0
// #define AIN1  3   // Arah motor A - input 1 3
// #define AIN2  1   // Arah motor A - input 2 1
// 
// --- Pin Motor B ---
// #define PWMB  10  // PWM kecepatan motor B  10
// #define BIN1  7   // Arah motor B - input 1 7
// #define BIN2  6   // Arah motor B - input 2 6
 
// --- Pin Motor A ---
#define PWMA  32   // PWM kecepatan motor A  0
#define AIN1  25   // Arah motor A - input 1 3
#define AIN2  33   // Arah motor A - input 2 1

// --- Pin Motor B ---
#define PWMB  14  // PWM kecepatan motor B  10
#define BIN1  26   // Arah motor B - input 1 7
#define BIN2  27   // Arah motor B - input 2 6


// --- Pin Standby ---
// #define STBY  8   // Standby TB6612FNG (HIGH = aktif, LOW = sleep)

// --- Konfigurasi PWM (ESP32-C3 pakai ledcAttach) ---
#define PWM_FREQ     1000   // Frekuensi PWM 1kHz
#define PWM_RESOLUTION  8  // Resolusi 8-bit (0-255)

// =============================================
// Fungsi Setup
// =============================================
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-C3 + TB6612FNG Motor Driver");

  // Inisialisasi pin arah sebagai OUTPUT
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  // pinMode(STBY, OUTPUT);

  // Setup PWM menggunakan API baru ESP32 Arduino (v3.x)
  ledcAttach(PWMA, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(PWMB, PWM_FREQ, PWM_RESOLUTION);

  // Aktifkan driver (keluar dari mode standby)
  // digitalWrite(STBY, HIGH);

  Serial.println("Motor driver siap!");
  delay(1000);
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
  motorA_berhenti();   // Motor A lebih cepat
  motorB_maju(kecepatan); // Motor B lebih lambat
}

void belok_kiri_2(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan); // Motor A lebih lambat
  motorB_berhenti();     // Motor B lebih cepat
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

// void standby(bool aktif) {
//   // false = masuk mode sleep (hemat daya)
//   // true  = keluar dari sleep
//   digitalWrite(STBY, aktif ? HIGH : LOW);
//   Serial.printf("Driver mode: %s\n", aktif ? "AKTIF" : "SLEEP");
// }

// =============================================
// Loop Utama - Demo Gerakan
// =============================================
void loop() {
  Serial.println("\n===== Demo Gerakan Robot =====");

  // Maju penuh 2 detik
  maju(200);
  delay(3000);
  maju(100);
  delay(3000);
  // delay(2000);

  // berhenti();
  // delay(500);

  // Mundur 1.5 detik
  // mundur(180);
  // delay(1500);

  // berhenti();
  // delay(500);

  // // Belok kanan 1 detik
  // belok_kanan(200);
  // delay(1000);

  // berhenti();
  // delay(500);

  // // Belok kiri 1 detik
  // belok_kiri(200);
  // delay(1000);

  // berhenti();
  // delay(500);

  // // Putar di tempat ke kanan
  // putar_kanan(150);
  // delay(800);

  // berhenti();
  // delay(500);

  // // Putar di tempat ke kiri
  // putar_kiri(150);
  // delay(800);

  // berhenti();
  // delay(500);

  // // Demo akselerasi bertahap
  // Serial.println(">> AKSELERASI BERTAHAP");
  // for (int spd = 0; spd <= 255; spd += 5) {
  //   maju(spd);
  //   delay(30);
  // }

  // // Deselerasi bertahap
  // Serial.println(">> DESELERASI BERTAHAP");
  // for (int spd = 255; spd >= 0; spd -= 5) {
  //   maju(spd);
  //   delay(30);
  // }

  // berhenti();

  // // Mode sleep 2 detik
  // standby(false);
  // delay(2000);
  // standby(true);

  // delay(1000);
}