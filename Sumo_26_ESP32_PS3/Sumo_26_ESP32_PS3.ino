#include <Ps3Controller.h>
int player = 1;
// =============================================
// ESP32 DevKit Type-C + TB6612FNG Motor Driver
// Kontrol 2 Motor DC (Motor A dan Motor B)
// =============================================
// ESP32 DevKit Type-C
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
#define PWMA 32  // PWM kecepatan motor A  0
#define AIN1 33  // Arah motor A - input 1 3
#define AIN2 25  // Arah motor A - input 2 1

// --- Pin Motor B ---
#define PWMB 14  // PWM kecepatan motor B  10
#define BIN1 27  // Arah motor B - input 1 7
#define BIN2 26  // Arah motor B - input 2 6
int kecepatan = 200;
// --- Pin Standby ---
// #define STBY  8   // Standby TB6612FNG (HIGH = aktif, LOW = sleep)

// --- Konfigurasi PWM (Arduino ESP32 core v3.x pakai ledcAttach) ---
#define PWM_FREQ 1000     // Frekuensi PWM 1kHz
#define PWM_RESOLUTION 8  // Resolusi 8-bit (0-255)
void onConnect() {
  Serial.println("Connected.");
}
// =============================================
// Fungsi Setup
// =============================================
void setup() {
  Serial.begin(115200);
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin("12:22:33:44:55:66");
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
  motorA_maju(kecepatan / 2);  // Motor A lebih cepat
  motorB_maju(kecepatan);      // Motor B lebih lambat
}

void belok_kiri_1(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan);      // Motor A lebih lambat
  motorB_maju(kecepatan / 2);  // Motor B lebih cepat
}

void belok_kanan_2(int kecepatan) {
  Serial.println(">> BELOK KANAN");
  motorA_berhenti();       // Motor A lebih cepat
  motorB_maju(kecepatan);  // Motor B lebih lambat
}

void belok_kiri_2(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan);  // Motor A lebih lambat
  motorB_berhenti();       // Motor B lebih cepat
}

void belok_kanan_3(int kecepatan) {
  Serial.println(">> BELOK KANAN");
  motorA_mundur(kecepatan);  // Motor A lebih cepat
  motorB_maju(kecepatan);    // Motor B lebih lambat
}

void belok_kiri_3(int kecepatan) {
  Serial.println(">> BELOK KIRI");
  motorA_maju(kecepatan);    // Motor A lebih lambat
  motorB_mundur(kecepatan);  // Motor B lebih cepat
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
// Fungsi Notify atau receivernya
// =============================================

void notify() {
  //--- Digital cross/square/triangle/circle button events ---
  if (Ps3.event.button_down.cross) {
    Serial.println("Started pressing the cross button");
  }
  if (Ps3.event.button_up.cross) {
    Serial.println("Released the cross button");
    berhenti();
  }

  if (Ps3.event.button_down.square) {
    Serial.println("Started pressing the square button");
  }
  if (Ps3.event.button_up.square) {
    Serial.println("Released the square button");
    berhenti();
  }

  if (Ps3.event.button_down.triangle) {
    Serial.println("Started pressing the triangle button");
  }
  if (Ps3.event.button_up.triangle) {
    Serial.println("Released the triangle button");
    berhenti();
  }

  if (Ps3.event.button_down.circle) {
    Serial.println("Started pressing the circle button");
  }
  if (Ps3.event.button_up.circle) {
    Serial.println("Released the circle button");
    berhenti();
  }

  //--------------- Digital D-pad button events --------------
  if (Ps3.event.button_down.up) {
    Serial.println("Started pressing the up button");
    maju(kecepatan);
  }
  if (Ps3.event.button_up.up) {
    Serial.println("Released the up button");
    berhenti();
  }

  if (Ps3.event.button_down.right) {
    Serial.println("Started pressing the right button");
    belok_kanan_2(kecepatan);
  }
  if (Ps3.event.button_up.right) {
    Serial.println("Released the right button");
    berhenti();
  }

  if (Ps3.event.button_down.down) {
    Serial.println("Started pressing the down button");
    mundur(kecepatan);
  }
  if (Ps3.event.button_up.down) {
    Serial.println("Released the down button");
    berhenti();
  }

  if (Ps3.event.button_down.left) {
    Serial.println("Started pressing the left button");
    belok_kiri_2(kecepatan);
  }
  if (Ps3.event.button_up.left) {
    Serial.println("Released the left button");
    berhenti();
  }

  //------------- Digital shoulder button events -------------
  if (Ps3.event.button_down.l1) {
    Serial.println("Started pressing the left shoulder button");
  }
  if (Ps3.event.button_up.l1) {
    Serial.println("Released the left shoulder button");
    berhenti();
  }

  if (Ps3.event.button_down.r1) {
    Serial.println("Started pressing the right shoulder button");
  }
  if (Ps3.event.button_up.r1) {
    Serial.println("Released the right shoulder button");
    berhenti();
  }

  //-------------- Digital trigger button events -------------
  if (Ps3.event.button_down.l2) {
    Serial.println("Started pressing the left trigger button");
  }
  if (Ps3.event.button_up.l2) {
    Serial.println("Released the left trigger button");
    berhenti();
  }

  if (Ps3.event.button_down.r2) {
    Serial.println("Started pressing the right trigger button");
  }
  if (Ps3.event.button_up.r2) {
    Serial.println("Released the right trigger button");
    berhenti();
  }
}

// =============================================
// Loop Utama - Demo Gerakan
// =============================================
void loop() {
  // Serial.println("\n===== Demo Gerakan Robot =====");
  if (!Ps3.isConnected()) {
    return;
  }
  Ps3.setPlayer(player);

  // Maju penuh 2 detik
  // maju(200);
  // delay(3000);
  // maju(100);
  // delay(3000);
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
