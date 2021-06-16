/*
  Adapted from the Adafruit and Xark's PDQ graphicstest sketch.

  See end of file for original header text and MIT license info.
*/

#define TFT_BL 22
#define TFT_CS 5
#define TFT_RST 33
#define TFT_DC 27
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_MISO 19
#define SPI_FREQUENCY 40000000

#include <LovyanGFX.hpp>

static LGFX lcd;
static lgfx::Panel_ILI9341 panel;

int32_t w, h, n, n1, cx, cy, cx1, cy1, cn, cn1;
uint8_t tsa, tsb, tsc, ds;

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Lovyan_GFX library Test!");

  panel.freq_write = SPI_FREQUENCY;
  panel.freq_fill = SPI_FREQUENCY;
  panel.spi_cs = TFT_CS;
  panel.spi_dc = TFT_DC;
  panel.gpio_rst = TFT_RST;
  panel.gpio_bl = TFT_BL;
  panel.panel_width = 240;
  panel.panel_height = 320;
  panel.rotation = 2;
  lcd.setPanel(&panel);
  lcd.init(TFT_SCLK, TFT_MISO, TFT_MOSI);

  w = lcd.width();
  h = lcd.height();
  n = min(w, h);
  n1 = n - 1;
  cx = w / 2;
  cy = h / 2;
  cx1 = cx - 1;
  cy1 = cy - 1;
  cn = min(cx1, cy1);
  cn1 = cn - 1;
  tsa = ((w <= 176) || (h <= 160)) ? 1 : (((w <= 240) || (h <= 240)) ? 2 : 3); // text size A
  tsb = ((w <= 240) || (h <= 220)) ? 1 : 2;                                    // text size B
  tsc = ((w <= 220) || (h <= 220)) ? 1 : 2;                                    // text size C
  ds = (w <= 160) ? 9 : 12;                                                    // digit size

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

  int32_t usecFilledTrangles = testFilledTriangles();
  serialOut(F("Triangles (filled)\t"), usecFilledTrangles, 100, false);

  int32_t usecTriangles = testTriangles();
  serialOut(F("Triangles (outline)\t"), usecTriangles, 100, true);

  int32_t usecFilledCircles = testFilledCircles(10);
  serialOut(F("Circles (filled)\t"), usecFilledCircles, 100, false);

  int32_t usecCircles = testCircles(10);
  serialOut(F("Circles (outline)\t"), usecCircles, 100, true);

  int32_t usecFilledArcs = testFillArcs();
  serialOut(F("Arcs (filled)\t"), usecFilledArcs, 100, false);

  int32_t usecArcs = testArcs();
  serialOut(F("Arcs (outline)\t"), usecArcs, 100, true);

  int32_t usecFilledRoundRects = testFilledRoundRects();
  serialOut(F("Rounded rects (filled)\t"), usecFilledRoundRects, 100, false);

  int32_t usecRoundRects = testRoundRects();
  serialOut(F("Rounded rects (outline)\t"), usecRoundRects, 100, true);

  Serial.println(F("Done!"));

  uint16_t c = 4;
  int8_t d = 1;
  for (int32_t i = 0; i < h; i++)
  {
    lcd.drawFastHLine(0, i, w, c);
    c += d;
    if (c <= 4 || c >= 11)
    {
      d = -d;
    }
  }

  lcd.setCursor(0, 0);

  lcd.setTextSize(tsa);
  lcd.setTextColor(TFT_MAGENTA);
  lcd.println(F("LovyanGFX PDQ"));

  if (h > w)
  {
    lcd.setTextSize(tsb);
    lcd.setTextColor(TFT_GREEN);
    lcd.print(F("\nBenchmark "));
    lcd.setTextSize(tsc);
    if (ds == 12)
    {
      lcd.print(F("   "));
    }
    lcd.println(F("micro-secs"));
  }

  lcd.setTextSize(1);
  printnice(F("Screen fill "), usecFillScreen);
  printnice(F("Text        "), usecText);
  printnice(F("Pixels      "), usecPixels);
  printnice(F("Lines       "), usecLines);
  printnice(F("H/V Lines   "), usecFastLines);
  printnice(F("Rectangles F"), usecFilledRects);
  printnice(F("Rectangles  "), usecRects);
  printnice(F("Triangles F "), usecFilledTrangles);
  printnice(F("Triangles   "), usecTriangles);
  printnice(F("Circles F   "), usecFilledCircles);
  printnice(F("Circles     "), usecCircles);
  printnice(F("Arcs F      "), usecFilledArcs);
  printnice(F("Arcs        "), usecArcs);
  printnice(F("RoundRects F"), usecFilledRoundRects);
  printnice(F("RoundRects  "), usecRoundRects);

  if ((h > w) || (h > 240))
  {
    lcd.setTextSize(tsc);
    lcd.setTextColor(TFT_GREEN);
    lcd.print(F("\nBenchmark Complete!"));
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
    lcd.fillScreen(TFT_BLACK);
  }
}

