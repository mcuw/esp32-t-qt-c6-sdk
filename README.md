# ESP32 LilyGo T-QT C6 SDK

## Description

This is an Arduino library (v3.x) to use the [LilyGo T-QT C6](https://lilygo.cc/products/t-qt-c6) with touch gesture display and LiPo battery.
<figure>
  <img src="https://lilygo.cc/cdn/shop/files/T-QT_C6_With_Ring_2.jpg?v=1734338859&width=480" alt="LilyGo T-QT C6 with ring adapter" />
  <figcaption>image from lilygo.cc</figcaption>
</figure>

[LilyGo T-QT C6](https://lilygo.cc/products/t-qt-c6) is a mini development board based on the [ESP32-C6](https://www.espressif.com/en/products/socs/esp32-c6) chip from [espressif](https://www.espressif.com/en) and with a 128x128px TFT full-color touch gesture screen

example code from LilyGo: [T-QT-C6](https://github.com/Xinyuan-LilyGO/T-QT-C6)

## Features

- Wireless: Wi-Fi 6 2.4GHz, Bluetooth 5 (LE), Thread, Zigbee
- CPU 32-bit RISC-V 160 MHz
- CPU 32-bit RISC-V 20 MHz
- 320 KB ROM
- 512 KB SRAM
- supports PSRAM
- protocols: SPI, UART, I2C, RMT, TWAI, PWM, SDIO, Motor Control PWM
- 12-bit ADC
- temperature sensor

## Example codes

Clone this repository.

Uncomment a `src_dir=` line in the `platform.ini` file.

|example|code|description|
|---|---|---|
| Serial | [./examples/serial](./examples/serial) | UART as debug output |
| MAC | [./examples/mac](./examples/mac) | Outputs the MAC address |
| breathing light | [./examples/breathing-light](./examples/breathing-light) | showcase breathing-light |
| task stack size | [./examples/task-stack-size](./examples/taskstack-size) | gather needed stack size |
| dev mode | [./examples/dev-mode](./examples/dev-mode) | QtDev class with all features |
| battery voltage | [./examples/battery-voltage](./examples/battery-voltage) | Outputs the battery voltage |
| touch CST816T | [./examples/touch-CST816T](./examples/touch-CST816T) | touch gesture to Serial |
| gfx | [./examples/gfx](./examples/gfx) | showcase display drawing |
| touch-CST816T-gfx | [./examples/touch-CST816T-gfx](./examples/touch-CST816T-gfx) | display with touch gesture |
| WiFi AP | [./examples/wifi-ap](./examples/wifi-ap) | Access Point with Qwik UI |
|  | [./examples/](./examples/) |  |

### WiFi AP - WiFi Access Point with Qwik UI example

<img src="./assets/wifi-ap-ui.png" alt="wifi-ap example with Qwik UI" />

1. modify the [./partitions.csv](./partitions.csv) if needed to prepare the filesystem

2. comment out `src_dir = ./examples/wifi-ap` line in [platformio.ini](platformio.ini)

3. build and upload the project

<img src="./assets/platformio_build_upload.png" alt="platformio build app" />

4. build and upload Qwik UI (SSG) in `/data`
```sh
cd ui && pnpm i
```
```sh
pnpm build.state
```
5. call
`Build Filesystem Image` then
`Upload Filesystem Image`:

<img src="./assets/filesystem_image.png" alt="upload filesystem platformio option" />

## Get Started a new project

1. create a platformio project (e.g. with expressif esp32-c6-devkitc-1)

2. copy the [boards](boards/) path to your project root path

3. adapt the platformio.ini

```ini
[env:esp32-c6]
board = esp32-c6-n4
framework = arduino
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
lib_deps =
  https://github.com/mcuw/esp32-t-qt-c6-sdk.git
```

## Disclaimer

Contribution and help ... if you find an issue or wants to contribute then please do not hesitate to create a merge request or an issue.

We provide our build template as is, and we make no promises or guarantees about this code.
