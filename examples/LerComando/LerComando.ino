#include <ESP32QRCodeReader.h>

LeitorQR qr(CAMERA_AI_THINKER);

void setup() {
  Serial.begin(115200);
  qr.ligar();
}

void loop() {
  String comando = qr.ler();

  if (comando == "FRENTE") {
    // andarParaFrente();
  }
}
