#if defined(ARDUINO_M5Stick_C)
#include <M5StickC.h>
#elif defined(ARDUINO_M5Stick_C_Plus)
#include <M5StickCPlus.h>
#endif

float accX = 0;
float accY = 0;
float accZ = 0;

void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.IMU.Init();
  pinMode(M5_LED, OUTPUT);
}

int step = 0;
float total = 0;
int count = 0;
float avg = 1.1;
float width = avg / 10;
boolean state = false;
boolean old_state = false;

void loop() {
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  float accel = sqrt(accX * accX + accY * accY + accZ * accZ);

  if (count < 100) {
    total += accel;
    count += 1;
  } else {
    avg = total / count;
    width = avg / 10;
    total = avg;
    count = 1;
  }
  if (accel > avg + width) {
    state = true;
  } else if (accel < avg - width) {
    state = false;
  }
  if (!old_state && state) {
    step += 1;
    // led on
    digitalWrite(M5_LED, LOW);
  } else {
    // led off
    digitalWrite(M5_LED, HIGH);
  }
  old_state = state;

  // Display
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Step:%d\n", step);
  M5.Lcd.printf("Count:%3d\n", count);
  M5.Lcd.printf("Accel:%f\n", accel);
  M5.Lcd.printf("Avg:%f\n", avg);

  delay(50);
}