void printnice(const __FlashStringHelper *item, long int v)
{
  lcd.setTextSize(tsb);
  lcd.setTextColor(TFT_CYAN);
  lcd.print(item);

  lcd.setTextSize(tsc);
  lcd.setTextColor(TFT_YELLOW);
  if (v < 0)
  {
    lcd.println(F("      N / A"));
  }
  else
  {
    char str[32] = {0};
#ifdef RTL8722DM
    sprintf(str, "%d", (int)v);
#else
    sprintf(str, "%ld", v);
#endif
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
    lcd.println(str);
  }
}

int32_t testFillScreen()
{
  uint32_t start = micros_start();
  // Shortened this tedious test!
  lcd.fillScreen(TFT_WHITE);
  lcd.fillScreen(TFT_RED);
  lcd.fillScreen(TFT_GREEN);
  lcd.fillScreen(TFT_BLUE);
  lcd.fillScreen(TFT_BLACK);

  return micros() - start;
}

int32_t testText()
{
  uint32_t start = micros_start();
  lcd.setCursor(0, 0);

  lcd.setTextSize(1);
  lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  lcd.println(F("Hello World!"));

  lcd.setTextSize(2);
  lcd.setTextColor(lcd.color565(0xff, 0x00, 0x00));
  lcd.print(F("RED   "));
  lcd.setTextColor(lcd.color565(0x00, 0xff, 0x00));
  lcd.print(F("GREEN "));
  lcd.setTextColor(lcd.color565(0x00, 0x00, 0xff));
  lcd.println(F("BLUE"));

  lcd.setTextSize(tsa);
  lcd.setTextColor(TFT_YELLOW);
  lcd.println(1234.56);

  lcd.setTextColor(TFT_WHITE);
  lcd.println((w > 128) ? 0xDEADBEEF : 0xDEADBEE, HEX);

  lcd.setTextColor(TFT_CYAN, TFT_WHITE);
  lcd.println(F("Groop,"));

  lcd.setTextSize(tsc);
  lcd.setTextColor(TFT_MAGENTA, TFT_WHITE);
  lcd.println(F("I implore thee,"));

  lcd.setTextSize(1);
  lcd.setTextColor(TFT_NAVY, TFT_WHITE);
  lcd.println(F("my foonting turlingdromes."));

  lcd.setTextColor(TFT_DARKGREEN, TFT_WHITE);
  lcd.println(F("And hooptiously drangle me"));

  lcd.setTextColor(TFT_DARKCYAN, TFT_WHITE);
  lcd.println(F("with crinkly bindlewurdles,"));

  lcd.setTextColor(TFT_MAROON, TFT_WHITE);
  lcd.println(F("Or I will rend thee"));

  lcd.setTextColor(TFT_PURPLE, TFT_WHITE);
  lcd.println(F("in the gobberwartsb"));

  lcd.setTextColor(TFT_OLIVE, TFT_WHITE);
  lcd.println(F("with my blurglecruncheon,"));

  lcd.setTextColor(TFT_DARKGREY, TFT_WHITE);
  lcd.println(F("see if I don't!"));

  lcd.setTextSize(2);
  lcd.setTextColor(TFT_RED);
  lcd.println(F("Size 2"));

  lcd.setTextSize(3);
  lcd.setTextColor(TFT_ORANGE);
  lcd.println(F("Size 3"));

  lcd.setTextSize(4);
  lcd.setTextColor(TFT_YELLOW);
  lcd.println(F("Size 4"));

  lcd.setTextSize(5);
  lcd.setTextColor(TFT_GREENYELLOW);
  lcd.println(F("Size 5"));

  lcd.setTextSize(6);
  lcd.setTextColor(TFT_GREEN);
  lcd.println(F("Size 6"));

  lcd.setTextSize(7);
  lcd.setTextColor(TFT_BLUE);
  lcd.println(F("Size 7"));

  lcd.setTextSize(8);
  lcd.setTextColor(TFT_PURPLE);
  lcd.println(F("Size 8"));

  lcd.setTextSize(9);
  lcd.setTextColor(TFT_PINK);
  lcd.println(F("Size 9"));

  return micros() - start;
}

