/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

#include "SPI.h"
#include "Ucglib.h"

uint32_t w = 240;
uint32_t h = 320;
uint32_t n = min(w, h);
uint32_t n1 = min(w, h) - 1;
uint32_t cx = w / 2;
uint32_t cy = h / 2;
uint32_t cx1 = cx - 1;
uint32_t cy1 = cy - 1;
uint32_t cn = min(cx1, cy1);
uint32_t cn1 = min(cx1, cy1) - 1;

// Use hardware SPI
Ucglib_ILI9341_18x240x320_HWSPI ucg(/*cd=*/ 16, /*cs=*/ 5, /*reset=*/ 17);

unsigned long total = 0;
unsigned long tn = 0;
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(""); Serial.println("");
  Serial.println("Ucglib library Test!");

  ucg.begin(UCG_FONT_MODE_TRANSPARENT);
//  ucg.setRotate180();
  ucg.setFont(ucg_font_7x13_mr);
  ucg.clearScreen();
}

void loop(void)
{
  Serial.println(F("Benchmark                Time (microseconds)"));

  //	uint32_t usecHaD = testHaD();
  //	Serial.print(F("HaD pushColor            "));
  //	Serial.println(usecHaD);
  //	delay(100);

  uint32_t usecFillScreen = testFillScreen();
  Serial.print(F("Screen fill              "));
  Serial.println(usecFillScreen);
  delay(100);

  uint32_t usecText = testText();
  Serial.print(F("Text                     "));
  Serial.println(usecText);
  delay(100);

  uint32_t usecPixels = testPixels();
  Serial.print(F("Pixels                   "));
  Serial.println(usecPixels);
  delay(100);

  uint32_t usecLines = testLines();
  Serial.print(F("Lines                    "));
  Serial.println(usecLines);
  delay(100);

  uint32_t usecFastLines = testFastLines();
  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(usecFastLines);
  delay(100);

  uint32_t usecRects = testRects();
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(usecRects);
  delay(100);

  uint32_t usecFilledRects = testFilledRects();
  Serial.print(F("Rectangles (filled)      "));
  Serial.println(usecFilledRects);
  delay(100);

  uint32_t usecFilledCircles = testFilledCircles(10);
  Serial.print(F("Circles (filled)         "));
  Serial.println(usecFilledCircles);
  delay(100);

  uint32_t usecCircles = testCircles(10);
  Serial.print(F("Circles (outline)        "));
  Serial.println(usecCircles);
  delay(100);

//  uint32_t usecTriangles = testTriangles();
//  Serial.print(F("Triangles (outline)      "));
//  Serial.println(usecTriangles);
//  delay(100);

  uint32_t usecFilledTrangles = testFilledTriangles();
  Serial.print(F("Triangles (filled)       "));
  Serial.println(usecFilledTrangles);
  delay(100);

  uint32_t usecRoundRects = testRoundRects();
  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(usecRoundRects);
  delay(100);

  uint32_t usedFilledRoundRects = testFilledRoundRects();
  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(usedFilledRoundRects);
  delay(100);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    ucg.setColor(0, 0, c * 8);
    ucg.drawHLine(0, i, w);
    c += d;
    if (c <= 4 || c >= 11)
      d = -d;
  }

  uint16_t y = 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(255, 0, 255); // MAGENTA
  ucg.print(F("Ucglib PDQ test"));

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 0); // GREEN
  ucg.print(F("Benchmark             microseconds"));

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("HaD pushColor      "));
  ucg.setColor(255, 255, 0); // YELLOW
//	printnice(usecHaD);
  ucg.print(F("          N/A"));

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Screen fill        "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecFillScreen);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Text               "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecText);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Pixels             "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecPixels);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Lines              "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecLines);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Horiz/Vert Lines   "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecFastLines);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Rectangles         "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecRects);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Rectangles-filled  "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecFilledRects);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Circles            "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecCircles);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Circles-filled     "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecFilledCircles);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Triangles          "));
  ucg.setColor(255, 255, 0); // YELLOW
//  printnice(usecTriangles);
  ucg.print(F("          N/A"));

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Triangles-filled   "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecFilledTrangles);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Rounded rects      "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usecRoundRects);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 255); // CYAN
  ucg.print(F("Rounded rects-fill "));
  ucg.setColor(255, 255, 0); // YELLOW
  printnice(usedFilledRoundRects);

  y += 18;
  ucg.setPrintPos(0, y);
  ucg.setColor(0, 255, 0); // GREEN
  ucg.print(F("Benchmark Complete!"));

  delay(60 * 1000L);
}

