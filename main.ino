#include <Wire.h>
#include <SparkFun_VL6180X.h>

#define VL6180X_ADDRESS 0x29

VL6180xIdentification identification;
VL6180x sensor(VL6180X_ADDRESS);

void setup() {
  Serial.begin(9600);                        // Start Serial data stream
  Wire.begin();                              // Start I2C library
  delay(100);

  int maxVal = 450;                          // max value of light scope
  int minVal = 3;                            // min value og light scope
  pinMode(3, OUTPUT);
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
  int lightLevel = sensor.getAmbientLight(GAIN_1);
  int distance = sensor.getDistance();

  Serial.print("Ambient Light Level (Lux) = ");
  Serial.println(lightLevel);

  Serial.print("Distance measured (mm) = ");
  Serial.println(distance);

  if (lightLevel >= maxVal) {
    lightLevel = 0;
  } else if (lightLevel <= minVal) {
    lightLevel = 255;
  } else {
    lightLevel = map(lightLevel, minVal, maxVal, 255, 0);
  }
  analogWrite(3, lightLevel);

  if (Serial.available()) {
    Serial.write(lightLevel);
  }
}

void printIdentification(struct VL6180xIdentification *temp)
{
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
