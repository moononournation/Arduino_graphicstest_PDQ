/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.

  This sketch uses the GLCD font only.

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include <SPI.h>
#include <UTFT.h>

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

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
UTFT tft (ILI9341_S5P, 5, 17, 16); // CS, RESET, CD

unsigned long total = 0;
unsigned long tn = 0;
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(""); Serial.println("");
  Serial.println("TFT_eSPI library Test!");

  tft.InitLCD(PORTRAIT);
}

void loop(void)
{

  Serial.println(F("Benchmark                Time (microseconds)"));

//  uint32_t usecHaD = testHaD();
//  Serial.print(F("HaD pushColor            "));
//  Serial.println(usecHaD);
//  delay(100);

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

  uint32_t usecLines = testLines(VGA_BLUE);
  Serial.print(F("Lines                    "));
  Serial.println(usecLines);
  delay(100);

  uint32_t usecFastLines = testFastLines(VGA_RED, VGA_BLUE);
  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(usecFastLines);
  delay(100);

  uint32_t usecRects = testRects(VGA_GREEN);
  Serial.print(F("Rectangles (outline)     "));
  Serial.println(usecRects);
  delay(100);

  uint32_t usecFilledRects = testFilledRects(VGA_YELLOW, VGA_PURPLE);
  Serial.print(F("Rectangles (filled)      "));
  Serial.println(usecFilledRects);
  delay(100);

  uint32_t usecFilledCircles = testFilledCircles(10, VGA_PURPLE);
  Serial.print(F("Circles (filled)         "));
  Serial.println(usecFilledCircles);
  delay(100);

  uint32_t usecCircles = testCircles(10, VGA_WHITE);
  Serial.print(F("Circles (outline)        "));
  Serial.println(usecCircles);
  delay(100);

//  uint32_t usecTriangles = testTriangles();
//  Serial.print(F("Triangles (outline)      "));
//  Serial.println(usecTriangles);
//  delay(100);

//  uint32_t usecFilledTrangles = testFilledTriangles();
//  Serial.print(F("Triangles (filled)       "));
//  Serial.println(usecFilledTrangles);
//  delay(100);

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
    tft.setColor(c);
    tft.drawHLine(0, i, w);
    c += d;
    if (c <= 4 || c >= 11)
      d = -d;
  }

  uint16_t y = 0;
  tft.setColor(VGA_PURPLE);
//  tft.setFont(BigFont);

  tft.print(F("UTFT-ESP PDQ test"), 0, y);

  y += 18;
