# Calibrate 3.5" TFT LCD Touchscreen With Arduino UNO R3/R4

![GitHub License](https://img.shields.io/github/license/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration)
![GitHub forks](https://img.shields.io/github/forks/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square&color=blue)
![GitHub Repo stars](https://img.shields.io/github/stars/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square&color=blue)
![GitHub issues](https://img.shields.io/github/issues-raw/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square&color=indianred)
![GitHub closed issues](https://img.shields.io/github/issues-closed-raw/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square)
![GitHub pull requests](https://img.shields.io/github/issues-pr/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square&color=indianred)
![GitHub closed pull requests](https://img.shields.io/github/issues-pr-closed/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration?style=flat-square)
![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration/build.yml?style=flat-square)

## Overview

This repository contains a program to calibrate a 3.5" touch screen display shield, as shown below -

|![Image of LCD Touch Shield from Top](images/LCD_top.png)|![Image of LCD Touch Shield from Bottom](images/LCD_bottom.png)|
|-|-|

Most 3.5" LCD Touch displays use the ILI9486 Display Driver and include a resistive touchscreen. **The PCB Layout & silkscreen text may vary slightly between displays. This does not change their behaviour and functionality.** This repository depends on the following libraries -

- [Adafruit Touch Screen Library](https://github.com/adafruit/Adafruit_TouchScreen) to manage touch input
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library/tree/master) for graphics primitives
- [This fork of the MCUFriend KBV library](https://github.com/slviajero/MCUFRIEND_kbv) to drive the display (this makes it compatible with the UNO R4)

The program has been written using PlatformIO, and has been tested on the [Arduino UNO R3](https://docs.arduino.cc/hardware/uno-rev3/), [Arduino UNO R4 Minima](https://docs.arduino.cc/hardware/uno-r4-minima/) & [Arduino UNO R4 WiFi](https://docs.arduino.cc/hardware/uno-r4-wifi/).

> [!TIP]
> A detailed tutorial on how to use this repository **with video demonstration** is available at https://dumblebots.com/2024/05/17/using-3-5-tft-lcd-display-with-ili9486-arduino-part-2-touch-calibration/. The tutorial includes a comprehensive explanation of the code as well as the calibration process itself.

## Building/Uploading With PlatformIO

Since this project has been written using PlatformIO by default, simply run the following commands to fetch the libraries, build the project and upload the program -

```shell
pio pkg install
pio run
pio run --target upload
```

## Building/Uploading With Arduino IDE

Create a new sketch and copy the contents of [```src/main.cpp```](/src/main.cpp) from this repository into the default ```.ino``` file. Create a new tab/file in the IDE named ```constants.h``` and copy the contents of [```src/constants.h```](/src/constants.h) from this repository into this file.

Install the Adafruit Touch Screen Library and Adafruit GFX Library from the Library Manager (under *Sketch>Include Library>Manage Libraries...*)

Download [this](https://github.com/slviajero/MCUFRIEND_kbv) repository as a ZIP file and install it by navigating to *Sketch>Include Library>Add .ZIP Library*, and selecting the downloaded file from the file explorer.

After this, you can Build and Upload the program as usual.

## Calibrating the Touchscreen

Run the program given in [this](https://github.com/Aditya-A-garwal/Arduino-TFT-LCD-3-5-Touch-Calibration) repository to calibrate the display. The display should print the values of 4 constants to the Serial Monitor. Copy and paste these values in [```src/constants.h```](/src/constants.h) to calibrate the touchscreen.

## Using the Example

As soon as the program is uploaded, the display should turn black and a red crosshair should appear at the top-left corner of the screen.

Using the included plastic stylus, gently tap as close to the centre of the crosshair as possible. Life the stylus once the crosshair turns green. After this process is repeated 5 times, the crosshair should turn white.

The program repeats this process for all 4 corners, and reports 4 values on the Serial Monitor as well as the display upon completion, namely ```XBEGIN```, ```XEND```, ```YBEGIN``` and ```YEND```. These values can be used along with the Adafruit Touch Screen Library to convert the coordinates from the touch screen library as follows -

```cpp

// this snippet uses the Adafruit Touch Screen Library
// XP, YP, XM and YM are the pins used by the touchscreen
// 300 is the resistance across the screen in ohms

// create constants XBEGIN, XEND, YBEGIN and YEND according to the output from the calibration example

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint p = ts.getPoint();

touchX = p.x; // Raw Horizontal Coordinate
touchY = p.y; // Raw Veritcal Coordinate

touchX = constrain(p.x, XBEGIN, XEND);
touchY = constrain(p.y, YBEGIN, YEND);

touchX = map(touchX, XBEGIN, XEND, 0, 319); // Horizontal Coordinate of Touch
touchX = map(touchX, XBEGIN, XEND, 479, 0); // Vertical Coordinate of Touch
```

## Troubleshooting

Some common problems and their solutions -

|Problem|Solution|
|-|-|
|Display stays white after uploading program|Non-Standard Driver (not ILI9486)|
|Display not responding after touch|Try changing the order of the touch pins in [```src/constants.h```](/src/constants.h) file, i.e. swap the values of ```XP```, ```YP```, ```XM``` and ```YM```|
|Compilation issues related to SPI|Update the Arduino IDE version and/or install the SPI library|
|Display Flickering/Arduino is reset automatically|Faulty Power Supply/Cable|
