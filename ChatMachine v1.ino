
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ChatMachine"); // name
  Serial.println("Chat Machine - ONLINE.");
}

void loop() {
  if (Serial.available())SerialBT.write(Serial.read());
  if (SerialBT.available())Serial.write(SerialBT.read());
  delay(20);
}
