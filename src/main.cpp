#include "Arduino.h"

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"
#include "TouchScreen.h"

#include "constants.h"

const int crosshairCoors[][2] = {
  {0 + (CROSSHAIR_W >> 1),                  0 + (CROSSHAIR_H >> 1)},
  {DISPLAY_WIDTH - 1 - (CROSSHAIR_W >> 1),  0 + (CROSSHAIR_W >> 1)},
  {0 + (CROSSHAIR_W >> 1),                  DISPLAY_HEIGHT - 1 - (CROSSHAIR_H >> 1)},
  {DISPLAY_WIDTH - 1 - (CROSSHAIR_W >> 1),  DISPLAY_HEIGHT - 1 - (CROSSHAIR_H >> 1)}
};

const int NUM_CROSSHAIRS = sizeof(crosshairCoors) / sizeof(crosshairCoors[0]);

MCUFRIEND_kbv tft;

TouchScreen ts(XP, YP, XM, YM, 300);
TSPoint p;

void
draw_crosshair(uint16_t x, uint16_t y, uint16_t clr) {

  const int x1 = x - (CROSSHAIR_W >> 1);
  const int x2 = x + (CROSSHAIR_W >> 1);

  const int y1 = y - (CROSSHAIR_H >> 1);
  const int y2 = y + (CROSSHAIR_H >> 1);

  tft.drawRect(x1, y1, CROSSHAIR_W, CROSSHAIR_H, clr);

  tft.drawLine(x1, y1, x2, y2, clr);
  tft.drawLine(x1, y2, x2, y1, clr);
}

void
toDisplayMode() {

  pinMode(XM, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  pinMode(YP, OUTPUT);
}

bool
valid_touch() {

  p = ts.getPoint();
  if (p.z > PRESSURE_RIGHT || p.z < PRESSURE_LEFT) {
    return false;
  }

  return true;
}

void
get_touch(uint16_t *xptr, uint16_t *yptr) {

  while (!valid_touch());

  *xptr = p.x;
  *yptr = p.y;
}

void
setup() {

  uint16_t touchCoors[NUM_CROSSHAIRS][2];

  uint16_t xbeginf = 0;
  uint16_t xendf   = 0;
  uint16_t ybeginf = 0;
  uint16_t yendf   = 0;

  uint16_t xbegin = 0;
  uint16_t xend   = 0;
  uint16_t ybegin = 0;
  uint16_t yend   = 0;

  Serial.begin(115200);

  tft.begin(0x9486);
  tft.fillScreen(BLACK);

  for (int i = 0; i < NUM_CROSSHAIRS; ++i) {

    uint16_t cx = crosshairCoors[i][0];
    uint16_t cy = crosshairCoors[i][1];

    uint32_t ax = 0;
    uint32_t ay = 0;

    uint16_t tx;
    uint16_t ty;

    for (int _ = 0; _ < CALIBRATION_TOUCH_CNT; ++_) {

      toDisplayMode();
      draw_crosshair(cx, cy, RED);

      get_touch(&tx, &ty);
      ax += tx;
      ay += ty;

      toDisplayMode();
      draw_crosshair(cx, cy, GREEN);

      delay(CALIBRATION_TOUCH_PERIOD_MS);
    }

    touchCoors[i][0] = ax / CALIBRATION_TOUCH_CNT;
    touchCoors[i][1] = ay / CALIBRATION_TOUCH_CNT;

    toDisplayMode();
    draw_crosshair(cx, cy, WHITE);

    while (valid_touch());
    delay(1000);
  }

  for (int i = 0; i < NUM_CROSSHAIRS; ++i) {

    uint16_t cx = crosshairCoors[i][0];
    uint16_t cy = crosshairCoors[i][1];

    uint16_t tx = touchCoors[i][0];
    uint16_t ty = touchCoors[i][1];

    if (cx == (CROSSHAIR_W >> 1)) {
      xbeginf += tx;
    }
    else if (cx == (DISPLAY_WIDTH - 1 - (CROSSHAIR_W >> 1))) {
      xendf += tx;
    }

    if (cy == (CROSSHAIR_H >> 1)) {
      ybeginf += ty;
    }
    else if (cy == (DISPLAY_HEIGHT - 1 - (CROSSHAIR_H >> 1))) {
      yendf += ty;
    }
  }

  xbeginf >>= 1;
  xendf >>= 1;
  ybeginf >>= 1;
  yendf >>= 1;

  xbegin = map(0, 0 + (CROSSHAIR_W >> 1), DISPLAY_WIDTH - 1 - (CROSSHAIR_W >> 1), xbeginf, xendf);
  xend = map(DISPLAY_WIDTH - 1, 0 + (CROSSHAIR_W >> 1), DISPLAY_WIDTH - 1 - (CROSSHAIR_W >> 1), xbeginf, xendf);

  ybegin = map(DISPLAY_HEIGHT- 1, 0 + (CROSSHAIR_H >> 1), DISPLAY_HEIGHT- 1 - (CROSSHAIR_H >> 1), ybeginf, yendf);
  yend = map(0, 0 + (CROSSHAIR_H >> 1), DISPLAY_HEIGHT - 1 - (CROSSHAIR_H >> 1), ybeginf, yendf);

  Serial.println("Paste this into constants.h");
  Serial.println();

  Serial.print("const int XBEGIN = "); Serial.println(xbegin);
  Serial.print("const int XEND = "); Serial.println(xend);
  Serial.print("const int YBEGIN = "); Serial.println(ybegin);
  Serial.print("const int YEND = "); Serial.println(yend);

  toDisplayMode();

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(0, 100);

  tft.print("  const int XBEGIN = ");
  tft.println(xbegin);

  tft.print("  const int XEND = ");
  tft.println(xend);

  tft.print("  const int YBEGIN = ");
  tft.println(ybegin);

  tft.print("  const int YEND = ");
  tft.println(yend);
}

void
loop() {
}