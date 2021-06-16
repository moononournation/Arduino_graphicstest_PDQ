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
#include <Ucglib.h>

Ucglib_ILI9341_18x240x320_HWSPI ucg(TFT_DC, TFT_CS, TFT_RST);

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.println("Ucglib library Test!");

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  SPI.setFrequency(SPI_FREQUENCY);
  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
  ucg.setFont(ucg_font_5x8_mr);

  w = 240;
  h = 320;
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
  serialOut(F("Arcs (filled)\t"), usecFilledArcs, 0, false);

  int32_t usecArcs = -1; // testArcs();
  serialOut(F("Arcs (outline)\t"), usecArcs, 0, false);

  int32_t usecFilledTrangles = testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 100, false);

  int32_t usecTriangles = -1; // testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 0, true);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    ucg.setColor(0, 0, c);
    ucg.drawHLine(0, i, w);
    c += d;
    if (c <= 4 || c >= 11)
    {
      d = -d;
    }
  }

  uint16_t y = 12;

  ucg.setScale2x2();
  ucg.setColor(255, 0, 255); // MAGENTA
  ucg.setPrintPos(0, y / 2);
  ucg.print(F("Ucglib PDQ"));

  if (h > w)
  {
    ucg.undoScale();
    y += 9;
    ucg.setColor(0, 255, 0); // GREEN
    ucg.setPrintPos(0, y);
    ucg.print(F("Benchmark       "));
    y += 9;
    ucg.setScale2x2();
    ucg.setPrintPos(70, y / 2);
    ucg.print(F("micro-secs"));
  }

  y += 9;
  ucg.undoScale();
  printnice(F("Screen fill "), usecFillScreen, y);
  y += 18;
  printnice(F("Text        "), usecText, y);
  y += 18;
  printnice(F("Pixels      "), usecPixels, y);
  y += 18;
  printnice(F("Lines       "), usecLines, y);
  y += 18;
  printnice(F("H/V Lines   "), usecFastLines, y);
  y += 18;
  printnice(F("Rectangles F"), usecFilledRects, y);
  y += 18;
  printnice(F("Rectangles  "), usecRects, y);
  y += 18;
  printnice(F("Circles F   "), usecFilledCircles, y);
  y += 18;
  printnice(F("Circles     "), usecCircles, y);
  y += 18;
  printnice(F("Arcs F      "), usecFilledArcs, y);
  y += 18;
  printnice(F("Arcs        "), usecArcs, y);
  y += 18;
  printnice(F("Triangles F "), usecFilledTrangles, y);
  y += 18;
  printnice(F("Triangles   "), usecTriangles, y);
  y += 18;
  printnice(F("RoundRects F"), usecFilledRoundRects, y);
  y += 18;
  printnice(F("RoundRects  "), usecRoundRects, y);

  if (h > w)
  {
    y += 28;
    ucg.setScale2x2();
    ucg.setColor(0, 255, 0); // GREEN
    ucg.setPrintPos(0, y / 2);
    ucg.print(F("Benchmark Complete!"));
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
    ucg.clearScreen();
  }
}

void printnice(const __FlashStringHelper *item, int32_t v, uint32_t y)
{
  ucg.undoScale();
  ucg.setColor(0, 255, 255); // CYAN
  ucg.setPrintPos(0, y);
  ucg.print(item);

  y += 9;
  ucg.setScale2x2();
  ucg.setColor(255, 255, 0); // YELLOW
  if (v < 0)
  {
    ucg.setPrintPos(90, y / 2);
    ucg.print(F("N / A"));
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
    ucg.setPrintPos(60, y / 2);
    ucg.print(str);
  }
  ucg.undoScale();
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  ucg.setColor(255, 255, 255); // WHITE
  ucg.drawBox(0, 0, w, h);
  ucg.setColor(255, 0, 0); // RED
  ucg.drawBox(0, 0, w, h);
  ucg.setColor(0, 255, 0); // GREEN
  ucg.drawBox(0, 0, w, h);
  ucg.setColor(0, 0, 255); // BLUE
  ucg.drawBox(0, 0, w, h);
  ucg.setColor(0, 0, 0); // BLACK
  ucg.drawBox(0, 0, w, h);

  return micros() - start;
}

