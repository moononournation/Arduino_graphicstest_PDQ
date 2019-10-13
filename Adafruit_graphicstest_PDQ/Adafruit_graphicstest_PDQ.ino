/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#if defined(ESP32)
#define TFT_CS 5
#define TFT_DC 27
#define TFT_RST 33
#define TFT_BL 22
#elif defined(ESP8266)
#define TFT_CS 15
#define TFT_DC 5
#define TFT_RST -1
#define TFT_BL 4
#else
#define TFT_CS 20
#define TFT_DC 19
#define TFT_RST 18
#define TFT_BL 10
#endif
#define TFT_ROTATION 0

// Use hardware SPI
Adafruit_ILI9341 *tft = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

unsigned long total = 0;
unsigned long tn = 0;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.println("Adafruit GFX library Test!");

  // tft->begin();
  tft->begin(40000000); /* specify data bus speed */
  tft->setRotation(TFT_ROTATION);

  w = tft->width();
  h = tft->height();
  n = min(w, h);
  n1 = min(w, h) - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = min(cx1, cy1) - 1;
  tsa = (w <= 160) ? 1 : ((w <= 240) ? 2 : 3); // text size A
  tsb = (w <= 240) ? 1 : 2;                    // text size B
  tsc = (w <= 220) ? 1 : 2;                    // text size C
  ds = (w <= 160) ? 9 : 12;                    // digit size

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif
}

void loop(void)
{

  Serial.println(F("Benchmark                Time (microseconds)"));

  uint32_t usecFillScreen = testFillScreen();
  Serial.print(F("Screen fill              "));
  Serial.println(usecFillScreen);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecText = testText();
  Serial.print(F("Text                     "));
  Serial.println(usecText);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecPixels = testPixels();
  Serial.print(F("Pixels                   "));
  Serial.println(usecPixels);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecLines = testLines(ILI9341_BLUE);
  Serial.print(F("Lines                    "));
  Serial.println(usecLines);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecFastLines = testFastLines(ILI9341_RED, ILI9341_BLUE);
  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(usecFastLines);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecFilledRects = testFilledRects(ILI9341_YELLOW, ILI9341_MAGENTA);
  Serial.print(F("Rectangles (filled)      "));
  Serial.println(usecFilledRects);
  delay(100);

  uint32_t usecRects = testRects(ILI9341_GREEN);
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(usecRects);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecFilledCircles = testFilledCircles(10, ILI9341_MAGENTA);
  Serial.print(F("Circles (filled)         "));
  Serial.println(usecFilledCircles);
  delay(100);

  uint32_t usecCircles = testCircles(10, ILI9341_WHITE);
  Serial.print(F("Circles (outline)        "));
  Serial.println(usecCircles);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecFilledTrangles = testFilledTriangles();
  Serial.print(F("Triangles (filled)       "));
  Serial.println(usecFilledTrangles);
  delay(100);

  uint32_t usecTriangles = testTriangles();
  Serial.print(F("Triangles (outline)      "));
  Serial.println(usecTriangles);
  delay(100);

  tft->fillScreen(ILI9341_BLACK);
  uint32_t usecFilledRoundRects = testFilledRoundRects();
  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(usecFilledRoundRects);
  delay(100);

  uint32_t usecRoundRects = testRoundRects();
  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(usecRoundRects);
  delay(100);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    tft->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
      d = -d;
  }

  tft->setCursor(0, 0);
  tft->setTextColor(ILI9341_MAGENTA);
  tft->setTextSize(tsa);
  tft->println(F("Adafruit GFX"));
  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE);
  tft->println(F(""));

  if (h > w)
  {
    tft->setTextColor(ILI9341_GREEN);
    tft->setTextSize(tsb);
    tft->print(F("Benchmark "));
    tft->setTextSize(tsc);
    if (ds == 12)
    {
      tft->print(F("   "));
    }
    tft->println(F("micro-secs"));
    tft->setTextSize(1);
    tft->println(F(""));
    tft->setTextColor(ILI9341_YELLOW);
  }

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Screen fill "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFillScreen);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Text        "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecText);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Pixels      "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecPixels);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Lines       "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecLines);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("H/V Lines   "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFastLines);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Rectangles F"));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledRects);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Rectangles  "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecRects);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Circles F   "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledCircles);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Circles     "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecCircles);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Triangles F "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledTrangles);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("Triangles   "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecTriangles);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("RoundRects F"));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecFilledRoundRects);

  tft->setTextColor(ILI9341_CYAN);
  tft->setTextSize(tsb);
  tft->print(F("RoundRects  "));
  tft->setTextColor(ILI9341_YELLOW);
  tft->setTextSize(tsc);
  printnice(usecRoundRects);

  if (h > w)
  {
    tft->setTextSize(1);
    tft->println(F(""));
    tft->setTextColor(ILI9341_GREEN);
    tft->setTextSize(tsc);
    tft->print(F("Benchmark Complete!"));
  }

  delay(60 * 1000L);
}

