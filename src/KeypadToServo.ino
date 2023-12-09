#include <Keypad.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ESP32Servo.h>

const byte ROWS = 4; // Jumlah baris pada keypad
const byte COLS = 4; // Jumlah kolom pada keypad

// Definisikan layout keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[COLS] = {27, 13, 22, 5};
byte colPins[ROWS] = {33, 25, 26, 14};

Keypad keypad = Keypad( makeKeymap(keys), colPins, rowPins, COLS, ROWS );

// Pin untuk mengendalikan servo
const int servoPin = 4; // Ganti dengan pin yang terhubung ke servo

Servo lockServo; // Objek untuk mengendalikan servo

// Variabel untuk menyimpan PIN yang benar
const char correctPIN[] = "1234"; // Ganti dengan PIN yang diinginkan

char enteredPIN[5]; // Variabel untuk menyimpan PIN yang dimasukkan
int pinIndex = 0; // Variabel untuk melacak indeks PIN yang dimasukkan

TaskHandle_t keypadTaskHandle;

void checkPINTask(void * parameter) {
  Serial.println("Silahkan Masukkan PIN");
  for (;;) {
    vTaskDelay(100); // Menunggu 100ms sebelum membaca keypad lagi
    
    char key = keypad.getKey();
  
    if (key != NO_KEY) {
      Serial.println(key); // Menampilkan tombol yang ditekan
      
      // Memeriksa apakah tombol yang ditekan adalah digit (0-9)
      if (isdigit(key)) {
        enteredPIN[pinIndex++] = key; // Menyimpan karakter PIN yang dimasukkan
        Serial.print("PIN yang dimasukkan: ");
        Serial.println(enteredPIN);
        
        // Jika sudah dimasukkan 4 karakter (PIN), verifikasi PIN
        if (pinIndex == 4) {
          checkPIN();
        }
      } else if (key == 'C') {
        returnServoToInitialPosition();
      }
    }
  }
}

void setup() {
  Serial.begin(9600);
  
  lockServo.attach(servoPin); // Menghubungkan servo ke pin yang ditentukan
  lockServo.write(0); // Posisi awal servo (sesuaikan dengan kebutuhan)
  
  Serial.println("Sistem Brankas");
  
  xTaskCreatePinnedToCore(
    checkPINTask,
    "CheckPIN",
    2048,
    NULL,
    1,
    &keypadTaskHandle,
    1
  );
}

void loop() {
  
}

void checkPIN() {
  enteredPIN[pinIndex] = '\0'; // Menambahkan null terminator ke akhir PIN
  
  // Verifikasi PIN
  if (strcmp(enteredPIN, correctPIN) == 0) { // Membandingkan PIN yang dimasukkan dengan PIN yang benar
    Serial.println("\nPIN Benar! Membuka Brankas");
    lockServo.write(90); // Posisi untuk membuka brankas (sesuaikan dengan kebutuhan)
  } else {
    Serial.println("\nPIN Salah! Brankas Tetap Tertutup");
  }
  
  // Reset PIN yang dimasukkan
  pinIndex = 0;
  memset(enteredPIN, 0, sizeof(enteredPIN));
}

void returnServoToInitialPosition() {
  Serial.println("\nKembali ke posisi awal / Menutup Brankas");
  lockServo.write(0); // Posisi untuk menutup brankas (sesuaikan dengan kebutuhan)
  delay(2000); // Waktu untuk menutup brankas (sesuaikan dengan kebutuhan)
}
