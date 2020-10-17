# Arduino Graphicstest PDQ

Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

## Testing Hardware

-  ESP32-DevKitC https://www.espressif.com/en/products/devkits/esp32-devkitc/overview
-  ESP32 LCDKit https://github.com/espressif/esp-iot-solution/blob/master/documents/evaluation_boards/ESP32_LCDKit_guide_en.md
-  ILI9341 Breakout Board

## Testing Software

-  Arduino IDE 1.8.13
-  Arduino-esp32 master branch

## Testing Libraries

-  Adafruit GFX https://github.com/adafruit/Adafruit-GFX-Library
-  Arduino GFX https://github.com/moononournation/Arduino_GFX
-  TFT_eSPI https://github.com/Bodmer/TFT_eSPI
-  Ucglib https://github.com/olikraus/ucglib
-  UTFT-ESP https://github.com/gnulabis/UTFT-ESP

## Testing Parameters
-  PSRAM: disable
-  Interface: SPI@40MHz

## Result
-  Time: 2020 Oct 15

| Benchmark          | Adafruit_GFX  | Arduino_GFX   | LovyanGFX     |TFT_eSPI       | Ucglib        | UTFT-ESP      |
|                    |               | ESP32SPI      |               |               |               |               |
| ------------------ | ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| Screen fill        |       195,259 |       160,096 |       154,216 |       166,761 |     2,010,234 |     4,907,718 |
| Text               |        97,567 |        18,933 |        21,726 |        24,662 |       107,286 |       492,000 |
| Pixels             |     1,353,315 |       903,550 |       814,232 |       770,618 |     3,047,934 |    12,255,529 |
| Lines              |     1,061,903 |       433,864 |       261,781 |       308,601 |     4,141,582 |     8,955,660 |
| Horiz/Vert Lines   |        17,601 |        14,181 |        13,612 |        14,587 |       160,288 |       415,958 |
| Rectangles-filled  |       405,723 |       332,680 |       320,492 |       346,283 |     4,226,878 |         1,349 |
| Rectangles         |        11,644 |         9,288 |         8,491 |         9,252 |       102,561 |       270,222 |
| Circles-filled     |        76,621 |        52,946 |        42,600 |        48,292 |       883,873 |     1,460,379 |
| Circles            |       118,051 |        42,096 |        24,819 |        44,739 |       487,466 |     1,153,872 |
| Arcs-filled        |           N/A |        34,012 |        20,966 |           N/A |           N/A |           N/A |
| Arcs               |           N/A |        67,269 |        45,842 |           N/A |           N/A |           N/A |
| Triangles-filled   |       150,826 |       118,745 |       105,557 |       117,507 |     1,280,068 |           N/A |
| Triangles          |        58,800 |        24,756 |        15,571 |        18,698 |           N/A |           N/A |
| Rounded rects-fill |       407,662 |       338,396 |       318,646 |       345,494 |     4,292,533 |    10,778,601 |
| Rounded rects      |        43,136 |        21,980 |        12,844 |        24,604 |       221,214 |       283,148 |