//  tft.setFont(SmallFont);
  tft.setColor(VGA_GREEN);
  tft.print(F(" Benchmark       microseconds"), 0, y);
  tft.setColor(VGA_YELLOW);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("HaD pushColor      "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
//  printnice(usecHaD, y);
  tft.print(F("N/A"), 200, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Screen fill        "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecFillScreen, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Text               "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecText, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Pixels             "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecPixels, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Lines              "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecLines, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Horiz/Vert Lines   "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecFastLines, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Rectangles         "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecRects, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Rectangles-filled  "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecFilledRects, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Circles            "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecCircles, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Circles-filled     "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecFilledCircles, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Triangles          "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
//  printnice(usecTriangles, y);
  tft.print(F("N/A"), 200, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Triangles-filled   "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
//  printnice(usecFilledTrangles, y);
  tft.print(F("N/A"), 200, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Rounded rects      "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usecRoundRects, y);

  y += 18;
  tft.setColor(VGA_AQUA);
//  tft.setFont(SmallFont);
  tft.print(F("Rounded rects-fill "), 0, y);
  tft.setColor(VGA_YELLOW);
//  tft.setFont(BigFont);
  printnice(usedFilledRoundRects, y);

  y += 18;
//  tft.setFont(SmallFont);
  tft.print(F(""), 0, y);

  y += 18;
  tft.setColor(VGA_GREEN);
//  tft.setFont(BigFont);
  tft.print(F("Benchmark Complete!"), 0, y);

  delay(60 * 1000L);
}

void printnice(int32_t v, uint32_t y)
{
  char	str[32] = { 0 };
  sprintf(str, "%lu", v);
  for (char *p = (str + strlen(str)) - 3; p > str; p -= 3)
  {
    memmove(p + 1, p, strlen(p) + 1);
    *p = ',';

  }
  while (strlen(str) < 10)
  {
    memmove(str + 1, str, strlen(str) + 1);
    *str = ' ';
  }
  tft.print(str, 160, y);
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
  tft.fillScr(VGA_WHITE);
  tft.fillScr(VGA_RED);
  tft.fillScr(VGA_GREEN);
  tft.fillScr(VGA_BLUE);
  tft.fillScr(VGA_BLACK);

  return (micros() - start) / 5;
}

uint32_t testText()
{
  uint32_t y = 16;
  tft.clrScr();
  uint32_t start = micros_start();
  tft.clrXY();
  tft.setColor(VGA_WHITE);
  tft.setBackColor(VGA_BLACK);
  tft.print(F("Hello World!"), 0, y);

  y += 18;
  tft.setFont(BigFont);
  tft.setColor(0xff, 0x00, 0x00);
  tft.print(F("RED "), 0, y);
  tft.setColor(0x00, 0xff, 0x00);
  tft.print(F("GREEN "), 80, y);
  tft.setColor(0x00, 0x00, 0xff);
  tft.print(F("BLUE"), 160, y);

  y += 18;
  tft.setColor(VGA_YELLOW);
  tft.printNumF(1234.56, 2, 0, y);

//  tft.setColor(VGA_RED);
//  tft.setTextSize(3);
//  tft.println(0xDEADBEEF, HEX);
//  tft.println();
//  tft.setColor(VGA_GREEN);
//  tft.setTextSize(5);
//  tft.println(F("Groop"));
//  tft.setFont(BigFont);

  y += 18;
  tft.print(F("I implore thee,"), 0, y);

  y += 18;
  tft.setColor(VGA_GREEN);
  tft.setFont(SmallFont);
  tft.print(F("my foonting turlingdromes."), 0, y);
  y += 18;
  tft.print(F("And hooptiously drangle me"), 0, y);
  y += 18;
  tft.print(F("with crinkly bindlewurdles,"), 0, y);
  y += 18;
  tft.print(F("Or I will rend thee"), 0, y);
  y += 18;
  tft.print(F("in the gobberwarts"), 0, y);
  y += 18;
  tft.print(F("with my blurglecruncheon,"), 0, y);
  y += 18;
  tft.print(F("see if I don't!"), 0, y);
//  tft.println(F(""));
//  tft.println(F(""));
//  tft.setColor(VGA_PURPLE);
//  tft.setTextSize(6);
//  tft.println(F("Woot!"));
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
      tft.setColor(x << 3, y << 3, x * y);
      tft.drawPixel(x, y);
    }
  }

  return micros() - start;
}


uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t	x1, y1, x2, y2;

  tft.clrScr();
  tft.setColor(color);

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

  t = micros() - start; // fillScreen doesn't count against timing

  tft.clrScr();

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  t += micros() - start;

  tft.clrScr();

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

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
  t += micros() - start;

  tft.clrScr();

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    tft.drawLine(x1, y1, x2, y2);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;

  tft.clrScr();

  start = micros_start();

  tft.setColor(color1);
  for (y = 0; y < h; y += 5)
    tft.drawHLine(0, y, w);
  tft.setColor(color2);
  for (x = 0; x < w; x += 5)
    tft.drawVLine(x, 0, h);

  return micros() - start;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t i, i2;

  tft.clrScr();
  start = micros_start();
  tft.setColor(color);
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
  }

  return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t i, i2;

  tft.clrScr();

  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    tft.setColor(color1);
    tft.fillRect(cx1 - i2, cy1 - i2, cx1 + i2 - 1, cy1 + i2 - 1);

    t += micros() - start;

    // Outlines are not included in timing results
    tft.setColor(color2);
    tft.drawRect(cx - i2, cy - i2, cx + i2 - 1, cy + i2 - 1);
  }

  return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;

  tft.clrScr();
  start = micros_start();

  tft.setColor(color);
  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      tft.fillCircle(x, y, radius);
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

  tft.setColor(color);
  for (x = 0; x < w1; x += r2)
  {
    for (y = 0; y < h1; y += r2)
    {
      tft.drawCircle(x, y, radius);
    }
  }

  return micros() - start;
}

uint32_t testRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  tft.clrScr();

  start = micros_start();

  for (i = 0; i < n1; i += 6)
  {
    i2 = i / 2;
    tft.setColor(i, 0, 0);
    tft.drawRoundRect(cx1 - i2, cy1 - i2, cx1 + i2 - 1, cy1 + i2 - 1);
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;

  tft.clrScr();
  start = micros_start();

  for (i = n1; i > 20; i -= 6)
  {
    i2 = i / 2;
    tft.setColor(0, i, 0);
    tft.fillRoundRect(cx1 - i2, cy1 - i2, cx1 + i2 - 1, cy1 + i2 - 1);
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

