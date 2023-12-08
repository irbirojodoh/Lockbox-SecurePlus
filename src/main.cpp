#include <MPU6050.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <math.h>

MPU6050 mpu;

const int buzzerPin = 13; // Adjust the pin according to your setup
const int buttonPin = 5;
const int solenoidPin = 4;
const float motionThresholdLow = 0.70; // Adjust the threshold based on your requirements
const float motionThresholdHigh = 0.5; // Adjust the threshold based on your requirements
const int freq = 500;

void initMPU6050() {
 Wire.begin(21, 22);
  mpu.initialize();
}

void toggleBuzzer() {
  digitalWrite(buzzerPin, !digitalRead(buzzerPin));

}

void solenoidTask(void *pvParameters) {
  for (;;) {
    if (digitalRead(buttonPin) == LOW) {
      digitalWrite(solenoidPin, 1);
      //servo.write(0);
      vTaskDelay(3000 / portTICK_PERIOD_MS);
      digitalWrite(solenoidPin, 0);
     // servo.write(90);
      Serial.println("Button pressed");
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void motionDetectionTask(void *pvParameters) {
  float motionMag = 10;
  for (;;) {
    // Read accelerometer data
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    // Convert to floating-point
float ax_f = static_cast<float>(ax);
float ay_f = static_cast<float>(ay);
float az_f = static_cast<float>(az);

Serial.printf("ax: %f, ay: %f, az: %f\n", ax_f/ 16384.0f - 1, ay_f/ 16384.0f, az_f/ 16384.0f);

// Calculate magnitude
float motionMag = sqrt(pow(fabs(ax_f)/ 16384.0f - 1, 2) + pow(fabs(ay_f) / 16384.0f, 2) + pow(fabs(az_f) / 16384.0f, 2));
    Serial.println(motionMag);

    // Check if motion is detected
    if (motionMag > motionThresholdHigh) {
      Serial.println("Motion Detected");
      //Serial.println(motionMag);

      // Play buzzer
     // digitalWrite(buzzerPin, HIGH); // Adjust frequency and duration based on your preference

      // Add any additional actions you want to perform when motion is detected
        Serial.println("gherak bos");
        digitalWrite(buzzerPin, LOW);


      delay(1000); // Delay to avoid repeated detections
    }else{ 
      digitalWrite(buzzerPin, HIGH);
    }
   // digitalWrite(buzzerPin, LOW);

    vTaskDelay(250 / portTICK_PERIOD_MS); // Adjust the delay based on your requirements
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, HIGH);
  pinMode(solenoidPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  digitalWrite(solenoidPin, HIGH);
 // servo.attach(14);

  // Initialize MPU6050
  Wire.begin();
  initMPU6050();

  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  xTaskCreatePinnedToCore(
      solenoidTask,
      "SolenoidTask",
      1000,
      NULL,
      1,
      NULL,
      1);

  xTaskCreatePinnedToCore(
      motionDetectionTask,
      "MotionDetectionTask",
      10000,
      NULL,
      2,
      NULL,
      1);
}

void loop() {
  // Your main loop can be empty or used for background tasks if needed
}
