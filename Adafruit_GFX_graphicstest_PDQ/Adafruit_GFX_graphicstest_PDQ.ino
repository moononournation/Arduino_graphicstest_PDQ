/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

#define TFT_BL 23
#define TFT_CS 5
#define TFT_RST 18
#define TFT_DC 19
#define TFT_MOSI 21
#define TFT_SCLK 22
#define TFT_MISO 27
#define SPI_FREQUENCY 40000000

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Use hardware SPI
Adafruit_ILI9341 *tft = new Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.println("Adafruit GFX library Test!");

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft->begin(SPI_FREQUENCY); /* specify data bus speed */

  w = tft->width();
  h = tft->height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;

#ifdef TFT_BL
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);
#endif
}

static inline uint32_t micros_start() __attribute__((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}

void loop(void)
{
  Serial.println(F("Benchmark\tmicro-secs"));

  int32_t usecFillScreen = testFillScreen();
  serialOut(F("Screen fill\t"), usecFillScreen, 100, true);

  int32_t usecText = testText();
  serialOut(F("Text\t"), usecText, 3000, true);

  int32_t usecPixels = testPixels();
  serialOut(F("Pixels\t"), usecPixels, 100, true);

  int32_t usecLines = testLines();
  serialOut(F("Lines\t"), usecLines, 100, true);

  int32_t usecFastLines = testFastLines();
  serialOut(F("Horiz/Vert Lines\t"), usecFastLines, 100, true);

  int32_t usecFilledRects = testFilledRects();
  serialOut(F("Rectangles (filled)\t"), usecFilledRects, 100, false);

  int32_t usecRects = testRects();
  serialOut(F("Rectangles (outline)\t"), usecRects, 100, true);

  int32_t usecFilledCircles = testFilledCircles(10);
  serialOut(F("Circles (filled)\t"), usecFilledCircles, 100, false);

  int32_t usecCircles = testCircles(10);
  serialOut(F("Circles (outline)\t"), usecCircles, 100, true);

  int32_t usecFilledArcs = -1; // testFillArcs();
  serialOut(F("Arcs (filled)\t"), usecFilledArcs, 100, false);

  int32_t usecArcs = -1; // testArcs();
  serialOut(F("Arcs (outline)\t"), usecArcs, 100, true);

  int32_t usecFilledTrangles = testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 100, false);

  int32_t usecTriangles = testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 100, true);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    tft->drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
    {
      d = -d;
    }
  }

  tft->setCursor(0, 0);

  tft->setTextSize(2);
  tft->setTextColor(ILI9341_MAGENTA);
  tft->println(F("Adafruit GFX PDQ"));

  if (h > w)
  {
    tft->setTextSize(1);
    tft->setTextColor(ILI9341_GREEN);
    tft->print(F("\nBenchmark       "));
    tft->setTextSize(2);
    tft->println(F("micro-secs"));
  }

  tft->setTextSize(1);
  printnice(F("Screen fill "), usecFillScreen);
  printnice(F("Text        "), usecText);
  printnice(F("Pixels      "), usecPixels);
  printnice(F("Lines       "), usecLines);
  printnice(F("H/V Lines   "), usecFastLines);
  printnice(F("Rectangles F"), usecFilledRects);
  printnice(F("Rectangles  "), usecRects);
  printnice(F("Circles F   "), usecFilledCircles);
  printnice(F("Circles     "), usecCircles);
  printnice(F("Arcs F      "), usecFilledArcs);
  printnice(F("Arcs        "), usecArcs);
  printnice(F("Triangles F "), usecFilledTrangles);
  printnice(F("Triangles   "), usecTriangles);
  printnice(F("RoundRects F"), usecFilledRoundRects);
  printnice(F("RoundRects  "), usecRoundRects);

  if (h > w)
  {
    tft->setTextSize(2);
    tft->setTextColor(ILI9341_GREEN);
    tft->print(F("\nBenchmark Complete!"));
  }

  delay(60 * 1000L);
}