void printnice(int32_t v)
{
  char	str[32] = { 0 };
  sprintf(str, "%lu", v);
  for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
  {
    memmove(p + 1, p, strlen(p) + 1);
    *p = ',';

  }
  while (strlen(str) < 15)
  {
    memmove(str + 1, str, strlen(str) + 1);
    *str = ' ';
  }
  ucg.print(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
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

  return (micros() - start) / 5;
}

uint32_t testText()
{
  uint32_t y = 16;
  ucg.clearScreen();
  uint32_t start = micros_start();
  ucg.setPrintPos(0, y);

  ucg.setColor(255, 255, 255); // WHITE
  ucg.setColor(1, 0, 0, 0); // BLACK
  ucg.println(F("Hello World!"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.setScale2x2();
  ucg.setColor(255, 0, 0); // RED
  ucg.print(F("RED "));
  ucg.setColor(0, 255, 0); // GREEN
  ucg.print(F("GREEN "));
  ucg.setColor(0, 0, 255); // BLUE
  ucg.println(F("BLUE"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.setColor(255, 255, 0); // YELLOW
  ucg.println(1234.56);
  y += 18;
  ucg.setPrintPos(0, y);

  //	ucg.setTextColor(TFT_RED);
  //	ucg.setTextSize(3);
  //	ucg.println(0xDEADBEEF, HEX);
  //	ucg.println();

  //	ucg.setTextColor(TFT_GREEN);
  //	ucg.setTextSize(5);
  //	ucg.println(F("Groop"));
  //	ucg.setTextSize(2);

  ucg.println(F("I implore thee,"));
  y += 90;
  ucg.setPrintPos(0, y);

  ucg.undoScale();
  ucg.setColor(0, 255, 0); // GREEN
  ucg.println(F("my foonting turlingdromes."));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("And hooptiously drangle me"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("with crinkly bindlewurdles,"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("Or I will rend thee"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("in the gobberwarts"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("with my blurglecruncheon,"));
  y += 18;
  ucg.setPrintPos(0, y);

  ucg.println(F("see if I don't!"));

  //	ucg.println(F(""));
  //	ucg.println(F(""));
  //  ucg.setColor(255, 0, 255); // MAGENTA
  //	ucg.setTextSize(6);
  //	ucg.println(F("Woot!"));
  uint32_t t = micros() - start;
  delay(1000);
  return t;
}

uint32_t testPixels()
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

uint32_t testLines()
{
  uint32_t start, t;
  int32_t	x1, y1, x2, y2;

  ucg.clearScreen();

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  ucg.setColor(0, 0, 255); // BLUE
  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  ucg.setColor(0, 0, 0); // BLACK
  ucg.drawBox(0, 0, w, h);

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  ucg.setColor(0, 0, 255); // BLUE
  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  t += micros() - start;

  ucg.setColor(0, 0, 0); // BLACK
  ucg.drawBox(0, 0, w, h);

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  ucg.setColor(0, 0, 255); // BLUE
  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }
  t += micros() - start;

  ucg.setColor(0, 0, 0); // BLACK
  ucg.drawBox(0, 0, w, h);

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  ucg.setColor(0, 0, 255); // BLUE
  for (x2 = 0; x2 < w; x2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    ucg.drawLine(x1, y1, x2, y2);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  ucg.clearScreen();

  start = micros_start();

  ucg.setColor(255, 0, 0); // RED
  for (y = 0; y < h; y += 5)
    ucg.drawHLine(0, y, w);

  ucg.setColor(0, 0, 255); // BLUE
  for (x = 0; x < w; x += 5)
    ucg.drawVLine(x, 0, h);

  return micros() - start;
}

uint32_t testRects()
{
  uint32_t start;
  int32_t i, i2;

  ucg.clearScreen();
  start = micros_start();

  ucg.setColor(0, 255, 0); // GREEN
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    ucg.drawFrame(cx - i2, cy - i2, i, i);
  }

  return micros() - start;
}

uint32_t testFilledRects()
{
  uint32_t start, t = 0;
  int32_t i, i2;

  ucg.clearScreen();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    ucg.setColor(255, 255, 0); // YELLOW
    ucg.drawBox(cx1 - i2, cy1 - i2, i, i);

    t += micros() - start;

    // Outlines are not included in timing results
    ucg.setColor(255, 0, 255); // MAGENTA
    ucg.drawFrame(cx1 - i2, cy1 - i2, i, i);
  }

  return t;
}

uint32_t testFilledCircles(uint8_t radius)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  ucg.clearScreen();
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

uint32_t testCircles(uint8_t radius)
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

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;

  ucg.clearScreen();
  start = micros_start();

  for (i = cn; i > 10; i -= 5) {
    start = micros_start();
    ucg.setColor(0, i, i);
    ucg.drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i);
    t += micros() - start;
    ucg.setColor(i, i, 0);
    ucg.drawTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i);
  }

  return t;
}

uint32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  ucg.clearScreen();
  start = micros_start();

  for (i = 0; i < n1; i += 6)
  {
    i2 = i / 2;
    ucg.setColor(i, 0, 0);
    ucg.drawRFrame(cx1 - i2, cy1 - i2, i, i, i / 8);
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  ucg.clearScreen();
  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    ucg.setColor(0, i, 0);
    ucg.drawRBox(cx1 - i2, cy1 - i2, i, i, i / 8);
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