int32_t testText()
{
  uint32_t y = 10;
  uint32_t start = micros_start();

  ucg.undoScale();
  ucg.setColor(255, 255, 255); // WHITE
  ucg.setPrintPos(0, y);
  ucg.print(F("Hello World!"));

  y += 18;
  ucg.setScale2x2();
  ucg.setColor(255, 0, 0); // RED
  ucg.setPrintPos(0, y / 2);
  ucg.print(F("RED"));
  ucg.setColor(0, 255, 0); // GREEN
  ucg.setPrintPos(40, y / 2);
  ucg.print(F("GREEN"));
  ucg.setColor(0, 0, 255); // BLUE
  ucg.setPrintPos(80, y / 2);
  ucg.print(F("BLUE"));

  y += 18;
  ucg.setColor(255, 255, 0); // YELLOW
  ucg.setPrintPos(0, y / 2);
  ucg.print(1234.56);

  y += 18;
  ucg.setColor(255, 255, 255); // WHITE
  ucg.setPrintPos(0, y / 2);
  ucg.print(0xDEADBEEF, HEX);

  y += 18;
  ucg.setColor(0, 255, 255); // CYAN
  ucg.setPrintPos(0, y / 2);
  ucg.print(F("Groop"));

  y += 18;
  ucg.setColor(255, 0, 255); // MAGENTA
  ucg.setPrintPos(0, y / 2);
  ucg.print(F("I implore thee,"));

  y += 18;
  ucg.undoScale();
  ucg.setColor(0, 0, 123); // NAVY
  ucg.setPrintPos(0, y);
  ucg.print(F("my foonting turlingdromes."));

  y += 18;
  ucg.setColor(0, 125, 0); // DARKGREEN
  ucg.setPrintPos(0, y);
  ucg.print(F("And hooptiously drangle me"));

  y += 18;
  ucg.setColor(0, 125, 123); // DARKCYAN
  ucg.setPrintPos(0, y);
  ucg.print(F("with crinkly bindlewurdles,"));

  y += 18;
  ucg.setColor(123, 0, 0); // MAROON
  ucg.setPrintPos(0, y);
  ucg.print(F("Or I will rend thee"));

  y += 18;
  ucg.setColor(123, 0, 123); // PURPLE
  ucg.setPrintPos(0, y);
  ucg.print(F("in the gobberwarts"));

  y += 18;
  ucg.setColor(123, 125, 0); // OLIVE
  ucg.setPrintPos(0, y);
  ucg.print(F("with my blurglecruncheon,"));

  y += 18;
  ucg.setColor(123, 125, 123); // DARKGREY
  ucg.setPrintPos(0, y);
  ucg.print(F("see if I don't!"));

  y += 18;
  ucg.setScale2x2();
  ucg.setColor(0, 255, 0); // GREEN
  ucg.setPrintPos(0, y / 2);
  ucg.print(F("Woot!"));

  ucg.undoScale();

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      ucg.setColor(x << 3, y << 3, x * y);
      ucg.drawPixel(x, y);
    }
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  ucg.setColor(0, 0, 255); // BLUE

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  ucg.setColor(255, 0, 0); // RED
  for (y = 0; y < h; y += 5)
  {
    ucg.drawHLine(0, y, w);
  }
  ucg.setColor(0, 0, 255); // BLUE
  for (x = 0; x < w; x += 5)
  {
    ucg.drawVLine(x, 0, h);
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

    ucg.setColor(i, i, 0);
    ucg.drawBox(cx1 - i2, cy1 - i2, i, i);
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  ucg.setColor(0, 255, 0); // GREEN
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    ucg.drawFrame(cx - i2, cy - i2, i, i);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  ucg.setColor(255, 0, 255); // MAGENTA
  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      ucg.drawDisc(x, y, radius, UCG_DRAW_ALL);
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

  ucg.setColor(255, 255, 255); // WHITE
  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      ucg.drawCircle(x, y, radius, UCG_DRAW_ALL);
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
    ucg.setColor(0, i, i);
    ucg.drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i);
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
    ucg.setColor(0, i, 0);
    ucg.drawRBox(cx - i2, cy - i2, i, i, i / 8);
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
    ucg.setColor(i, 0, 0);
    ucg.drawRFrame(cx - i2, cy - i2, i, i, i / 8);
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
