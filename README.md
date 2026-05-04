🎧 Reproducción de Audio AAC desde Memoria Interna usando I2S (ESP32)

Proyecto de ejemplo para reproducir audio en formato AAC almacenado en memoria interna de un microcontrolador ESP32, utilizando el protocolo I2S y un amplificador digital como el MAX98357A.

📌 Descripción

Este proyecto implementa un reproductor de audio básico que:

Lee un archivo AAC almacenado en memoria (PROGMEM)
Decodifica el audio usando la librería ESP8266Audio
Envía la señal digital a través del bus I2S
Genera salida de audio mediante un DAC/amplificador externo

El objetivo es comprender el funcionamiento del protocolo I2S y su uso en aplicaciones de audio digital embebido .

🧠 Conceptos clave
I2S (Inter-IC Sound): protocolo para transmisión de audio digital
DAC (Digital-to-Analog Converter): convierte señal digital a analógica
ESP32: microcontrolador con soporte nativo I2S
PROGMEM: almacenamiento de datos en memoria flash interna
🔌 Hardware requerido
ESP32 (NodeMCU o similar)
Módulo amplificador I2S (MAX98357A)
Altavoz (4Ω o 8Ω)
Cables de conexión
🔧 Conexiones
ESP32 GPIO	MAX98357A
GPIO 4	BCLK
GPIO 5	LRC (WS)
GPIO 6	DIN
ESP32        MAX98357A
------       ----------
GPIO4  ----> BCLK
GPIO5  ----> LRC
GPIO6  ----> DIN
GND    ----> GND
3.3V   ----> VIN
📦 Librerías necesarias

Este proyecto utiliza la librería:

ESP8266Audio

Repositorio:
https://github.com/earlephilhower/ESP8266Audio

🚀 Funcionamiento del código
Inicialización (setup())
Se inicia la comunicación serie
Se configuran los objetos:
Fuente de audio (AudioFileSourcePROGMEM)
Decodificador AAC (AudioGeneratorAAC)
Salida I2S (AudioOutputI2S)
Se establecen:
Pines I2S
Ganancia de salida
Se inicia la reproducción
Loop principal (loop())
Mientras el audio está en ejecución:
Se llama a aac->loop() para procesar el buffer
Si termina:
Se detiene el reproductor
Se muestra mensaje por serial
🧾 Código principal
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
🖥️ Salida por monitor serie

Ejemplo de salida:

P7 ejercicio 1 - reproduccion I2S desde memoria interna
MAX98357A -> BCLK: GPIO4, LRC/WS: GPIO5, DIN/SD: GPIO6
Reproduciendo muestra AAC almacenada en memoria interna
⚙️ Parámetros importantes
I2S_GAIN: controla el volumen (0.0 – 1.0)
Pines I2S configurables según tu hardware
Archivo sampleaac.h: contiene el audio en memoria
📈 Ventajas del enfoque
✔ No requiere tarjeta SD
✔ Baja latencia
✔ Implementación sencilla
✔ Ideal para sistemas embebidos
⚠️ Limitaciones
Memoria interna limitada
Tamaño del audio restringido
No escalable para archivos grandes
🔮 Posibles mejoras
Reproducción desde tarjeta SD
Soporte para MP3/WAV
Control de volumen dinámico
Interfaz con botones o Bluetooth
📚 Referencias
Documentación práctica I2S
Librería ESP8266Audio
Datasheet MAX98357A
