# ESP32QR

A câmera do ESP32-CAM lê o QR. A biblioteca devolve a palavra que está escrita nele. O movimento do robô não vem pronto: comparar a palavra e andar é código de vocês.

A classe da aula é `LeitorQR`. O arquivo do `#include` continua `ESP32QRCodeReader.h`.

## Sketch

```cpp
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
```

O exemplo pronto está em `examples/LerComando/LerComando.ino`.

## O que cada linha faz

- **`#include <ESP32QRCodeReader.h>`** traz `LeitorQR` e os modelos de placa.
- **`LeitorQR qr(CAMERA_AI_THINKER);`** cria o leitor fora do `setup` e do `loop`, com os pinos da ESP32-CAM AI-Thinker. `LeitorQR qr;` faz a mesma escolha de placa.
- **`Serial.begin(115200);`** abre o Monitor Serial. `ligar()` escreve nele se a câmera subiu ou falhou.
- **`qr.ligar();`** inicia a câmera e a leitura. Chame uma vez no `setup`. Devolve `true` quando ficou pronto e `false` quando a placa não tem PSRAM ou a câmera não iniciou. Chamar de novo não reinicia: devolve `true`.
- **`String comando = qr.ler();`** pede a palavra. Chame no `loop`, o tempo todo.

## O que `ler()` devolve

Uma `String` com o texto do QR, do jeito que foi gravado. `FRENTE` e `frente` são palavras diferentes.

Devolve texto vazio (`""`) quando não há palavra nova. Isso acontece o tempo inteiro: a maior parte das voltas do `loop` não vê QR. O `if (comando == "FRENTE")` simplesmente não entra.

A mesma palavra sai **uma vez**. Enquanto o QR continua na frente da câmera, as próximas chamadas devolvem `""`. Tirem o QR da cena. Depois de cerca de 0,8 s sem leitura, a próxima vez que ele aparecer devolve a palavra de novo.

Se o QR trocar (de `FRENTE` para `GIRAR`) sem sair da cena, `ler()` devolve a palavra nova na hora.

Cada chamada devolve no máximo uma palavra: a mais recente que a câmera já tinha reconhecido.

`ler()` antes de um `ligar()` com sucesso também devolve `""`.

## Vários comandos

```cpp
#include <ESP32QRCodeReader.h>

LeitorQR qr(CAMERA_AI_THINKER);

void setup() {
  Serial.begin(115200);
  if (!qr.ligar()) {
    return;
  }
}

void loop() {
  String comando = qr.ler();

  if (comando == "FRENTE") {
    // andarParaFrente();
  } else if (comando == "GIRAR") {
    // girar();
  } else if (comando == "PARAR") {
    // parar();
  }
}
```

A palavra dentro do QR e a palavra entre aspas precisam ser a mesma.

## Modelo da placa

O nome entre parênteses escolhe os pinos da câmera. O padrão, e o da ESP32-CAM mais comum em aula, é `CAMERA_AI_THINKER`.

| Nome | Placa |
| --- | --- |
| `CAMERA_AI_THINKER` | ESP32-CAM AI-Thinker |
| `CAMERA_WROVER_KIT` | ESP32 Wrover Kit |
| `CAMERA_ESP_EYE` | ESP-EYE |
| `CAMERA_M5STACK_PSRAM` | M5Stack com PSRAM |
| `CAMERA_M5STACK_V2_PSRAM` | M5Stack V2 com PSRAM |
| `CAMERA_M5STACK_WIDE` | M5Stack Wide |
| `CAMERA_M5STACK_ESP32CAM` | M5Stack ESP32CAM |
| `CAMERA_TTGO_T_JOURNAL` | TTGO T-Journal |

Cada nome curto acima é o mesmo que o nome longo `CAMERA_MODEL_...` (por exemplo `CAMERA_AI_THINKER` e `CAMERA_MODEL_AI_THINKER`).

A imagem usada na leitura é cinza, tamanho QVGA. O QR precisa estar perto e legível. A placa precisa ter PSRAM.

## Instalar no Arduino

Copie a pasta `ESP32QR` para a pasta `libraries` do Arduino. O sketch usa a placa **ESP32** com câmera e PSRAM. Abra o Monitor Serial em **115200**.

Mensagens de `ligar()`:

- `LeitorQR pronto.` — a câmera subiu.
- `LeitorQR: placa sem PSRAM.` — essa placa não serve para esta biblioteca.
- `LeitorQR: camera nao iniciou. Confira o modelo da placa.` — o nome passado no construtor não é o da placa que está ligada.

## A classe antiga

`ESP32QRCodeReader` continua no mesmo header, para quem precisa da fila, do payload cru ou do debug. Na aula, usem `LeitorQR`. Ela chama essa classe por baixo: `ligar()` faz o `setup()` e o `begin()`; `ler()` esvazia a fila e aplica a regra de devolver a palavra uma vez.
