# AT7456E OSD Display Driver for STM32

This project is a minimal driver and demonstration of the AT7456E On-Screen Display (OSD) video overlay chip, controlled via SPI from an STM32 microcontroller (using HAL drivers).

It allows you to:
- Initialize the OSD controller
- Upload custom fonts
- Display text and custom characters on screen
- Enable/disable OSD overlay
- Directly manage video RAM and character memory

## 📁 File Overview

- `main.c` — Main application file. Initializes STM32, SPI3, and the OSD module. Demonstrates writing "TEST OSD" on screen using custom characters.
- `osd.c` — Core logic for interfacing with the AT7456E: character upload, text print, font writing, screen clearing, etc.
- `osd.h` — Header file for `osd.c` with public API definitions.
- `AT7456E_registers.h` — Register definitions for AT7456E (not included, assumed to be present).
- `max7456font.h` — Font data array used for uploading fonts (not included, assumed to be present).

## ✅ Features

- SPI-based AT7456E control
- PAL/NTSC video mode support
- Custom character upload (54-byte definition)
- Full font flash (NVM) upload
- Text and character rendering by coordinates
- Full screen clear and rendering control

## 🚀 How to Use

1. Ensure SPI3 is correctly initialized on your STM32 (as in `MX_SPI3_Init()`).
2. Connect AT7456E's CS pin to `GPIOD, PIN_2` (or modify `OSD_CS_PORT/PIN` macros).
3. In `main.c`, call:

