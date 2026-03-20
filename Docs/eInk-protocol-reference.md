# P2 eINK Driver — Technical Protocol Reference

Detailed protocol-level documentation for each eINK display supported by the P2 eINK Click driver. This covers SPI communication, command registers, initialization sequences, frame buffer memory organization, LUT waveform tables, busy/idle signaling, and power management for all nine supported display types.

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)

---

## Table of Contents

- [SPI Bus Protocol](#spi-bus-protocol)
- [Pin Assignments](#pin-assignments)
- [Display Protocol Sections](#display-protocol-sections)
  1. [DS_eink154 — 1.54" B/W (SSD1608)](#1-ds_eink154--154-bw-ssd1608)
  2. [DS_eink200gs — 2.00" Grayscale (SSD1606)](#2-ds_eink200gs--200-4-color-grayscale-ssd1606)
  3. [DS_eink213 — 2.13" B/W (SSD1675)](#3-ds_eink213--213-bw-ssd1675)
  4. [DS_eink290 — 2.90" B/W (SSD1675)](#4-ds_eink290--290-bw-ssd1675)
  5. [DSWS_2in13BV3 — WaveShare 2.13" B/W/R](#5-dsws_2in13bv3--waveshare-213-3-color-bwr)
  6. [DSAF_213BRYW — Adafruit 2.13" Quad-Color (JD79661AA)](#6-dsaf_213bryw--adafruit-213-quad-color-jd79661aa)
  7. [DSGD_370Tch — Good Display 3.70" B/W with Touch (UC8253)](#7-dsgd_370tch--good-display-370-bw-with-touch-uc8253)
  8. [DSWS_5in65F — WaveShare 5.65" 7-Color ACeP (UC8159)](#8-dsws_5in65f--waveshare-565-7-color-acep-uc8159)
  9. [DSWS_370BW — WaveShare 3.70" B/W (SSD1677)](#9-dsws_370bw--waveshare-370-bw-ssd1677)
- [Cross-Display Summary Tables](#cross-display-summary-tables)

---

## SPI Bus Protocol

All displays communicate via SPI Mode 0 (CPOL=0, CPHA=0) using the P2 smart-pin SPI engine.

| Parameter | Value |
| --- | --- |
| **SPI Mode** | Mode 0 (clock idle low, data sampled on rising edge) |
| **Clock Speed** | 20 MHz (`EINK_SPI_CLK_MHZ = 20_000`) |
| **Bit Order** | MSB first |
| **Word Size** | 8 bits per transfer |

### Command vs. Data Distinction

The D/C (Data/Command) pin distinguishes between register commands and data bytes:

| D/C Pin | Meaning |
| --- | --- |
| **LOW** | SPI byte is a **command** register address |
| **HIGH** | SPI byte is **data** (parameter or pixel data) |

### SPI Transaction Sequence

Each byte transfer follows this pattern:

1. Assert CS (drive LOW)
2. Set D/C pin (LOW for command, HIGH for data)
3. Clock out 8 bits MSB-first on MOSI
4. Deassert CS (drive HIGH)

The driver's `writeCmd(cmd)` method sets D/C LOW and sends one byte. The `writeData(val)` method sets D/C HIGH and sends one byte. Bulk data writes (`writeDataBlock(pData, nCount)`) hold D/C HIGH and send multiple bytes with CS toggled per byte.

---

## Pin Assignments

### Mikroe eINK Click Module (Standard Displays)

Used by: DS_eink154, DS_eink200gs, DS_eink213, DS_eink290, DSWS_2in13BV3, DSWS_5in65F, DSAF_213BRYW, DSWS_370BW

```
             ┌─────────────────────────────┐
  Base + 06  │() AN                 D/C(*) │  Base + 05
             │                             │
  Base + 07  │(*) RST               BSY(*) │  Base + 04
             │                             │
  Base + 08  │(*) CS              <-- RX() │  Base + 03
             │                             │
  Base + 09  │(*) SCK  <--        --> TX() │  Base + 02
             │                             │
  Base + 10  │(*) MISO -->       --> SCL() │  Base + 01
             │                             │
  Base + 11  │(*) MOSI <--       <-> SDA() │  Base + 00
             │                             │
             │(*) 3v3                5v0() │
             │                             │
             │(*) GND               GND(*) │
             └─────────────────────────────/
```

| Signal | Pin Offset | Direction | Function |
| --- | --- | --- | --- |
| MOSI | +11 | P2→Display | SPI data out |
| MISO | +10 | Display→P2 | SPI data in (unused by most displays) |
| SCK | +9 | P2→Display | SPI clock |
| CS | +8 | P2→Display | Chip select (active LOW) |
| RST | +7 | P2→Display | Hardware reset (active LOW) |
| D/C | +5 | P2→Display | Data/Command select |
| BUSY | +4 | Display→P2 | Busy status output |

### Good Display 3.70" Direct Wiring (DSGD_370Tch)

This display does NOT use the Mikroe Click module. It connects directly to the P2 Eval Click Adapter via wire harness, with a different pin mapping:

| Signal | Pin Offset | Direction | Function |
| --- | --- | --- | --- |
| MOSI/SDO | +11 | P2→Display | SPI data out |
| D/C | +10 | P2→Display | Data/Command select |
| SCK | +9 | P2→Display | SPI clock |
| CS | +8 | P2→Display | Chip select (active LOW) |
| RST | +7 | P2→Display | Hardware reset (active LOW) |
| BUSY | +6 | Display→P2 | Busy status (on AN pin) |

The touch controller (GT911) uses the I2C side of the same adapter:

| Signal | Pin Offset | Direction | Function |
| --- | --- | --- | --- |
| SDA | +0 | Bidirectional | I2C data |
| SCL | +1 | P2→Touch | I2C clock (100 kHz) |
| INT | +2 | Touch→P2 | Touch interrupt |
| RST | +3 | P2→Touch | Touch controller reset |

Touch controller I2C address: `$38` (GT911)

---

## Display Protocol Sections

---

### 1. DS_eink154 — 1.54" B/W (SSD1608)

**Controller:** SSD1608 (Solomon Systech) — 200-source × 200-gate

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 200 × 200 pixels |
| Frame buffer size | 5,000 bytes |
| Pixel format | 1 bpp — 8 pixels per byte, MSB = leftmost pixel |
| Byte ordering | Row-major, left to right, top to bottom |
| RAM X range | 0–24 (25 bytes × 8 = 200 pixels) |
| RAM Y range | 0–199 |

**Pixel-to-byte mapping:** Pixel at (x, y) is bit `(7 - (x & 7))` of byte at offset `(y × 25) + (x >> 3)`. Bit value 1 = white, 0 = black.

#### Color Values

| User Enum | Device Value | Display |
| --- | --- | --- |
| `EINK_COLOR_BLACK` | `$00` | Black (all bits 0) |
| `EINK_COLOR_WHITE` | `$FF` | White (all bits 1) |

#### Reset Sequence

```
RST pin HIGH  →  wait 1 ms
RST pin LOW   →  wait 2 ms
RST pin HIGH  →  wait 3 ms
```

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | DRIVER_OUTPUT_CTRL | `$01` | `$C7, $00, $00` | Gate count = 199 (200-1), scan direction |
| 2 | BOOSTER_SOFT_START_CTRL | `$0C` | `$D7, $D6, $9D` | Booster phase timing |
| 3 | WRITE_VCOM_REGISTER | `$2C` | `$A8` | VCOM voltage = −2.1V |
| 4 | SET_DUMMY_LINE_PERIOD | `$3A` | `$1A` | Dummy line period = 26 TGate |
| 5 | SET_GATE_TIME | `$3B` | `$08` | Gate line width = 8µs |
| 6 | DATA_ENTRY_MODE_SETTING | `$11` | `$03` | X increment, Y increment |
| 7 | *(wait 2 ms)* | — | — | Settle time |
| 8 | WRITE_LUT_REGISTER | `$32` | *(30 bytes — see below)* | Load waveform LUT |
| 9 | *(wait 1000 ms)* | — | — | LUT settle time |

#### Waveform LUT (30 bytes)

```
Offset  00: $02 $02 $01 $11 $12 $12 $22 $22
Offset  08: $66 $69 $69 $59 $58 $99 $99 $88
Offset  16: $00 $00 $00 $00 $F8 $B4 $13 $51
Offset  24: $35 $51 $51 $19 $01 $00
```

The LUT defines the voltage waveform sequence applied to each pixel during display update. It controls the electrophoretic particle movement that creates the visible image. The 30-byte table is organized as phase timing groups that control the number of voltage pulses applied during each refresh phase.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for idle)* | — | Poll BUSY pin until idle |
| 2 | SET_RAM_X_ADDR (`$44`) | `$00, $0F` | X window: 0 to 15 (0–127 in 8-pixel units) |
| 3 | SET_RAM_Y_ADDR (`$45`) | `$00, $C7, $00` | Y window: 0 to 199 |
| 4 | SET_RAM_X_CTR (`$4E`) | `$00` | X pointer = 0 |
| 5 | SET_RAM_Y_CTR (`$4F`) | `$00` | Y pointer = 0 |
| 6 | WRITE_RAM (`$24`) | *(5,000 bytes)* | Frame buffer data, row by row |
| 7 | MASTER_ACTIVATION (`$20`) | — | Trigger display update |
| 8 | *(wait 100 ms)* | — | — |
| 9 | DISPLAY_UPDATE_CTRL_2 (`$22`) | `$C4` | Clock on, analog on, display mode 1 |
| 10 | MASTER_ACTIVATION (`$20`) | — | Execute display update |
| 11 | TERMINATE_FRAME (`$FF`) | — | End frame write |

#### Busy Pin Protocol

- **Polarity:** BUSY LOW = device idle, BUSY HIGH = device busy
- **Wait method:** Poll until BUSY reads LOW

#### Sleep Sequence

```
Command $10 (DEEP_SLEEP_MODE) — no data
Wait for device idle
```

---

### 2. DS_eink200gs — 2.00" 4-Color Grayscale (SSD1606)

**Controller:** SSD1606 (Solomon Systech) — 72-source × 172-gate, 2-bit grayscale

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 72 × 172 pixels |
| Frame buffer size | 3,096 bytes |
| Pixel format | 2 bpp — 4 pixels per byte |
| Byte ordering | Row-major; **Y-axis inverted** (hardware Y=0 is at bottom) |
| RAM X range | 0–17 ($00–$11) — 18 bytes × 4 pixels = 72 pixels |
| RAM Y range | 0–171 ($00–$AB) |

**Pixel-to-byte mapping (2bpp):** Each byte holds 4 pixels. Pixel at x-position within a byte occupies bits `[7:6]` for pixel 0, `[5:4]` for pixel 1, `[3:2]` for pixel 2, `[1:0]` for pixel 3. The 2-bit value selects the grayscale level.

**Y-axis inversion:** The driver internally compensates — the memory pointer starts at Y=$AB (bottom) and counts down. User coordinates (0,0) map to the visual top-left.

#### Color Values

| User Enum | 2-bit Value | Byte Fill Value | Display |
| --- | --- | --- | --- |
| `EINK_COLOR_BLACK` | `%00` | `$00` | Black |
| `EINK_COLOR_DARK_GREY` | `%01` | `$55` | Dark grey |
| `EINK_COLOR_LIGHT_GREY` | `%10` | `$AA` | Light grey |
| `EINK_COLOR_WHITE` | `%11` | `$FF` | White |

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | DEEP_SLEEP_MODE | `$10` | `$00` | Exit deep sleep |
| 2 | DATA_ENTRY_MODE_SETTING | `$11` | `$01` | X increment, Y decrement |
| 3 | SET_RAM_X_ADDR | `$44` | `$00, $11` | X window: 0–17 (72/4 = 18 bytes) |
| 4 | SET_RAM_Y_ADDR | `$45` | `$00, $AB` | Y window: 0–171 |
| 5 | SET_RAM_X_CTR | `$4E` | `$00` | X pointer = 0 |
| 6 | SET_RAM_Y_CTR | `$4F` | `$00` | Y pointer = 0 |
| 7 | BOOSTER_FEEDBACK_SEL | `$F0` | `$1F` | Booster feedback select |
| 8 | DISPLAY_UPDATE_CTRL_1 | `$21` | `$03` | Bypass RAM, normal update |
| 9 | WRITE_VCOM_REGISTER | `$2C` | `$A0` | VCOM voltage |
| 10 | BORDER_WAVEFORM_CTRL | `$3C` | `$63` | Border waveform |
| 11 | DISPLAY_UPDATE_CTRL_2 | `$22` | `$C4` | Clock on, analog on |
| 12 | *(wait 2 ms)* | — | — | Settle time |
| 13 | WRITE_LUT_REGISTER | `$32` | *(90 bytes — see below)* | Load waveform LUT |
| 14 | *(wait 1000 ms)* | — | — | LUT settle time |

#### Waveform LUT (90 bytes)

```
Offset  00: $82 $00 $00 $00 $AA $00 $00 $00
Offset  08: $AA $AA $00 $00 $AA $AA $AA $00
Offset  16: $55 $AA $AA $00 $55 $55 $55 $55
Offset  24: $AA $AA $AA $AA $55 $55 $55 $55
Offset  32: $AA $AA $AA $AA $15 $15 $15 $15
Offset  40: $05 $05 $05 $05 $01 $01 $01 $01
Offset  48: $00 $00 $00 $00 $00 $00 $00 $00
Offset  56: $00 $00 $00 $00 $00 $00 $00 $00
Offset  64: $00 $00 $00 $00 $00 $00 $00 $00
Offset  72: $00 $00 $00 $00 $00 $00 $00 $00
Offset  80: $41 $45 $F1 $FF $5F $55 $01 $00
Offset  88: $00 $00
```

The 90-byte LUT for this 4-level grayscale display encodes the waveform phases needed to drive pixels between all four grayscale states. The larger LUT size (vs. 30 bytes for B/W) is required because more transition combinations exist between four gray levels.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | SET_RAM_Y_CTR (`$4F`) | `$AB` | Y pointer = 171 (bottom, Y-inverted) |
| 2 | SET_RAM_X_CTR (`$4E`) | `$00` | X pointer = 0 |
| 3 | WRITE_RAM (`$24`) | *(3,096 bytes)* | Frame buffer data |
| 4 | MASTER_ACTIVATION (`$20`) | — | Trigger update |
| 5 | *(wait 100 ms)* | — | — |
| 6 | DISPLAY_UPDATE_CTRL_2 (`$22`) | `$C4` | Execute update sequence |
| 7 | MASTER_ACTIVATION (`$20`) | — | Activate |

#### Busy Pin Protocol

- **Polarity:** Same as SSD1608 — BUSY LOW = idle
- **Wait method:** Poll until BUSY reads LOW

---

### 3. DS_eink213 — 2.13" B/W (SSD1675)

**Controller:** SSD1675 (Solomon Systech) — 128-source × 250-gate

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Visible resolution | 122 × 250 pixels |
| Physical SRAM width | 128 pixels (16 bytes/row) |
| Frame buffer size | 4,000 bytes (128 × 250 / 8) |
| Pixel format | 1 bpp — 8 pixels per byte, MSB leftmost |
| Padding | 6 unused bits per row (pixels 122–127) |
| RAM X range | 0–15 (16 bytes × 8 = 128 pixels) |
| RAM Y range | 0–249 |

**Row padding:** Each row uses 16 bytes (128 bits), but only 122 pixels are visible. The last 6 bits of each row are padding that the display ignores. The driver writes all 16 bytes per row.

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | DRIVER_OUTPUT_CTRL | `$01` | `$F9, $00` | Gate count = 249 (250-1) |
| 2 | SET_DUMMY_LINE_PERIOD | `$3A` | `$06` | Dummy line period |
| 3 | SET_GATE_TIME | `$3B` | `$0B` | Gate line width |
| 4 | DATA_ENTRY_MODE_SETTING | `$11` | `$01` | X increment, Y decrement |
| 5 | SET_RAM_X_ADDR | `$44` | `$00, $0F` | X window: 0–15 |
| 6 | SET_RAM_Y_ADDR | `$45` | `$F9, $00` | Y window: 249 (start, reversed) to 0 |
| 7 | WRITE_VCOM_REGISTER | `$2C` | `$4B` | VCOM = −1.9V |
| 8 | BORDER_WAVEFORM_CTRL | `$3C` | `$33` | Border control |
| 9 | *(wait 2 ms)* | — | — | Settle time |
| 10 | WRITE_LUT_REGISTER | `$32` | *(70 bytes — see below)* | Load waveform LUT |
| 11 | *(wait 1000 ms)* | — | — | LUT settle time |

#### Waveform LUT (70 bytes)

```
Offset  00: $A0 $90 $50 $00 $00 $00 $00
Offset  07: $50 $90 $A0 $00 $00 $00 $00
Offset  14: $A0 $90 $50 $00 $00 $00 $00
Offset  21: $50 $90 $A0 $00 $00 $00 $00
Offset  28: $00 $00 $00 $00 $00 $00 $00
Offset  35: $0F $0F $0F $0F $02 $10 $10
Offset  42: $0A $0A $03 $08 $08 $09 $43
Offset  49: $07 $00 $00 $00 $00 $00 $00
Offset  56: $00 $00 $00 $00 $00 $00 $00
Offset  63: $00 $00 $00 $00 $00 $00 $00
```

This LUT is shared between the DS_eink213 and DS_eink290 displays (both use SSD1675). The 70-byte table contains 5 groups of 7-byte waveform phase definitions (bytes 0–34) followed by 5 groups of 7-byte timing repeat counts (bytes 35–69).

#### Frame Buffer Write Sequence

This display uses a **line-by-line** write approach (not bulk):

| Step | Description |
| --- | --- |
| 1 | Wait for device idle |
| 2 | Set RAM window: X[0,15], Y[0,249] via commands `$44`, `$45` |
| 3 | For each row Y = 0 to 249: |
| 3a | — Set RAM pointer to (0, Y) via commands `$4E`, `$4F` |
| 3b | — Send WRITE_RAM (`$24`) |
| 3c | — Write 16 bytes for this row |
| 4 | TERMINATE_FRAME (`$FF`) |
| 5 | Wait 100 ms |
| 6 | DISPLAY_UPDATE_CTRL_2 (`$22`) with data `$C4` |
| 7 | MASTER_ACTIVATION (`$20`) |

**Note:** The line-by-line approach differs from the DS_eink290's bulk write. This is due to how the SSD1675 handles the row padding for 122-pixel-wide displays.

---

### 4. DS_eink290 — 2.90" B/W (SSD1675)

**Controller:** SSD1675 (Solomon Systech) — 128-source × 296-gate

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 128 × 296 pixels |
| Frame buffer size | 4,736 bytes (128 × 296 / 8) |
| Pixel format | 1 bpp — 8 pixels per byte, MSB leftmost |
| RAM X range | 0–15 (16 bytes × 8 = 128 pixels) |
| RAM Y range | 0–295 |

No padding — the 128-pixel width aligns exactly to 16 bytes.

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | DRIVER_OUTPUT_CTRL | `$01` | `$27, $01, $00` | Gate count = 295 (296-1), two bytes for >255 |
| 2 | SET_DUMMY_LINE_PERIOD | `$3A` | `$1A` | Dummy line period = 26 |
| 3 | SET_GATE_TIME | `$3B` | `$08` | Gate line width = 8µs |
| 4 | DATA_ENTRY_MODE_SETTING | `$11` | `$01` | X increment, Y decrement |
| 5 | SET_RAM_X_ADDR | `$44` | `$00, $0F` | X window: 0–15 |
| 6 | SET_RAM_Y_ADDR | `$45` | `$27, $01, $00, $00` | Y window: 295 to 0 (4 bytes for extended range) |
| 7 | WRITE_VCOM_REGISTER | `$2C` | `$6E` | VCOM voltage |
| 8 | BORDER_WAVEFORM_CTRL | `$3C` | `$33` | Border control |
| 9 | DATA_ENTRY_MODE_SETTING | `$11` | `$03` | Change to X incr, Y incr for data write |
| 10 | *(wait 2 ms)* | — | — | Settle time |
| 11 | WRITE_LUT_REGISTER | `$32` | *(70 bytes)* | Same LUT as DS_eink213 |
| 12 | *(wait 1000 ms)* | — | — | LUT settle time |

**Note:** The DS_eink290 switches data entry mode from Y-decrement (for address setup) to Y-increment (for sequential data write) after configuration. It also requires 4 data bytes for the Y address range since 296 > 255.

#### Frame Buffer Write Sequence (Bulk)

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for idle)* | — | Poll BUSY pin |
| 2 | SET_RAM_X_ADDR (`$44`) | `$00, $0F` | X window |
| 3 | SET_RAM_Y_ADDR (`$45`) | `$00, $00, $27, $01` | Y window: 0 to 295 |
| 4 | SET_RAM_X_CTR (`$4E`) | `$00` | X pointer = 0 |
| 5 | SET_RAM_Y_CTR (`$4F`) | `$00, $00` | Y pointer = 0 |
| 6 | WRITE_RAM (`$24`) | *(4,736 bytes)* | Entire frame buffer, sequential |
| 7 | TERMINATE_FRAME (`$FF`) | — | End write |
| 8 | *(wait 100 ms)* | — | — |
| 9 | DISPLAY_UPDATE_CTRL_2 (`$22`) | `$C7` | Clock on, analog on, display mode |
| 10 | MASTER_ACTIVATION (`$20`) | — | Trigger refresh |
| 11 | TERMINATE_FRAME (`$FF`) | — | End |

**Note:** Uses `$C7` for DISPLAY_UPDATE_CTRL_2 (vs. `$C4` for the DS_eink154 and DS_eink213).

---

### 5. DSWS_2in13BV3 — WaveShare 2.13" 3-Color (B/W/R)

**Controller:** SSD1680 variant — 104-source × 212-gate, dual-plane (black + red)

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 104 × 212 pixels |
| Frame buffer size | 5,512 bytes total |
| Black plane | 2,756 bytes (104 × 212 / 8) — first half |
| Red plane | 2,756 bytes (104 × 212 / 8) — second half |
| Pixel format | 1 bpp × 2 planes |
| Bytes per row | 13 (104 / 8) |

**Dual-plane color encoding:** Each pixel has one bit in the black plane and one bit in the red plane. The combination determines the displayed color:

| Black Plane Bit | Red Plane Bit | Displayed Color |
| --- | --- | --- |
| 0 | 0 | Red |
| 0 | 1 | Black |
| 1 | 0 | Red |
| 1 | 1 | White |

**Internal color constants:**
- `EINK213B_COLOR_BLACK = $00` → black bit = 0, red bit = 1
- `EINK213B_COLOR_RED = $02` → black bit = 1, red bit = 0
- `EINK213B_COLOR_WHITE = $03` → black bit = 1, red bit = 1

The driver maps the two bits from the internal color value to the correct position in each plane buffer.

#### Reset Sequence

```
RST pin HIGH  →  wait 200 ms
RST pin LOW   →  wait 2 ms
RST pin HIGH  →  wait 200 ms
Soft reset command $04
Wait for BUSY = HIGH (idle)
```

The longer reset timings (200 ms vs. 1–3 ms for SSD1675-based displays) are required by the SSD1680 variant controller.

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | PANEL_SETTINGS | `$00` | `$0F, $89` | Panel config: 3-color mode |
| 2 | RESOLUTION_SETTINGS | `$61` | `$68, $00, $D4` | 104 (=$68) width, 212 (=$D4) height |
| 3 | VCOM_DATA_INTERVAL | `$50` | `$77` | VCOM interval / border waveform |

**Note:** No external LUT is loaded — this display uses OTP (One-Time Programmable) internal waveform tables.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for BUSY = HIGH)* | — | Device idle when BUSY is HIGH |
| 2 | DATA_START_XMIT_1 (`$10`) | *(2,756 bytes)* | Black plane data |
| 3 | DATA_START_XMIT_2 (`$13`) | *(2,756 bytes)* | Red plane data |
| 4 | DISPLAY_REFRESH (`$12`) | — | Trigger display refresh |
| 5 | *(wait 100 ms)* | — | Allow refresh to begin |

#### Busy Pin Protocol

- **Polarity:** BUSY LOW = busy, BUSY HIGH = **idle** (opposite of SSD1608/SSD1675!)
- **Wait method:** Poll until BUSY reads HIGH

This is a critical difference from the standard Mikroe displays. The WaveShare controller signals idle when BUSY is high, not low.

#### Sleep Sequence

```
Command $50 — Data: $F7         (VCOM data interval)
Command $02 — Data: $07, $A5   (Power off + check code)
Wait 2000 ms
```

The `$A5` check code is a safety mechanism — the controller ignores the power-off unless this exact value follows.

---

### 6. DSAF_213BRYW — Adafruit 2.13" Quad-Color (JD79661AA)

**Controller:** JD79661AA (JADARD) — 176-source × 250-gate, 2-bit color

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Visible resolution | 122 × 250 pixels |
| Internal SRAM width | 176 pixels (176 source channels) |
| Frame buffer size | 11,000 bytes (176 × 250 / 4) |
| Pixel format | 2 bpp — 4 pixels per byte |
| Horizontal offset | +8 pixels (visible area starts at SRAM column 8) |
| Visible SRAM columns | 8–129 (122 pixels) |

**Pixel-to-byte mapping (2bpp):** Each byte holds 4 pixels in 2-bit pairs: bits [7:6] = pixel 0 (leftmost), bits [5:4] = pixel 1, bits [3:2] = pixel 2, bits [1:0] = pixel 3. The driver adds +8 to all user X coordinates to center the visible area within the 176-column SRAM.

#### Color Values

| User Enum | 2-bit Value | Panel LUT Name | Display |
| --- | --- | --- | --- |
| `EINK_COLOR_BLACK` | `$00` | Gray0 | Black |
| `EINK_COLOR_WHITE` | `$01` | Gray1 | White |
| `EINK_COLOR_YELLOW` | `$02` | Gray2 | Yellow |
| `EINK_COLOR_RED` | `$03` | Gray3 | Red |

#### Reset Sequence

```
RST pin HIGH  →  wait 10 ms
RST pin LOW   →  wait 10 ms
RST pin HIGH  →  wait 10 ms
Wait for BUSY = HIGH (idle)
```

The JD79661AA requires 10 ms reset timing — longer than standard SSD-based displays but shorter than WaveShare's 200 ms.

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | *(wait 10 ms)* | — | — | Post-reset settle |
| 2 | — | `$4D` | `$78` | Undocumented init register |
| 3 | PANEL_SETTING | `$00` | `$0F, $29` | RES=00 (176×296 mode), panel options |
| 4 | POWER_SETTING | `$01` | `$07, $00` | Power configuration |
| 5 | POWER_OFF_SEQ | `$03` | `$10, $54, $44` | Power-off sequence timing |
| 6 | BOOSTER_SOFT_START | `$06` | `$05, $00, $3F, $0A, $25, $12, $1A` | 7-byte booster config |
| 7 | VCOM_LUT | `$20` | `$37` | VCOM voltage LUT |
| 8 | TCON | `$60` | `$02, $02` | TCON timing |
| 9 | RESOLUTION | `$61` | `$00, $B0, $00, $FA` | 176 source (=$B0), 250 gate (=$FA) |
| 10 | — | `$E7` | `$1C` | Undocumented register |
| 11 | PWS | `$E3` | `$22` | Power-saving |
| 12 | — | `$B4` | `$D0` | Undocumented register |
| 13 | — | `$B5` | `$03` | Undocumented register |
| 14 | — | `$E9` | `$01` | Undocumented register |
| 15 | PLL_CONTROL | `$30` | `$08` | PLL clock rate |
| 16 | POWER_ON | `$04` | — | Power on command |
| 17 | *(wait for idle)* | — | — | Wait for BUSY = HIGH |

**Note:** Several undocumented registers (`$4D`, `$E7`, `$B4`, `$B5`, `$E9`) are used per the Adafruit reference implementation. No external LUT is loaded — the JD79661AA uses internal panel LUTs.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for BUSY = HIGH)* | — | Device idle |
| 2 | DATA_START_XMIT_1 (`$10`) | *(11,000 bytes)* | Full frame buffer (DTM — Display Transfer Memory) |
| 3 | DISPLAY_REFRESH (`$12`) | `$00` | Trigger refresh |
| 4 | *(wait 100 ms)* | — | Wait for BUSY to assert |
| 5 | *(wait for BUSY = HIGH)* | — | Refresh complete |

#### Busy Pin Protocol

- **Polarity:** BUSY LOW = busy, BUSY HIGH = idle (same as DSWS_2in13BV3)
- **Wait method:** Poll until BUSY reads HIGH

#### Sleep Sequence

```
Command $02 — Data: $00   (Power off)
Wait for BUSY = HIGH       (Power-off complete)
Command $07 — Data: $A5   (Deep sleep + check code)
```

---

### 7. DSGD_370Tch — Good Display 3.70" B/W with Touch (UC8253)

**Controller:** UC8253 (UltraChip) — 240-source × 416-gate, with partial refresh support

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 240 × 416 pixels |
| Frame buffer size | 12,480 bytes (240 × 416 / 8) |
| Prior frame buffer | 12,480 bytes (for delta/partial updates) |
| Total RAM usage | 24,960 bytes |
| Pixel format | 1 bpp — 8 pixels per byte |
| Bytes per row | 30 (240 / 8) |

**Dual frame buffers:** The driver maintains both a current frame (`deviceFrame`) and a prior frame (`deviceFramePrior`). The display controller uses both to compute differential updates, which reduces ghosting and enables fast partial refresh.

#### Color Values

| User Enum | Device Value | Display |
| --- | --- | --- |
| `EINK_COLOR_BLACK` | `$00` | Black |
| `EINK_COLOR_WHITE` | `$FF` | White |

#### Reset Sequence

```
RST pin LOW   →  wait 10 ms
RST pin HIGH  →  wait 10 ms
```

Simpler than other displays — BUSY clears within ~70µs, so 10 ms is conservative.

#### Initialization Sequence (Two-Stage)

**Stage 1 — Initial Clear:**

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | *(hardware reset)* | — | — | Reset sequence above |
| 2 | POWER_ON | `$04` | — | Power on |
| 3 | *(wait for idle)* | — | — | BUSY = HIGH |
| 4 | VCOM_DATA_INTRVL | `$50` | `$97` | VCOM interval |
| 5 | *(clear display to white)* | — | — | Fill buffer with $FF, write to display |
| 6 | *(sleep)* | — | — | Power off, deep sleep |
| 7 | *(wait 1000 ms)* | — | — | Recovery time |

**Stage 2 — Fast Mode Init:**

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | *(hardware reset)* | — | — | Reset again |
| 2 | POWER_ON | `$04` | — | Power on |
| 3 | *(wait for idle)* | — | — | BUSY = HIGH |
| 4 | CASCADE_SETTING | `$E0` | `$02` | Enable cascade mode |
| 5 | FORCE_TEMPERATURE | `$E5` | `$32` | Force temperature = 50°C (for fast waveform) |
| 6 | VCOM_DATA_INTRVL | `$50` | `$97` | VCOM interval |

**Note:** The two-stage initialization is critical. Stage 1 clears any residual image and ensures a known state. Stage 2 sets up the fast-refresh mode by forcing a warm temperature value, which selects a faster waveform from the controller's internal table.

#### Waveform LUT Data (Partial Refresh — 216 bytes)

The LUT is organized into multiple sections:

| Byte Range | Size | Purpose |
| --- | --- | --- |
| 0–209 | 210 bytes | Waveform phase timing data (5 LUT groups for VCOM, W2W, K2W, W2K, K2K) |
| 210 | 1 byte | PLL frame rate control |
| 211 | 1 byte | VGH/VGL setting (20V / −20V) |
| 212 | 1 byte | VSH = 15V |
| 213 | 1 byte | VSL = −15V |
| 214 | 1 byte | VSHR |
| 215 | 1 byte | VCOM_DC = −2.0V |

The LUT is loaded via five separate commands, one for each transition group:

| Command | Hex | Purpose |
| --- | --- | --- |
| VCOM_LUT | `$20` | VCOM waveform (42 bytes) |
| W2W_LUT | `$21` | White-to-white transitions (42 bytes) |
| K2W_LUT | `$22` | Black-to-white transitions (42 bytes) |
| W2K_LUT | `$23` | White-to-black transitions (42 bytes) |
| K2K_LUT | `$24` | Black-to-black transitions (42 bytes) |
| LUT_OPTION | `$2A` | 5 bytes: PLL, VGH/VGL, VSH, VSL, VSHR |
| VCOM_DATA_INTRVL | `$50` | 1 byte: VCOM_DC |

#### Full Refresh Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for idle)* | — | BUSY = HIGH |
| 2 | DISPLAY_START_OLD (`$10`) | *(12,480 bytes)* | Prior frame data |
| 3 | DISPLAY_START (`$13`) | *(12,480 bytes)* | New frame data |
| 4 | *(copy new → prior buffer)* | — | Update prior frame for next delta |
| 5 | DISPLAY_REFRESH (`$12`) | — | Trigger refresh |
| 6 | *(wait 1 ms)* | — | — |
| 7 | *(wait for idle)* | — | Refresh complete |
| 8 | POWER_OFF (`$02`) | — | Power off display driver |

**Delta update:** By sending both old and new frame data, the controller calculates which pixels changed and applies the appropriate waveform only to those pixels. This reduces ghosting and enables faster updates.

#### Partial Refresh Sequence

The display supports updating a rectangular region without refreshing the entire screen:

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | PARTIAL_ENTER (`$91`) | — | Enter partial update mode |
| 2 | PARTIAL_RESOLUTION (`$90`) | X_start, X_end−1, Y_start_hi, Y_start_lo, Y_end_hi, Y_end_lo−1, `$01` | Define update region |
| 3 | DISPLAY_START_OLD (`$10`) | *(region bytes)* | Prior frame data for region |
| 4 | DISPLAY_START (`$13`) | *(region bytes)* | New frame data for region |
| 5 | *(copy region to prior buffer)* | — | Update prior |
| 6 | DISPLAY_REFRESH (`$12`) | — | Refresh partial region |
| 7 | *(wait 10 ms minimum)* | — | Min 200µs, driver uses 10 ms |
| 8 | *(wait for idle)* | — | Refresh complete |
| 9 | POWER_OFF (`$02`) | — | — |

**Region data size:** `(X_range × Y_range) / 8` bytes for each plane.

#### Busy Pin Protocol

- **Polarity:** BUSY LOW = busy, BUSY HIGH = idle
- **Wait method:** Poll until BUSY reads HIGH

#### Sleep Sequence

```
Command $02 (POWER_OFF)     — no data
Wait for BUSY = HIGH         — power off complete
Wait 100 ms
Command $07 (DEEP_SLEEP)    — Data: $A5 (check code)
```

---

### 8. DSWS_5in65F — WaveShare 5.65" 7-Color ACeP (UC8159)

**Controller:** UC8159 (UltraChip) — 600-source × 448-gate, Advanced Color ePaper (ACeP)

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display resolution | 600 × 448 pixels |
| Frame buffer size | 134,400 bytes (600 × 448 / 2) |
| Pixel format | 4 bpp — 2 pixels per byte (nibble-packed) |
| Upper nibble (bits 7:4) | Left pixel |
| Lower nibble (bits 3:0) | Right pixel |
| Bytes per row | 300 (600 / 2) |

**Important:** This is the largest frame buffer of any supported display at 134,400 bytes — a significant P2 memory allocation.

#### Color Values (3-bit, stored in 4-bit nibbles)

| User Enum | Nibble Value | Display |
| --- | --- | --- |
| `EINK_COLOR_BLACK` | `$0` | Black |
| `EINK_COLOR_WHITE` | `$1` | White |
| `EINK_COLOR_GREEN` | `$2` | Green |
| `EINK_COLOR_BLUE` | `$3` | Blue |
| `EINK_COLOR_RED` | `$4` | Red |
| `EINK_COLOR_YELLOW` | `$5` | Yellow |
| `EINK_COLOR_ORANGE` | `$6` | Orange |
| *(CLEAN)* | `$7` | Pre-clean waveform (internal use) |

**Pixel packing:** Two pixels per byte. For pixel at x-position: if x is even, it occupies the upper nibble (bits 7:4); if x is odd, the lower nibble (bits 3:0). Byte offset = `(y × 300) + (x >> 1)`.

**Nibble swap on write:** When transmitting pixel data to the display, the driver swaps the nibbles of each byte: `outByte = ((inByte & $F0) >> 4) | ((inByte & $0F) << 4)`. This is required by the UC8159's data format expectations.

#### Reset Sequence

```
RST pin LOW   →  wait 1 ms
RST pin HIGH  →  wait 200 ms
Wait for BUSY = desired state
```

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | PANEL_SETTING | `$00` | `$EF, $08` | Panel options for ACeP mode |
| 2 | POWER_SETTING | `$01` | `$37, $00, $23, $23` | VGH/VGL and source voltages |
| 3 | POWER_OFF_SEQ | `$03` | `$00` | 1-frame power-off |
| 4 | BOOSTER_SOFT_START | `$06` | `$C7, $C7, $1D` | Booster phase timing |
| 5 | PLL_CONTROL | `$30` | `$3C` | 50 Hz frame rate |
| 6 | TEMP_SENSOR_ENABLE | `$41` | `$00` | Internal temp sensor, 0 offset |
| 7 | VCOM_DATA_INTERVAL | `$50` | `$37` | VCOM interval |
| 8 | TCON | `$60` | `$22` | TCON setting |
| 9 | RESOLUTION_SETTING | `$61` | `$02, $58, $01, $C0` | 600 (=$0258) × 448 (=$01C0) |
| 10 | PWS | `$E3` | `$AA` | Power-saving |
| 11 | *(wait 100 ms)* | — | — | Settle |
| 12 | — | `$82` | `$37` | VCOM offset |

**Note:** No external LUT — the UC8159 uses internal OTP waveform tables optimized for 7-color ACeP technology.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | *(wait for BUSY = not busy)* | — | See busy polarity note below |
| 2 | RESOLUTION_SETTING (`$61`) | *(4 bytes: W_hi, W_lo, H_hi, H_lo)* | Set resolution before data write |
| 3 | DATA_START_XMIT_1 (`$10`) | *(134,400 bytes, nibble-swapped)* | Pixel data |
| 4 | DISPLAY_REFRESH (`$12`) | — | Trigger refresh |
| 5 | *(wait 200 ms)* | — | Wait for BUSY to assert |
| 6 | *(wait for BUSY = idle)* | — | Full refresh cycle (~35 seconds) |
| 7 | *(power off sequence)* | — | |
| 8 | *(wait 200 ms)* | — | Post-refresh settle |

**Refresh time:** The 7-color ACeP refresh takes approximately **35 seconds** — by far the slowest of all supported displays. This is inherent to ACeP technology which must cycle through multiple voltage phases for each color.

#### Busy Pin Protocol

- **Polarity:** BUSY HIGH = **busy**, BUSY LOW = idle (**opposite of all other displays!**)
- **Wait method:** The driver uses `wrCmdWtDesiredBusyWS565F(cmd, desiredState)` which polls until BUSY matches the desired state
- During long operations, BUSY stays HIGH for the entire refresh cycle

This is a critical implementation difference — every other display in this driver has BUSY LOW = busy or BUSY HIGH = idle, but the UC8159 inverts this convention.

#### Sleep Sequence

```
Wait 100 ms
Command $07 (Deep Sleep) — Data: $A5 (check code)
Wait 100 ms
RST pin LOW                  (hardware reset hold)
```

---

### 9. DSWS_370BW — WaveShare 3.70" B/W (SSD1677)

**Controller:** SSD1677 (Solomon Systech) — 280-source × 480-gate

#### Memory Organization

| Parameter | Value |
| --- | --- |
| Display physical size | 480 wide × 280 tall |
| Source (column) direction | 280 pixels |
| Gate (row) direction | 480 rows |
| Frame buffer size | 16,800 bytes (280 × 480 / 8) |
| Pixel format | 1 bpp — 8 pixels per byte |
| Bytes per row | 35 (280 / 8) |
| RAM X range | 0–279 (note: SSD1677 uses 16-bit X addressing) |
| RAM Y range | 0–479 |

**SSD1677 16-bit addressing:** Unlike the SSD1675 which uses 8-bit X addressing (byte units), the SSD1677 uses 16-bit addresses for both X and Y coordinates. The SET_RAM_X_ADDR command takes 4 data bytes (start_lo, start_hi, end_lo, end_hi) instead of 2.

#### Reset Sequence

```
RST pin HIGH  →  wait 200 ms
RST pin LOW   →  wait 5 ms    (shorter LOW pulse than SSD1675)
RST pin HIGH  →  wait 200 ms
```

The SSD1677 requires longer HIGH-state times (200 ms each) but a shorter LOW pulse (5 ms) compared to other controllers.

#### Initialization Command Sequence

| Step | Command | Hex | Data Bytes | Description |
| --- | --- | --- | --- | --- |
| 1 | DRIVER_OUTPUT_CTRL | `$01` | `$DF, $01, $00` | Gate count = 479 (480-1) as 16-bit: lo=$DF, hi=$01 |
| 2 | Gate Voltage | `$03` | `$00` | Default gate driving voltage |
| 3 | Source Voltage | `$04` | `$41, $A8, $32` | VSH1, VSH2, VSL voltage settings |
| 4 | DATA_ENTRY_MODE | `$11` | `$03` | X increment, Y increment |
| 5 | BORDER_WAVEFORM | `$3C` | `$03` | Border waveform control |
| 6 | BOOSTER_SOFT_START | `$0C` | `$AE, $C7, $C3, $C0, $C0` | 5-byte booster config |
| 7 | Temp Sensor Enable | `$18` | `$80` | Internal temperature sensor |
| 8 | WRITE_VCOM | `$2C` | `$44` | VCOM voltage |
| 9 | Display Option | `$37` | `$00, $FF, $FF, $FF, $FF, $4F, $FF, $FF, $FF, $FF` | 10-byte display option register |
| 10 | SET_RAM_X_ADDR | `$44` | `$00, $00, $17, $01` | X: 0 to 279 (16-bit: $0000 to $0117) |
| 11 | SET_RAM_Y_ADDR | `$45` | `$00, $00, $DF, $01` | Y: 0 to 479 (16-bit: $0000 to $01DF) |
| 12 | DISPLAY_UPDATE_CTRL_2 | `$22` | `$CF` | Update control sequence |

**SSD1677-specific notes:**
- Command `$18` for temperature sensor (not `$1A` used by SSD1608/1675)
- Command `$37` with 10 data bytes for display options — unique to SSD1677
- Command `$03` (gate voltage) and `$04` (source voltage) are explicit — not needed on SSD1675
- 16-bit addressing throughout: X and Y start/end positions are all 2 bytes each

#### LUT Data — GC Mode (Grayscale Clear, 105 bytes)

The SSD1677 uses a 105-byte LUT for full refresh (GC mode):

```
Offset  00: $2A $05 $00 $00 $00 $00 $00 $00 $00 $00
Offset  10: $05 $2A $00 $00 $00 $00 $00 $00 $00 $00
Offset  20: $2A $15 $00 $00 $00 $00 $00 $00 $00 $00
Offset  30: $05 $0A $00 $00 $00 $00 $00 $00 $00 $00
Offset  40: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset  50: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset  60: $00 $00 $00 $00 $00
Offset  65: $00 $02 $03 $0A $00 $02 $06 $0A $05 $00
Offset  75: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset  85: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset  95: $22 $22 $22 $22 $22
```

**LUT structure:** The 105-byte LUT is organized as:
- Bytes 0–49: 5 waveform phase groups × 10 bytes each (voltage level + repeat definitions)
- Bytes 50–64: Reserved/padding
- Bytes 65–94: Phase timing parameters (TP values defining pulse durations)
- Bytes 95–99: Frame rate / gate scan group settings
- Bytes 100–104: Additional timing

#### LUT Data — A2 Mode (Fast/Partial, 105 bytes)

A second 105-byte LUT is available for fast partial updates (A2 mode), with simplified waveforms that trade image quality for speed.

#### Frame Buffer Write Sequence

| Step | Command | Data | Description |
| --- | --- | --- | --- |
| 1 | SET_RAM_X_CTR (`$4E`) | `$00, $00` | X pointer = 0 (16-bit) |
| 2 | SET_RAM_Y_CTR (`$4F`) | `$00, $00` | Y pointer = 0 (16-bit) |
| 3 | WRITE_RAM (`$24`) | *(16,800 bytes)* | Frame buffer data |
| 4 | WRITE_LUT_REGISTER (`$32`) | *(105 bytes)* | Load GC LUT |
| 5 | MASTER_ACTIVATION (`$20`) | — | Trigger display update |
| 6 | *(wait ~5 seconds)* | — | Full refresh time |

**Note on LUT loading:** Unlike some controllers where the LUT is loaded once during init, the SSD1677 driver reloads the LUT before each display update. This allows switching between GC (full quality) and A2 (fast) modes on the fly.

#### Busy Pin Protocol

- **Polarity:** BUSY LOW = busy, BUSY HIGH = idle
- **Wait method:** Poll until BUSY reads HIGH
- **Current implementation:** Uses a fixed 5-second wait instead of polling BUSY — this is noted as a temporary workaround in the driver

#### Sleep Sequence

```
Command $10 (DEEP_SLEEP_MODE) — Data: $03 (deep sleep mode 2)
```

Deep sleep mode 2 powers down the analog circuits while retaining register settings.

---

## Cross-Display Summary Tables

### BUSY Pin Polarity

| Display | BUSY = Busy | BUSY = Idle | Driver Wait Condition |
| --- | --- | --- | --- |
| DS_eink154 | HIGH | LOW | Wait for LOW |
| DS_eink200gs | HIGH | LOW | Wait for LOW |
| DS_eink213 | HIGH | LOW | Wait for LOW |
| DS_eink290 | HIGH | LOW | Wait for LOW |
| DSWS_2in13BV3 | LOW | **HIGH** | Wait for HIGH |
| DSAF_213BRYW | LOW | **HIGH** | Wait for HIGH |
| DSGD_370Tch | LOW | **HIGH** | Wait for HIGH |
| DSWS_5in65F | **HIGH** | LOW | Wait for desired state (configurable) |
| DSWS_370BW | LOW | **HIGH** | Wait for HIGH |

### Color Encoding Comparison

| Display | BPP | Pixels/Byte | Black | White | Other Colors |
| --- | --- | --- | --- | --- | --- |
| DS_eink154 | 1 | 8 | `$00` | `$FF` | — |
| DS_eink200gs | 2 | 4 | `$00` | `$FF` | `$55` dk grey, `$AA` lt grey |
| DS_eink213 | 1 | 8 | `$00` | `$FF` | — |
| DS_eink290 | 1 | 8 | `$00` | `$FF` | — |
| DSWS_2in13BV3 | 1×2 planes | 8 (per plane) | blk=0,red=1 | blk=1,red=1 | blk=1,red=0 → Red |
| DSAF_213BRYW | 2 | 4 | `$00` | `$01` | `$02` yellow, `$03` red |
| DSGD_370Tch | 1 | 8 | `$00` | `$FF` | — |
| DSWS_5in65F | 4 | 2 | `$0` | `$1` | `$2`–`$6` (G/B/R/Y/O), `$7` clean |
| DSWS_370BW | 1 | 8 | `$00` | `$FF` | — |

### LUT Requirements

| Display | Controller | LUT Source | LUT Size | Loaded When |
| --- | --- | --- | --- | --- |
| DS_eink154 | SSD1608 | External | 30 bytes | Once at init |
| DS_eink200gs | SSD1606 | External | 90 bytes | Once at init |
| DS_eink213 | SSD1675 | External | 70 bytes | Once at init |
| DS_eink290 | SSD1675 | External | 70 bytes (shared with 213) | Once at init |
| DSWS_2in13BV3 | SSD1680 | Internal (OTP) | — | Not needed |
| DSAF_213BRYW | JD79661AA | Internal | — | Not needed |
| DSGD_370Tch | UC8253 | External | 216 bytes (5 groups + options) | Once at init + per partial |
| DSWS_5in65F | UC8159 | Internal (OTP) | — | Not needed |
| DSWS_370BW | SSD1677 | External | 105 bytes (GC) + 105 bytes (A2) | Per display update |

### Frame Buffer Architecture

| Display | Buffer Count | Total RAM | Write Method | Refresh Support |
| --- | --- | --- | --- | --- |
| DS_eink154 | 1 | 5,000 B | Bulk write | Full only |
| DS_eink200gs | 1 | 3,096 B | Bulk write | Full only |
| DS_eink213 | 1 | 4,000 B | Line-by-line | Full only |
| DS_eink290 | 1 | 4,736 B | Bulk write | Full only |
| DSWS_2in13BV3 | 1 (dual-plane) | 5,512 B | Plane-by-plane | Full only |
| DSAF_213BRYW | 1 | 11,000 B | Bulk write | Full only |
| DSGD_370Tch | **2** (current + prior) | **24,960 B** | Old + new write | Full + **partial** |
| DSWS_5in65F | 1 | 134,400 B | Bulk write (nibble-swapped) | Full only |
| DSWS_370BW | 1 | 16,800 B | Bulk write + LUT reload | Full (GC or A2) |

### Reset Timing Comparison

| Display | HIGH→LOW | LOW Duration | LOW→HIGH | Post-Reset |
| --- | --- | --- | --- | --- |
| DS_eink154 | 1 ms | 2 ms | 3 ms | — |
| DS_eink200gs | 1 ms | 2 ms | 3 ms | — |
| DS_eink213 | 1 ms | 2 ms | 3 ms | — |
| DS_eink290 | 1 ms | 2 ms | 3 ms | — |
| DSWS_2in13BV3 | 200 ms | 2 ms | 200 ms | Soft reset ($04) + wait |
| DSAF_213BRYW | 10 ms | 10 ms | 10 ms | Wait for idle |
| DSGD_370Tch | — | 10 ms | 10 ms | — |
| DSWS_5in65F | — | 1 ms | 200 ms | Wait for BUSY |
| DSWS_370BW | 200 ms | 5 ms | 200 ms | — |

### Approximate Refresh Times

| Display | Full Refresh | Notes |
| --- | --- | --- |
| DS_eink154 | ~2 seconds | Standard B/W |
| DS_eink200gs | ~2 seconds | 4-level grayscale |
| DS_eink213 | ~2 seconds | Standard B/W |
| DS_eink290 | ~2 seconds | Standard B/W |
| DSWS_2in13BV3 | ~15 seconds | 3-color (red requires extra waveform phases) |
| DSAF_213BRYW | ~15 seconds | 4-color |
| DSGD_370Tch | ~1–2 seconds (partial: <1 sec) | Fast mode with partial refresh |
| DSWS_5in65F | **~35 seconds** | 7-color ACeP — slowest |
| DSWS_370BW | ~5 seconds | GC mode; A2 mode is faster |

---

Additional pages:

- [Display Specifications](./eInk-display-specs.md)
- [Display Images and Hardware Docs](./README.md)
- [eInk Touch Wiring Guide](./eInk-touch.md)
- [Display Orientation Guide](./Orientation.md)
- [Top README](https://github.com/ironsheep/P2-Click-eINK) — Return to the top-level README for this repository

---

> If you like my work and/or this has helped you in some way then feel free to help me out for a couple of :coffee:'s or :pizza: slices!
>
> [![coffee](https://www.buymeacoffee.com/assets/img/custom_images/black_img.png)](https://www.buymeacoffee.com/ironsheep) &nbsp;&nbsp; -OR- &nbsp;&nbsp; [![Patreon](../Images/patreon.png)](https://www.patreon.com/IronSheep?fan_landing=true)[Patreon.com/IronSheep](https://www.patreon.com/IronSheep?fan_landing=true)

---

## Disclaimer and Legal

> *Parallax, Propeller Spin, and the Parallax and Propeller Hat logos* are trademarks of Parallax Inc., dba Parallax Semiconductor
>
> This project is a community project not for commercial use.
>
> This project is in no way affiliated with, authorized, maintained, sponsored or endorsed by *Parallax Inc., dba Parallax Semiconductor* or any of its affiliates or subsidiaries.

---

## License

Licensed under the MIT License.

Follow these links for more information:

### [Copyright](../copyright) | [License](../LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://img.shields.io/badge/License-MIT-yellow.svg
