#if defined(ARDUINO_M5Stick_C)
#include <M5StickC.h>
#elif defined(ARDUINO_M5Stick_C_Plus)
#include <M5StickCPlus.h>
#endif

void setup() {
  M5.begin();

  // Improve battery life.
  M5.Axp.ScreenBreath(8);
  setCpuFrequencyMhz(10);

  // Display setting
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(5);

  // IMU & LED
  M5.IMU.Init();
  pinMode(M5_LED, OUTPUT);
}

// Variables for step counter
int step = 0;
float total = 0;
int count = 0;
float avg = 1.1;
float width = avg / 10;
boolean state = false;
boolean old_state = false;

void loop() {
  // Accelerometer value for x,y,z axis.
  float accX = 0;
  float accY = 0;
  float accZ = 0;

  // Get accel.
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  float accel = sqrt(accX * accX + accY * accY + accZ * accZ);

  // Calibration for average acceleration.
  if (count < 100) {
    total += accel;
    count += 1;
  } else {
    avg = total / count;
    width = avg / 10;
    total = avg;
    count = 1;
  }

  // When current accel. is ...
  if (accel > avg + width) {
    state = true;
  } else if (accel < avg - width) {
    state = false;
  }

  // Count up step.
  if (!old_state && state) {
    step += 1;
    digitalWrite(M5_LED, LOW);  // led on

    // Display step
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("Step:\n");
    M5.Lcd.printf("%5d\n", step);
  } else {
    digitalWrite(M5_LED, HIGH); // led off
  }
  old_state = state;

  delay(50);
}