int32_t testPixels()
{
  uint32_t start = micros_start();

  for (int16_t y = 0; y < h; y++)
  {
    for (int16_t x = 0; x < w; x++)
    {
      lcd.drawPixel(x, y, lcd.color565(x << 3, y << 3, x * y));
    }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif
  }

  return micros() - start;
}

int32_t testLines()
{
  uint32_t start;
  int32_t x1, y1, x2, y2;

  start = micros_start();

  x1 = y1 = 0;
  y2 = h - 1;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  for (x2 = 0; x2 < w; x2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;
  for (x2 = 0; x2 < w; x2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    lcd.drawLine(x1, y1, x2, y2, TFT_BLUE);
  }
#ifdef ESP8266
    yield(); // avoid long run triggered ESP8266 WDT restart
#endif

  return micros() - start;
}

int32_t testFastLines()
{
  uint32_t start;
  int32_t x, y;

  start = micros_start();

  for (y = 0; y < h; y += 5)
  {
    lcd.drawFastHLine(0, y, w, TFT_RED);
  }
  for (x = 0; x < w; x += 5)
  {
    lcd.drawFastVLine(x, 0, h, TFT_BLUE);
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

    lcd.fillRect(cx - i2, cy - i2, i, i, lcd.color565(i, i, 0));
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
    lcd.drawRect(cx - i2, cy - i2, i, i, TFT_GREEN);
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
      lcd.fillCircle(x, y, radius, TFT_MAGENTA);
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
      lcd.drawCircle(x, y, radius, TFT_WHITE);
    }
  }

  return micros() - start;
}

int32_t testFillArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    lcd.fillArc(cx1, cy1, i, i - 3, 0, i * r, TFT_RED);
  }

  return micros() - start;
}

int32_t testArcs()
{
  int16_t i, r = 360 / cn;
  uint32_t start = micros_start();

  for (i = 6; i < cn; i += 6)
  {
    lcd.drawArc(cx1, cy1, i, i - 3, 0, i * r, TFT_WHITE);
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
    lcd.fillTriangle(cx1, cy1 - i, cx1 - i, cy1 + i, cx1 + i, cy1 + i,
                     lcd.color565(0, i, i));
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
    lcd.drawTriangle(
        cx1, cy1 - i,     // peak
        cx1 - i, cy1 + i, // bottom left
        cx1 + i, cy1 + i, // bottom right
        lcd.color565(0, 0, i));
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
    lcd.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, lcd.color565(0, i, 0));
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
    lcd.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, lcd.color565(i, 0, 0));
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
