#include <Wire.h>
#include <SparkFun_VL6180X.h>
#define VL6180X_ADDRESS 0x29

const int pingPin = 4;                       // Trigger Pin of Ultrasonic Sensor
const int echoPin = 5;                       // Echo Pin of Ultrasonic Sensor
const int maxVal = 120;                      // max value of light scope (450)
const int minVal = 0;                        // min value og light scope
int counter = 0;
int light = 0;
int lightPercent = 0;
int distance;

VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

void setup() {
  Serial.begin(9600);                        // Start Serial data stream
  Wire.begin();                              // Start I2C library
  delay(100);

  pinMode(3, OUTPUT);
  pinMode(pingPin, OUTPUT);
  pinMode(echoPin, INPUT);
  sensor.getIdentification(&identification); // Retrieve manufacture info from device memory
  printIdentification(&identification);      // Helper function to print all the Module information

  if (sensor.VL6180xInit() != 0)  {          // verify sensor availability
    Serial.println("Failed to initialize. Freezing...");
    while (true);
  }

  sensor.VL6180xDefautSettings();            // Load default settings to get started

  delay(1000);
}

void loop() {
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distance = microsecondsToCentimeters(duration);

  //  Serial.print(distance);
  //  Serial.println("cm");

  if (distance > 70) {
    counter++;

    if (counter > 6) {
      Serial.println("dist");
      light = 0;
      lightPercent = 0;
      counter = 0;
    }
  } else {
    light = sensor.getAmbientLight(GAIN_1);
    lightPercent = light;
    //  Serial.print("Ambient Light Level (Lux) = ");
    //  Serial.println(light);

    if (light >= maxVal) {
      light = 255;
      lightPercent = 100;
    } else if (light <= minVal) {
      light = 0;
      lightPercent = 0;
    } else {
      lightPercent = map(light, minVal, maxVal, 0, 100);
      light = map(light, minVal, maxVal, 0, 255);
    }
  }
  analogWrite(3, light);
  Serial.println(lightPercent);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void printIdentification(struct VL6180xIdentification *temp) {
  Serial.print("Model ID = ");
  Serial.println(temp->idModel);

  Serial.print("Model Rev = ");
  Serial.print(temp->idModelRevMajor);
  Serial.print(".");
  Serial.println(temp->idModelRevMinor);

  Serial.print("Module Rev = ");
  Serial.print(temp->idModuleRevMajor);
  Serial.print(".");
  Serial.println(temp->idModuleRevMinor);

  Serial.print("Manufacture Date = ");
  Serial.print((temp->idDate >> 3) & 0x001F);
  Serial.print("/");
  Serial.print((temp->idDate >> 8) & 0x000F);
  Serial.print("/1");
  Serial.print((temp->idDate >> 12) & 0x000F);
  Serial.print(" Phase: ");
  Serial.println(temp->idDate & 0x0007);

  Serial.print("Manufacture Time (s)= ");
  Serial.println(temp->idTime * 2);
  Serial.println();
  Serial.println();
}