#include <esp_now.h>
#include <WiFi.h>
#include <MPU6050.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>
#include <Keypad.h>
#include <ESP32Servo.h>


//********************* SETUP ESPNOW*****************************************//
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = { 0x08, 0x3A, 0x8D, 0x97, 0x54, 0x64 };

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
} struct_message;

// Create a struct_message called myData
struct_message myData;

SemaphoreHandle_t mutex; // Declare a mutex


esp_now_peer_info_t peerInfo;

void checkPIN();
void beep(int count, int delayOn, int delayOff);
void returnServoToInitialPosition();


// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
//****************************SETUP ESPNOW*****************************************************//


//***************************SETUP BUZZER*****************************************************//
MPU6050 mpu;

const int buzzerPin = 13;  // Adjust the pin according to your setup
const int buttonPin = 5;
const int solenoidPin = 4;
const int freq = 500;

void initMPU6050() {
  Wire.begin(21, 22);
  mpu.initialize();
}


/**
 * @brief Task function for motion detection.
 *
 * This function is responsible for detecting motion and performing related actions.
 *
 * @param pvParameters Pointer to task parameters (not used in this function).
 */
void motionDetectionTask(void *pvParameters) {
  float threshold = 1.2;
  for (;;) {
    int16_t ax1, ay1, az1, ax2, ay2, az2;

    //Gets the initial acceleration
    mpu.getAcceleration(&ax1, &ay1, &az1);
    float ax1_f = static_cast<float>(ax1) / 16384.0;;
    float ay1_f = static_cast<float>(ay1)/ 16384.0;;
    float az1_f = static_cast<float>(az1)/ 16384.0;;

    vTaskDelay(40 / portTICK_PERIOD_MS);

    //Gets the final acceleration
    mpu.getAcceleration(&ax2, &ay2, &az2);
    float ax2_f = static_cast<float>(ax2)/ 16384.0;;
    float ay2_f = static_cast<float>(ay2)/ 16384.0;;
    float az2_f = static_cast<float>(az2)/ 16384.0;;

    //Calculates the jerk
    float jerkX = ax2_f - ax1_f;
    float jerkY = ay2_f - ay1_f;
    float jerkZ = az2_f - az1_f;

    //Calculates the magnitude of the jerk
    float jerkMag = sqrt(pow(jerkX, 2) + pow(jerkY, 2) + pow(jerkZ, 2));
    Serial.println(jerkMag);


    if (jerkMag>threshold) {
      xSemaphoreTake(mutex, portMAX_DELAY)
      Serial.println("Motion Detected");
  
      digitalWrite(buzzerPin, HIGH); 

      Serial.println("gherak bos");
      digitalWrite(buzzerPin, LOW);
      strcpy(myData.a, "/photo");
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
      beep(300,45, 15);
      xSemaphoreGive(mutex);

    } else {
      digitalWrite(buzzerPin, HIGH);
    }

    vTaskDelay(250 / portTICK_PERIOD_MS); 
}
}


//********************************SETUP KEYPAD************************************************************//

const byte ROWS = 4;  // Jumlah baris pada keypad
const byte COLS = 4;  // Jumlah kolom pada keypad

// Definisikan layout keypad
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[COLS] = { 27, 23, 32, 12 };
byte colPins[ROWS] = { 33, 25, 26, 14 };

Keypad keypad = Keypad(makeKeymap(keys), colPins, rowPins, COLS, ROWS);

// Pin untuk mengendalikan servo
const int servoPin = 4;  // Ganti dengan pin yang terhubung ke servo

Servo lockServo;  // Objek untuk mengendalikan servo

// Variabel untuk menyimpan PIN yang benar
const char correctPIN[] = "1234";  // Ganti dengan PIN yang diinginkan

char enteredPIN[5];  // Variabel untuk menyimpan PIN yang dimasukkan
int pinIndex = 0;    // Variabel untuk melacak indeks PIN yang dimasukkan

TaskHandle_t keypadTaskHandle;

//***************KEYPAD SETUP*****************************************************//
/**
 * @brief This function is a task that checks the PIN for the Brankas IoT project.
 *
 * @param parameter A pointer to the task parameter.
 */
void checkPINTask(void * parameter) {
  if (xSemaphoreTake(mutex, portMAX_DELAY) == pdTRUE) {
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
       xSemaphoreGive(mutex);
  }
}

/**
 * @brief Checks the PIN entered by the user.
 * 
 * This function is responsible for verifying the PIN entered by the user
 * and performing the necessary actions based on the result.
 */
void checkPIN() {
  enteredPIN[pinIndex] = '\0'; // Menambahkan null terminator ke akhir PIN
  
  // Verifikasi PIN
  if (strcmp(enteredPIN, correctPIN) == 0) { // Membandingkan PIN yang dimasukkan dengan PIN yang benar
    Serial.println("\nPIN Benar! Membuka Brankas");
    lockServo.write(10); // Posisi untuk membuka brankas (sesuaikan dengan kebutuhan)

    //mengirim pesan ke esp cam
    strcpy(myData.a, "/photo");
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
  } else {
    Serial.println("\nPIN Salah! Brankas Tetap Tertutup");
    beep(4,100,100);
          //mengirim pesan ke esp cam
      strcpy(myData.a, "/photo");
      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));
    
  }
  
  // Reset PIN yang dimasukkan
  pinIndex = 0;
  memset(enteredPIN, 0, sizeof(enteredPIN));
}


/**
 * @brief Produces a beep sound.
 * 
 * This function is responsible for producing a beep sound.
 * It can be used to indicate a certain event or condition.
 */
void beep(int count, int delayOn, int delayOff) {
  for (int i = 0; i < count; ++i) {
    digitalWrite(buzzerPin, LOW);
    vTaskDelay(delayOn / portTICK_PERIOD_MS);
    digitalWrite(buzzerPin, HIGH);
    vTaskDelay(delayOff / portTICK_PERIOD_MS);
  }
}


/**
 * Returns the servo to its initial position.
 */
void returnServoToInitialPosition() {
  Serial.println("\nKembali ke posisi awal / Menutup Brankas");
  lockServo.write(90);

}

//*********************************SSETUP************************************************************//


void setup() {

  Serial.begin(9600);
  Serial.println("Starting...");

    Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
    // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(solenoidPin, HIGH);
  // servo.attach(14);

  // Initialize MPU6050
  initMPU6050();
  lockServo.attach(servoPin); // Menghubungkan servo ke pin yang ditentukan
  lockServo.write(90); // Posisi awal servo (sesuaikan dengan kebutuhan)


  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1)
      ;
  }

  // xTaskCreatePinnedToCore(
  //   solenoidTask,
  //   "SolenoidTask",
  //   1000,
  //   NULL,
  //   1,
  //   NULL,
  //   1);

  xTaskCreatePinnedToCore(
    motionDetectionTask,
    "MotionDetectionTask",
    10000,
    NULL,
    1,
    NULL,
    0);

  // Init Serial Monitor
  returnServoToInitialPosition();  
  Serial.println("Sistem Brankas");
  
  xTaskCreatePinnedToCore(
    checkPINTask,
    "CheckPIN",
    10000,
    NULL,
    2,
    &keypadTaskHandle,
    1
  );


}

void loop() {
}
