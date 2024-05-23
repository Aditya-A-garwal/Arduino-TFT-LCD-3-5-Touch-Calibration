#include "Arduino.h"

#include "Adafruit_GFX.h"
#include "MCUFRIEND_kbv.h"

#include "TouchScreen.h"

#include "constants.h"

const int CROSSHAIR_LEFT = CROSSHAIR_W / 2;
const int CROSSHAIR_RIGHT = DISPLAY_WIDTH - 1 - (CROSSHAIR_W / 2);
const int CROSSHAIR_TOP = CROSSHAIR_H / 2;
const int CROSSHAIR_BOTTOM = DISPLAY_HEIGHT - 1 - (CROSSHAIR_H / 2);

const int crosshair_coors[][2] = {
  {CROSSHAIR_LEFT, CROSSHAIR_TOP},
  {CROSSHAIR_RIGHT, CROSSHAIR_TOP},
  {CROSSHAIR_LEFT, CROSSHAIR_BOTTOM},
  {CROSSHAIR_RIGHT, CROSSHAIR_BOTTOM},
};

const int NUM_CROSSHAIRS = sizeof(crosshair_coors) / sizeof(crosshair_coors[0]);

MCUFRIEND_kbv tft;

TouchScreen ts(XP, YP, XM, YM, 300);

void draw_crosshair(uint16_t x, uint16_t y, uint16_t clr) {

  const int x1 = x - (CROSSHAIR_W >> 1);
  const int x2 = x + (CROSSHAIR_W >> 1);

  const int y1 = y - (CROSSHAIR_H >> 1);
  const int y2 = y + (CROSSHAIR_H >> 1);

  tft.drawRect(x1, y1, CROSSHAIR_W, CROSSHAIR_H, clr);

  tft.drawLine(x1, y1, x2, y2, clr);
  tft.drawLine(x1, y2, x2, y1, clr);
}

void to_display_mode() {

  pinMode(XM, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  pinMode(YP, OUTPUT);
}

bool valid_touch() {

  TSPoint p = ts.getPoint();
  to_display_mode();

  return (PRESSURE_LO <= p.z) && (p.z <= PRESSURE_HI);
}

void get_touch(uint16_t *xptr, uint16_t *yptr) {

  TSPoint p;

  while (1) {
    p = ts.getPoint();
    if ((PRESSURE_LO <= p.z) && (p.z <= PRESSURE_HI)) {
      break;
    }
  }

  *xptr = p.x;
  *yptr = p.y;

  to_display_mode();
}

void setup() {

  uint16_t touch_coors[NUM_CROSSHAIRS][2];

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

    uint16_t cx = crosshair_coors[i][0];
    uint16_t cy = crosshair_coors[i][1];

    uint32_t ax = 0;
    uint32_t ay = 0;

    uint16_t tx;
    uint16_t ty;

    for (uint16_t _ = 0; _ < CALIBRATION_TOUCH_CNT; ++_) {

      draw_crosshair(cx, cy, RED);

      get_touch(&tx, &ty);
      ax += tx;
      ay += ty;

      draw_crosshair(cx, cy, GREEN);

      delay(CALIBRATION_TOUCH_PERIOD_MS);
    }

    touch_coors[i][0] = ax / CALIBRATION_TOUCH_CNT;
    touch_coors[i][1] = ay / CALIBRATION_TOUCH_CNT;

    draw_crosshair(cx, cy, WHITE);

    while (valid_touch());
    delay(1000);
  }

  for (int i = 0; i < NUM_CROSSHAIRS; ++i) {

    uint16_t cx = crosshair_coors[i][0];
    uint16_t cy = crosshair_coors[i][1];

    uint16_t tx = touch_coors[i][0];
    uint16_t ty = touch_coors[i][1];

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

  xbeginf /= 2;
  xendf /= 2;
  ybeginf /= 2;
  yendf /= 2;

  xbegin = map(0, CROSSHAIR_LEFT, CROSSHAIR_RIGHT, xbeginf, xendf);
  xend = map(DISPLAY_WIDTH - 1, CROSSHAIR_LEFT, CROSSHAIR_RIGHT, xbeginf, xendf);

  ybegin = map(DISPLAY_HEIGHT - 1, CROSSHAIR_TOP, CROSSHAIR_BOTTOM, ybeginf, yendf);
  yend = map(0, CROSSHAIR_TOP, CROSSHAIR_BOTTOM, ybeginf, yendf);

  Serial.println("Paste this into constants.h");
  Serial.println();

  Serial.print("const int XBEGIN = "); Serial.println(xbegin);
  Serial.print("const int XEND = "); Serial.println(xend);
  Serial.print("const int YBEGIN = "); Serial.println(ybegin);
  Serial.print("const int YEND = "); Serial.println(yend);

  to_display_mode();

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

void loop() {
}