void printnice(int32_t v)
{
  char str[32] = {0};
  sprintf(str, "%lu", v);
  for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
  {
    memmove(p + 1, p, strlen(p) + 1);
    *p = ',';
  }
  while (strlen(str) < ds)
  {
    memmove(str + 1, str, strlen(str) + 1);
    *str = ' ';
  }
  tft->println(str);
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

uint32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  tft->fillScreen(ILI9341_WHITE);
  tft->fillScreen(ILI9341_RED);
  tft->fillScreen(ILI9341_GREEN);
  tft->fillScreen(ILI9341_BLUE);
  tft->fillScreen(ILI9341_BLACK);

  return (micros() - start) / 5;
}

uint32_t testText()
{
  uint32_t start = micros_start();
  tft->setCursor(0, 0);
  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft->println(F("Hello World!"));
  tft->setTextSize(2);
  tft->setTextColor(tft->color565(0xff, 0x00, 0x00));
  tft->print(F("RED "));
  tft->setTextColor(tft->color565(0x00, 0xff, 0x00));
  tft->print(F("GREEN "));
  tft->setTextColor(tft->color565(0x00, 0x00, 0xff));
  tft->println(F("BLUE"));
  tft->setTextSize(tsa);
  tft->setTextSize(3);
  tft->setTextColor(ILI9341_YELLOW);
  tft->println(1234.56);
  tft->setTextColor(ILI9341_WHITE);
  tft->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);
  tft->setTextColor(ILI9341_CYAN, ILI9341_WHITE);
  tft->println(F("Groop,"));
  tft->setTextSize(tsc);
  tft->setTextColor(ILI9341_MAGENTA, ILI9341_WHITE);
  tft->println(F("I implore thee,"));
  tft->setTextSize(1);
  tft->setTextColor(ILI9341_NAVY, ILI9341_WHITE);
  tft->println(F("my foonting turlingdromes."));
  tft->setTextColor(ILI9341_DARKGREEN, ILI9341_WHITE);
  tft->println(F("And hooptiously drangle me"));
  tft->setTextColor(ILI9341_DARKCYAN, ILI9341_WHITE);
  tft->println(F("with crinkly bindlewurdles,"));
  tft->setTextColor(ILI9341_MAROON, ILI9341_WHITE);
  tft->println(F("Or I will rend thee"));
  tft->setTextColor(ILI9341_PURPLE, ILI9341_WHITE);
  tft->println(F("in the gobberwartsb"));
  tft->setTextColor(ILI9341_OLIVE, ILI9341_WHITE);
  tft->println(F("with my blurglecruncheon,"));
  tft->setTextColor(ILI9341_DARKGREY, ILI9341_WHITE);
  tft->println(F("see if I don't!"));
  if (h > 160)
  {
    tft->setTextColor(ILI9341_ORANGE);
    tft->setTextSize(4);
    tft->println(F("Size 4"));
  }
  if (h > 220)
  {
    tft->setTextColor(ILI9341_GREENYELLOW);
    tft->setTextSize(5);
    tft->println(F("Size 5"));
  }
  if (h > 240)
  {
    tft->setTextColor(ILI9341_PINK);
    tft->setTextSize(6);
    tft->println(F("Size 6"));
  }
  uint32_t t = micros() - start;
  delay(3000); // delay for verifing the text
  return t;
}

uint32_t testPixels()
{
  uint32_t start = micros_start();

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      tft->drawPixel(x, y, tft->color565(x << 3, y << 3, x * y));
    }
  }

  return micros() - start;
}

uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t x1, y1, x2, y2;

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }
  t += micros() - start;

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
    tft->drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    tft->drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t i, i2;

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    tft->fillRect(cx1 - i2, cy1 - i2, i, i, color1);

    t += micros() - start;

    // Outlines are not included in timing results
    tft->drawRect(cx - i2, cy - i2, i, i, color2);
  }

  return t;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft->drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft->fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w1 = w + radius;
  int32_t h1 = h + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      tft->drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    start = micros_start();
    tft->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      tft->color565(0, i, i));
    t += micros() - start;
    tft->drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      tft->color565(i, i, 0));
  }

  return t;
}

uint32_t testTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = 0; i < cn; i += 5)
  {
    tft->drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        tft->color565(0, 0, i));
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    tft->fillRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, tft->color565(0, i, 0));
  }

  return micros() - start;
}

uint32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 0; i < n1; i += 6)
  {
    i2 = i / 2;
    tft->drawRoundRect(cx1 - i2, cy1 - i2, i, i, i / 8, tft->color565(i, 0, 0));
  }

  return micros() - start;
}

/***************************************************
  Original sketch text:

  This is an example sketch for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
