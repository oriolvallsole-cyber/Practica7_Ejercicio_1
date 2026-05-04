#include <Arduino.h>
#include "AudioFileSourcePROGMEM.h"
#include "AudioGeneratorAAC.h"
#include "AudioOutputI2S.h"
#include "sampleaac.h"

#define I2S_BCLK 4
#define I2S_LRC 5
#define I2S_DOUT 6
#define I2S_GAIN 0.125f

AudioFileSourcePROGMEM *in = nullptr;
AudioGeneratorAAC *aac = nullptr;
AudioOutputI2S *out = nullptr;

void setup() {
  Serial.begin(115200);
  delay(1000);

  audioLogger = &Serial;
  Serial.println();
  Serial.println("P7 ejercicio 1 - reproduccion I2S desde memoria interna");
  Serial.printf("MAX98357A -> BCLK: GPIO%d, LRC/WS: GPIO%d, DIN/SD: GPIO%d\n",
                I2S_BCLK, I2S_LRC, I2S_DOUT);

  in = new AudioFileSourcePROGMEM(sampleaac, sizeof(sampleaac));
  aac = new AudioGeneratorAAC();
  out = new AudioOutputI2S();

  out->SetGain(I2S_GAIN);
  out->SetPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  if (aac->begin(in, out)) {
    Serial.println("Reproduciendo muestra AAC almacenada en memoria interna");
  } else {
    Serial.println("No se pudo iniciar el generador AAC");
  }
}

void loop() {
  if (aac != nullptr && aac->isRunning()) {
    if (!aac->loop()) {
      aac->stop();
    }
  } else {
    Serial.printf("Sound Generator\n");
    delay(1000);
  }
}