void serialOut(const __FlashStringHelper *item, int32_t v, uint32_t d, bool clear)
{
  Serial.print(item);
  if (v < 0)
  {
    Serial.println(F("N/A"));
  }
  else
  {
    Serial.println(v);
  }
  delay(d);
  if (clear)
  {
    tft->fillScreen(ILI9341_BLACK);
  }
}

void printnice(const __FlashStringHelper *item, int32_t v)
{
  tft->setTextSize(1);
  tft->setTextColor(ILI9341_CYAN);
  tft->print(item);

  tft->setTextSize(2);
  tft->setTextColor(ILI9341_YELLOW);
  if (v < 0)
  {
    tft->println(F("      N / A"));
  }
  else
  {
    char str[32] = {0};
    sprintf(str, "%lu", v);
    for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
    {
      memmove(p + 1, p, strlen(p) + 1);
      *p = ',';
    }
    while (strlen(str) < 12)
    {
      memmove(str + 1, str, strlen(str) + 1);
      *str = ' ';
    }
    tft->println(str);
  }
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  tft->fillScreen(ILI9341_WHITE);
  tft->fillScreen(ILI9341_RED);
  tft->fillScreen(ILI9341_GREEN);
  tft->fillScreen(ILI9341_BLUE);
  tft->fillScreen(ILI9341_BLACK);

  return micros() - start;
}

int32_t testText()
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

  tft->setTextColor(ILI9341_YELLOW);
  tft->println(1234.56);

  tft->setTextColor(ILI9341_WHITE);
  tft->println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

  tft->setTextColor(ILI9341_CYAN, ILI9341_WHITE);
  tft->println(F("Groop,"));

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

  tft->setTextSize(2);
  tft->setTextColor(ILI9341_RED);
  tft->println(F("Size 2"));

  tft->setTextSize(3);
  tft->setTextColor(ILI9341_ORANGE);
  tft->println(F("Size 3"));

  tft->setTextSize(4);
  tft->setTextColor(ILI9341_YELLOW);
  tft->println(F("Size 4"));

  tft->setTextSize(5);
  tft->setTextColor(ILI9341_GREENYELLOW);
  tft->println(F("Size 5"));

  tft->setTextSize(6);
  tft->setTextColor(ILI9341_GREEN);
  tft->println(F("Size 6"));

  tft->setTextSize(7);
  tft->setTextColor(ILI9341_BLUE);
  tft->println(F("Size 7"));

  tft->setTextSize(8);
  tft->setTextColor(ILI9341_PURPLE);
  tft->println(F("Size 8"));

  tft->setTextSize(9);
  tft->setTextColor(ILI9341_PINK);
  tft->println(F("Size 9"));

  return micros() - start;
}

int32_t testPixels()
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

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft->drawLine(x1, y1, x2, y2, ILI9341_BLUE);
  }

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
  {
    tft->drawFastHLine(0, y, w, ILI9341_RED);
  }
  for (x = 0; x < w; x += 5)
  {
    tft->drawFastVLine(x, 0, h, ILI9341_BLUE);
  }

  return micros() - start;
}

int32_t testFilledRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    tft->fillRect(cx - i2, cy - i2, i, i, tft->color565(i, i, 0));
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft->drawRect(cx - i2, cy - i2, i, i, ILI9341_GREEN);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft->fillCircle(x, y, radius, ILI9341_MAGENTA);
    }
  }

  return micros() - start;
}

int32_t testCircles(uint8_t radius)
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
      tft->drawCircle(x, y, radius, ILI9341_WHITE);
    }
  }

  return micros() - start;
}

int32_t testFilledTriangles()
{
  uint32_t start;
  int32_t i;

  start = micros_start();

  for (i = cn1; i > 10; i -= 5)
  {
    tft->fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                      tft->color565(0, i, i));
  }

  return micros() - start;
}

int32_t testTriangles()
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

int32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    tft->fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft->color565(0, i, 0));
  }

  return micros() - start;
}

int32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  for (i = 20; i < n1; i += 6)
  {
    i2 = i / 2;
    tft->drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft->color565(i, 0, 0));
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
