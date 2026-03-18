# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

P2 (Parallax Propeller 2) driver for Mikroe eINK adapter Click board, written in Spin2. Drives multiple eINK display sizes (1.54", 2.00", 2.13", 2.90", WaveShare 2.13" R/B/W, WaveShare 5.65" 7-color, Good Display 3.70" w/Touch) via SPI through the P2 Eval Click Adapter.

## Language and Toolchain

- **Primary language**: Spin2 (.spin2) — the native language for Parallax Propeller 2 microcontrollers
- **Build tool**: FlexProp or PNut (Spin2 compilers for P2). There is no Makefile for the Spin2 code; compilation is done through the IDE or command-line compiler
- **Bitmap converter**: C utility in `C-src/` — build with `make clean all` in that directory

## Architecture

### Driver Layer (`src/`)

- **`isp_eInk_click.spin2`** — Main driver object. Provides the full public API: display init (`start`/`stop`), drawing primitives (lines, circles, boxes, text, images, fill), orientation control, and frame buffer management. Contains all display-specific command sequences, resolution constants, and SPI communication for each supported display type. Display type is selected via enum at `start()` time (e.g., `DS_eink213`, `DS_eink290`, `DSGD_370Tch`).
- **`isp_eInk_fonts.spin2`** — Font data and metrics (Tahoma 6-18pt, Exo2 Condensed, Roboto Mono). Referenced by the main driver via `eInkFonts` OBJ.
- **`jm_ez_spi.spin2`** — SPI smart-pin communication (by Jon McPhalen). Used by the driver for all SPI bus operations.
- **`isp_eink_touch.spin2`** / **`isp_i2c_singleton.spin2`** — Touch overlay support for Good Display 3.70" (I2C-based touch controller).
- **`isp_eink_gd.spin2`** — Good Display-specific driver extensions.

### Demo Layer (`src/`)

- **`demo_eInk_click.spin2`** — Main demo top-level file. Configures display type and pin group in `CON` block, then runs drawing demos.
- **`demo_eInk_gd.spin2`** / **`demo_eink_touch.spin2`** — Demos for Good Display and touch variants.
- **`isp_eInk_demo_images.spin2`** / **`isp_image_coffee.spin2`** / **`isp_demo_P2logo_image.spin2`** — Bitmap image data used by demos.

### Bitmap Converter (`C-src/`)

C utility that converts Piskel-exported .c bitmap files into .spin2 DAT sections for inclusion in P2 projects. Build and run:

```bash
cd C-src
make clean all
./convert -v -o myImage.spin2 -i myImageLabel
```

## Key Conventions

- **Pin groups**: Hardware connection location is specified as `PINS_P0_P15`, `PINS_P16_P31`, or `PINS_P32_P47` — these correspond to which P2 Eval Click Adapter slot the module is plugged into.
- **Two drawing modes**: (1) Immediate — methods like `clearScreen()`, `fillScreen()`, `displayImage()`, `displayTextAtXY()` write to buffer and flush to display immediately. (2) Buffered — methods like `setBackground()`, `setTextAtXY()`, `fillRegionAtXY()`, `lineAtXY()`, etc. write to the frame buffer only; call `flushToDisplay()` (or `updateDisplay()`) when done.
- **Orientation**: `setOrientation(ROTATE_0/90/180/270)` adjusts coordinate mapping so (0,0) is always top-left regardless of physical connector position. Font orientation (`FO_HORIZONTAL`, `FO_VERTICAL`, `FO_VERTICAL_COLUMN`) is additive on top of device orientation.
- **Color enums**: Use `EINK_COLOR_BLACK`, `EINK_COLOR_WHITE`, etc. — internal color values differ per display type and are mapped by the driver.
- **Display resolution constant `LARGEST_FRAME_SIZE_IN_BYTES`**: Controls frame buffer allocation. Can be reduced if not using larger displays, to save P2 memory.
- **Spin2 comment style**: `''` for doc comments, `'` for inline comments. File headers use a standard block format with File, Purpose, Authors, E-mail, Started, Updated fields.
