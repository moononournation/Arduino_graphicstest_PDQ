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
#include <UTFT.h>

UTFT tft(ILI9341_S5P, TFT_CS, TFT_RST, TFT_DC);
extern uint8_t SmallFont[];
extern uint8_t BigFont[];

uint32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
  {
    // wait and do nothing
  }

  Serial.println("UTFT-ESP library Test!");

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);
  tft.InitLCD(PORTRAIT);
  SPI.setFrequency(SPI_FREQUENCY);

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

  int32_t usecFilledTrangles = -1; // testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 0, false);

  int32_t usecTriangles = -1; // testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 0, false);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    tft.setColor(c);
    tft.drawHLine(0, i, w);
    c += d;
    if (c <= 4 || c >= 11)
    {
      d = -d;
    }
  }

  uint16_t y = 0;

  tft.setFont(BigFont);
  tft.setColor(VGA_PURPLE);
  tft.setBackColor(VGA_TRANSPARENT);
  tft.print(F("UTFT-ESP PDQ"), 0, y);

  if (h > w)
  {
    y += 28;
    tft.setFont(SmallFont);
    tft.setColor(VGA_GREEN);
    tft.print(F("Benchmark"), 0, y);
    tft.setFont(BigFont);
    tft.setColor(VGA_GREEN);
    tft.print(F("micro-secs"), 81, y);
  }

  y += 28;
  tft.setFont(SmallFont);
  printnice(F("Screen fill "), usecFillScreen, y);
  y += 14;
  printnice(F("Text        "), usecText, y);
  y += 14;
  printnice(F("Pixels      "), usecPixels, y);
  y += 14;
  printnice(F("Lines       "), usecLines, y);
  y += 14;
  printnice(F("H/V Lines   "), usecFastLines, y);
  y += 14;
  printnice(F("Rectangles F"), usecFilledRects, y);
  y += 14;
  printnice(F("Rectangles  "), usecRects, y);
  y += 14;
  printnice(F("Circles F   "), usecFilledCircles, y);
  y += 14;
  printnice(F("Circles     "), usecCircles, y);
  y += 14;
  printnice(F("Arcs F      "), usecFilledArcs, y);
  y += 14;
  printnice(F("Arcs        "), usecArcs, y);
  y += 14;
  printnice(F("Triangles F "), usecFilledTrangles, y);
  y += 14;
  printnice(F("Triangles   "), usecTriangles, y);
  y += 14;
  printnice(F("RoundRects F"), usecFilledRoundRects, y);
  y += 14;
  printnice(F("RoundRects  "), usecRoundRects, y);

  if (h > w)
  {
    y += 28;
    tft.setFont(BigFont);
    tft.setColor(VGA_GREEN);
    tft.print(F("Benchmark Complete!"), 0, y);
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
    tft.clrScr();
  }
}

void printnice(const __FlashStringHelper *item, int32_t v, uint32_t y)
{

  tft.setFont(SmallFont);
  tft.setColor(VGA_AQUA);
  tft.print(item, 0, y);

  tft.setFont(BigFont);
  tft.setColor(VGA_YELLOW);
  if (v < 0)
  {
    tft.print(F("N / A"), 145, y);
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
    tft.print(str, 49, y);
  }
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  tft.fillScr(VGA_WHITE);
  tft.fillScr(VGA_RED);
  tft.fillScr(VGA_GREEN);
  tft.fillScr(VGA_BLUE);
  tft.fillScr(VGA_BLACK);

  return micros() - start;
}

int32_t testText()
{
  uint32_t y = 0;
  uint32_t start = micros_start();
  tft.setFont(SmallFont);
  tft.setColor(VGA_WHITE);
  tft.setBackColor(VGA_BLACK);
  tft.print(F("Hello World!"), 0, y);

  y += 12;
  tft.setFont(BigFont);
  tft.setColor(0xff, 0x00, 0x00);
  tft.setBackColor(VGA_TRANSPARENT);
  tft.print(F("RED"), 0, y);
  tft.setColor(0x00, 0xff, 0x00);
  tft.print(F("GREEN"), 80, y);
  tft.setColor(0x00, 0x00, 0xff);
  tft.print(F("BLUE"), 160, y);

  y += 16;
  tft.setColor(VGA_YELLOW);
  tft.printNumF(1234.56, 2, 0, y);

  y += 16;
  tft.setColor(VGA_WHITE);
  char str[10] = {0};
  sprintf(str, "%X", 0xDEADBEEF);
  tft.print(str, 0, y);

  y += 16;
  tft.setColor(VGA_AQUA);
  tft.setBackColor(VGA_WHITE);
  tft.print(F("Groop"), 0, y);

  y += 16;
  tft.setColor(VGA_PURPLE);
  tft.print(F("I implore thee,"), 0, y);

  y += 16;
  tft.setFont(SmallFont);
  tft.setColor(VGA_BLUE);
  tft.print(F("my foonting turlingdromes."), 0, y);

  y += 12;
  tft.setColor(VGA_GREEN);
  tft.print(F("And hooptiously drangle me"), 0, y);

  y += 12;
  tft.setColor(VGA_AQUA);
  tft.print(F("with crinkly bindlewurdles,"), 0, y);

  y += 12;
  tft.setColor(VGA_YELLOW);
  tft.print(F("Or I will rend thee"), 0, y);

  y += 12;
  tft.setColor(VGA_RED);
  tft.print(F("in the gobberwarts"), 0, y);

  y += 12;
  tft.setColor(VGA_PURPLE);
  tft.print(F("with my blurglecruncheon,"), 0, y);

  y += 12;
  tft.setColor(VGA_BLUE);
  tft.print(F("see if I don't!"), 0, y);

  y += 12;
  tft.setFont(BigFont);
  tft.setColor(VGA_GREEN);
  tft.setBackColor(VGA_TRANSPARENT);
  tft.print(F("Woot!"), 0, y);

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      tft.setColor(x << 3, y << 3, x * y);
      tft.drawPixel(x, y);
    }
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  tft.setColor(VGA_BLUE);

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  tft.setColor(VGA_RED);
  for (y = 0; y < h; y += 5)
  {
    tft.drawHLine(0, y, w);
  }
  tft.setColor(VGA_BLUE);
  for (x = 0; x < w; x += 5)
  {
    tft.drawVLine(x, 0, h);
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

    tft.setColor(i, i, 0);
    tft.fillRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
  }

  return micros() - start;
}

int32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  start = micros_start();

  tft.setColor(VGA_GREEN);
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
  }

  return micros() - start;
}

int32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  start = micros_start();

  tft.setColor(VGA_PURPLE);
  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft.fillCircle(x, y, radius);
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

  tft.setColor(VGA_WHITE);
  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      tft.drawCircle(x, y, radius);
    }
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
    tft.setColor(0, i, 0);
    tft.fillRoundRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
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
    tft.setColor(i, 0, 0);
    tft.drawRoundRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
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
