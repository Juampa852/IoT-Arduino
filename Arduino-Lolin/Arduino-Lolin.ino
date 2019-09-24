#include "SoftwareSerial.h"

const int baudRate = 9600;
const byte rx=2;
const byte tx=3;

SoftwareSerial softSerial(rx, tx); // RX, TX

void setup()
{
  Serial.begin(baudRate);
  softSerial.begin(baudRate);
}

void loop()
// enviar los datos de la consola serial al ESP-01, 
// y mostrar lo enviado por el ESP-01 a nuestra consola
{
  if (softSerial.available())
  {
    Serial.print((char)softSerial.read());
  }
  if (Serial.available())
  {
    softSerial.print((char)Serial.read());
  }
}
