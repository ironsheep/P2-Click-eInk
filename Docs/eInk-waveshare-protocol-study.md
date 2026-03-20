# WaveShare eINK Display Protocol Study

A detailed protocol-level study derived from WaveShare's reference code examples across multiple platforms (Arduino, Arduino R4, STM32, Raspberry Pi C, Raspberry Pi Python). Each display section catalogs findings from every available code example, then synthesizes a unified protocol understanding.

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)

---

## Table of Contents

- [Introduction](#introduction)
- [Display 1: WaveShare 2.13" 3-Color B/W/R (DSWS_2in13BV3)](#display-1-waveshare-213-3-color-bwr-dsws_2in13bv3)
  - [Variant A: EPD2IN13BC (Original B/C Model)](#variant-a-epd2in13bc-original-bc-model)
  - [Variant B: EPD2IN13B_V3 (Revised Model)](#variant-b-epd2in13b_v3-revised-model)
  - [Variant C: EPD2IN13B_V4 (Latest Model — Different IC!)](#variant-c-epd2in13b_v4-latest-model--different-ic)
  - [2.13" B/W/R Synthesis — Cross-Variant Comparison](#213-bwr-synthesis--cross-variant-comparison)
- [Display 2: WaveShare 5.65" 7-Color ACeP (DSWS_5in65F)](#display-2-waveshare-565-7-color-acep-dsws_5in65f)
  - [Example Set: EPD5IN65F (All Platforms)](#example-set-epd5in65f-all-platforms)
  - [5.65" ACeP Synthesis — Cross-Platform Comparison](#565-acep-synthesis--cross-platform-comparison)
- [Display 3: WaveShare 3.70" B/W (DSWS_370BW)](#display-3-waveshare-370-bw-dsws_370bw)
  - [Example Set: EPD3IN7 (All Platforms)](#example-set-epd3in7-all-platforms)
  - [E-paper Separate Program: 3in7_e-Paper_G Variant](#e-paper-separate-program-3in7_e-paper_g-variant)
  - [3.70" B/W Synthesis — Cross-Platform Comparison](#370-bw-synthesis--cross-platform-comparison)
- [Master Summary Tables](#master-summary-tables)

---

## Introduction

WaveShare provides no formal datasheets for their eINK display modules. The only protocol documentation available is their reference source code, distributed across multiple platform implementations. Each implementation may reveal different aspects of the protocol, contain platform-specific variations, or even target different hardware revisions of the same display.

This document catalogs every technical detail found in the WaveShare `e-Paper-master` reference code repository for each display supported by our P2 driver. By studying all implementations independently and then combining findings, we construct the most complete protocol picture available.

### Source Repository Structure

All reference code is located under `REF-Arduino/e-Paper-master/` with these platform directories:

| Platform | Path | Language |
| --- | --- | --- |
| Arduino (legacy) | `Arduino/epd{name}/` | C++ (.cpp/.h/.ino) |
| Arduino R4 | `Arduino_R4/src/e-Paper/` and `Arduino_R4/src/Examples/` | C++ (.cpp/.h) |
| STM32 | `STM32/STM32-F103ZET6/User/e-Paper/` and `.../Examples/` | C (.c/.h) |
| Raspberry Pi C | `RaspberryPi_JetsonNano/c/lib/e-Paper/` and `.../examples/` | C (.c/.h) |
| Raspberry Pi Python | `RaspberryPi_JetsonNano/python/lib/waveshare_epd/` | Python (.py) |
| Separate Program | `E-paper_Separate_Program/{name}/` | Multi-platform |

---

## Display 1: WaveShare 2.13" 3-Color B/W/R (DSWS_2in13BV3)

Our driver's `DSWS_2in13BV3` display type. Three distinct hardware variants exist in the reference code, each with a different controller IC and protocol differences.

---

### Variant A: EPD2IN13BC (Original B/C Model)

**Files studied:** `Arduino/epd2in13bc/`, `Arduino_R4/.../EPD_2in13bc.*`, `STM32/.../EPD_2in13bc.*`, `RaspberryPi_JetsonNano/c/.../EPD_2in13bc.*`, `RaspberryPi_JetsonNano/python/.../epd2in13bc.py`

#### Controller IC

**SSD1608** (implied) — Identified from the register command set pattern. Uses BOOSTER_SOFT_START (`$06`), POWER_ON (`$04`), PANEL_SETTING (`$00`), and OTP-based LUT. This is the original controller used on the first WaveShare 2.13" B/C tri-color displays.

#### Display Resolution

| Parameter | Value |
| --- | --- |
| Width | 104 pixels |
| Height | 212 pixels |
| Planes | 2 (black + red/yellow) |
| Bytes per row | 13 (104 / 8) |
| Buffer per plane | 2,756 bytes (13 × 212) |
| Total buffer | 5,512 bytes |

#### Reset Sequence

| Platform | HIGH Duration | LOW Duration | HIGH Duration |
| --- | --- | --- | --- |
| Arduino / Arduino R4 / STM32 | 200 ms | 2 ms | 200 ms |
| RPi Python | 200 ms | 5 ms | 200 ms |

#### Initialization Command Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | BOOSTER_SOFT_START | `$06` | `$17, $17, $17` | Booster phase A/B/C timing |
| 2 | POWER_ON | `$04` | — | Power on; wait for BUSY idle |
| 3 | PANEL_SETTING | `$00` | `$8F` | Panel configuration |
| 4 | VCOM_DATA_INTERVAL | `$50` | `$37` or `$F0` | See platform differences |
| 5 | RESOLUTION_SETTING | `$61` | `$68, $00, $D4` | W=104, H=212 |

**Platform difference at step 4:** Arduino (legacy) sends `$37`, while Arduino R4 / STM32 / RPi send `$F0`.

#### LUT Data

**None sent explicitly.** The BC variant uses factory-programmed OTP (One-Time Programmable) internal LUT. The LUT command registers are defined (`$20` VCOM, `$21` W2W, `$22` B2W, `$23` W2B, `$24` B2B) but never written to — the controller loads them automatically from OTP.

#### Frame Buffer Write Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | DATA_START_XMIT_1 | `$10` | *(2,756 bytes)* | Black plane data |
| 2 | DATA_START_XMIT_2 | `$13` | *(2,756 bytes)* | Red plane data |
| 3 | DISPLAY_REFRESH | `$12` | — | Trigger refresh |
| 4 | *(wait for idle)* | — | — | BUSY = idle |

**Note:** Arduino R4 and STM32 implementations insert a `PARTIAL_OUT` (`$92`) command between planes. Arduino legacy and RPi Python do not.

**Pixel encoding:** MSB-first within each byte: bit 7 = leftmost pixel, bit 0 = rightmost pixel. For each plane, `$00` = ink (black or red), `$FF` = no ink (white).

#### Busy Pin Protocol

- **Polarity:** LOW = busy, HIGH = idle
- **Wait:** `while(pin == 0) { delay(100); }`

#### Partial Refresh Support

**YES — Full partial refresh support in BC variant:**

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | PARTIAL_IN | `$91` | — | Enter partial mode |
| 2 | PARTIAL_WINDOW | `$90` | X_start, X_end, Y_start_hi, Y_start_lo, Y_end_hi, Y_end_lo, `$01` | Define region |
| 3 | DATA_START_XMIT_1 | `$10` | *(region bytes)* | Black plane partial data |
| 4 | DATA_START_XMIT_2 | `$13` | *(region bytes)* | Red plane partial data |
| 5 | PARTIAL_OUT | `$92` | — | Exit partial mode |

**Constraints:** X coordinates must be byte-aligned (multiple of 8).

#### Sleep Sequence

```
Command $02 (POWER_OFF)         — wait for idle
Command $07 (DEEP_SLEEP)        — Data: $A5 (check code)
```

Wake requires hardware reset (RST pin pulse) followed by full re-initialization.

#### Platform-Specific Differences (BC)

| Feature | Arduino | Arduino R4 | STM32 | RPi C | RPi Python |
| --- | --- | --- | --- | --- | --- |
| VCOM interval (`$50`) data | `$37` | `$F0` | `$F0` | `$F0` | `$F0` |
| PARTIAL_OUT between planes | No | Yes | Yes | No | No |
| Reset LOW duration | 2 ms | 2 ms | 2 ms | 2 ms | 5 ms |

---

### Variant B: EPD2IN13B_V3 (Revised Model)

**Files studied:** `Arduino/epd2in13b_V3/` (if present), `Arduino_R4/.../EPD_2in13b_V3.*`, `STM32/.../EPD_2in13b_V3.*`, `RaspberryPi_JetsonNano/c/.../EPD_2in13b_V3.*`, `RaspberryPi_JetsonNano/python/.../epd2in13b_V3.py`

#### Controller IC

**SSD1680** — Newer controller revision than the BC variant. Distinguished by different init sequence (POWER_ON before PANEL_SETTING), OTP LUT selection via panel setting byte, and the requirement to send GET_STATUS (`$71`) before reading the BUSY pin.

#### Display Resolution

Same as BC: **104 × 212 pixels**, dual-plane, 5,512 bytes total.

#### Reset Sequence

| Platform | HIGH Duration | LOW Duration | HIGH Duration |
| --- | --- | --- | --- |
| Arduino (legacy) | 200 ms | 2 ms | 200 ms |
| Arduino R4 / STM32 | 100 ms | 2 ms | 10 ms |
| RPi Python | 200 ms | 2 ms | 200 ms |

**Notable:** Arduino R4 and STM32 use significantly shorter post-reset delays (10 ms vs 200 ms).

#### Initialization Command Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | POWER_ON | `$04` | — | Power on **first** (different from BC!) |
| 2 | *(wait for idle)* | — | — | Wait for BUSY = HIGH |
| 3 | PANEL_SETTING | `$00` | `$0F, $89` | OTP LUT, 128×296 mode, panel options |
| 4 | RESOLUTION_SETTING | `$61` | `$68, $00, $D4` | W=104, H=212 |
| 5 | VCOM_DATA_INTERVAL | `$50` | `$77` | VCOM interval / border waveform |

**Key difference from BC:** V3 sends POWER_ON (`$04`) *before* PANEL_SETTING, whereas BC sends it after BOOSTER_SOFT_START. The `$0F` panel setting byte explicitly selects "LUT from OTP, 128×296 mode."

#### LUT Data

**None sent.** The `$0F` parameter to PANEL_SETTING tells the SSD1680 to use its internal OTP LUT.

#### Frame Buffer Write Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | DATA_START_XMIT_1 | `$10` | *(2,756 bytes)* | Black plane |
| 2 | *(delay 2 ms)* | — | — | Inter-plane settle |
| 3 | DATA_START_XMIT_2 | `$13` | *(2,756 bytes)* | Red plane |
| 4 | *(delay 2 ms)* | — | — | Settle |
| 5 | DISPLAY_REFRESH | `$12` | — | Trigger refresh |
| 6 | *(delay 100 ms)* | — | — | Wait for BUSY to assert |
| 7 | *(wait for idle)* | — | — | Refresh complete |

#### Busy Pin Protocol — V3-Specific Requirement

- **Polarity:** LOW = busy, HIGH = idle (same as BC)
- **Critical difference:** V3 **requires** sending GET_STATUS (`$71`) before reading the BUSY pin!

```
do {
    SendCommand($71)           // MUST send this before reading BUSY!
    busy = ReadPin(BUSY)
    busy = !(busy & $01)
} while (busy)
delay(200)                      // Extra settle after idle detected
```

This GET_STATUS requirement is confirmed across **all** V3 platform implementations (Arduino R4, STM32, RPi C, RPi Python).

#### Partial Refresh Support

**Not implemented** in any V3 example.

#### Sleep Sequence

```
Command $50 — Data: $F7         (VCOM data interval change)
Command $02 (POWER_OFF)         — wait for idle
Command $07 (DEEP_SLEEP)        — Data: $A5 (check code)
```

**Different from BC:** V3 sets VCOM interval to `$F7` before power-off.

#### Platform-Specific Differences (V3)

| Feature | Arduino | Arduino R4 | STM32 | RPi Python |
| --- | --- | --- | --- | --- |
| Reset timing (H/L/H) | 200/2/200 ms | 100/2/10 ms | 100/2/10 ms | 200/2/200 ms |
| BUSY check: `$71` required | Yes | Yes | Yes | Yes |
| Post-busy settle delay | 200 ms | 200 ms | 200 ms | 100 ms |
| Partial refresh | No | No | No | No |

---

### Variant C: EPD2IN13B_V4 (Latest Model — Different IC!)

**Files studied:** `Arduino/epd2in13b_V4/`, `Arduino_R4/.../EPD_2in13b_V4.*`, `STM32/.../EPD_2in13b_V4.*`, `RaspberryPi_JetsonNano/c/.../EPD_2in13b_V4.*`, `RaspberryPi_JetsonNano/python/.../epd2in13b_V4.py`

#### Controller IC

**SSD1681** — Completely different controller from BC and V3. Identified by:
- Software reset command (`$12`) instead of power-on (`$04`) at init
- SSD1675-style register set: DRIVER_OUTPUT_CONTROL (`$01`), DATA_ENTRY_MODE (`$11`), SET_RAM_X/Y (`$44`/`$45`)
- Data written to RAM registers `$24`/`$26` instead of transmission commands `$10`/`$13`
- Display refresh via MASTER_ACTIVATION (`$20`) instead of DISPLAY_REFRESH (`$12`)

#### Display Resolution — DIFFERENT FROM V3/BC!

| Parameter | Value |
| --- | --- |
| **Width** | **122 pixels** (not 104!) |
| **Height** | **250 pixels** (not 212!) |
| Buffer width | 128 pixels / 8 = 16 bytes per row (byte-aligned to 128) |
| Buffer per plane | 4,000 bytes (16 × 250) |
| Total buffer | 8,000 bytes |

**This is a completely different physical display panel than V3/BC.**

#### Reset Sequence

| Platform | HIGH Duration | LOW Duration | HIGH Duration |
| --- | --- | --- | --- |
| All platforms | 20 ms | 2 ms | 20 ms |

Much shorter than V3/BC — consistent across all implementations.

#### Initialization Command Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | SW_RESET | `$12` | — | Software reset |
| 2 | *(wait for idle)* | — | — | |
| 3 | DRIVER_OUTPUT_CTRL | `$01` | `$F9, $00, $00` | Gate count = 249 (250−1) |
| 4 | DATA_ENTRY_MODE | `$11` | `$03` | X incr, Y incr |
| 5 | SET_RAM_X_ADDR | `$44` | `$00, $0F` | X: 0–15 (128 pixels / 8) |
| 6 | SET_RAM_Y_ADDR | `$45` | `$00, $00, $F9, $00` | Y: 0–249 |
| 7 | SET_RAM_X_CTR | `$4E` | `$00` | X pointer = 0 |
| 8 | SET_RAM_Y_CTR | `$4F` | `$00, $00` | Y pointer = 0 |
| 9 | BORDER_WAVEFORM | `$3C` | `$05` | Border control |
| 10 | TEMP_SENSOR | `$18` | `$80` | Internal temp sensor |
| 11 | DISPLAY_UPDATE_CTRL | `$21` | `$80, $80` | Display update options |
| 12 | *(wait for idle)* | — | — | |

**This init sequence is SSD1675/1681-style, completely different from the SSD1680 V3 or SSD1608 BC.**

#### LUT Data

**None sent.** The SSD1681 uses internal factory LUT.

#### Frame Buffer Write Sequence — DIFFERENT COMMANDS!

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | WRITE_RAM_BW | `$24` | *(4,000 bytes)* | Black plane via RAM register |
| 2 | WRITE_RAM_RED | `$26` | *(4,000 bytes)* | Red plane via RAM register |
| 3 | MASTER_ACTIVATION | `$20` | — | Trigger display update |
| 4 | *(wait for idle)* | — | — | |

**Critical difference:** V4 uses `$24`/`$26` (RAM write registers) instead of `$10`/`$13` (data transmission commands), and `$20` (MASTER_ACTIVATION) instead of `$12` (DISPLAY_REFRESH).

#### Busy Pin Protocol — INVERTED FROM V3/BC!

- **Polarity:** **HIGH = busy, LOW = idle** (opposite of V3 and BC!)
- **Wait:** `while(pin == 1) { delay(10); }`
- **No GET_STATUS (`$71`) required** (unlike V3)

This is a critical incompatibility. Code written for V3/BC will hang on V4 (and vice versa) due to the inverted busy polarity.

#### Partial Refresh Support

**Not implemented** in any V4 example.

#### Sleep Sequence

```
Command $10 (DEEP_SLEEP) — Data: $01
delay(100)
```

Much simpler than V3/BC. Uses `$01` check code instead of `$A5`.

#### Platform-Specific Differences (V4)

All platforms are **identical** for V4 — no variations found in init sequence, timing, or commands.

---

### 2.13" B/W/R Synthesis — Cross-Variant Comparison

#### Resolution Differences

| Variant | Width | Height | Buffer/Plane | Total Buffer |
| --- | --- | --- | --- | --- |
| BC (original) | 104 | 212 | 2,756 B | 5,512 B |
| V3 (revised) | 104 | 212 | 2,756 B | 5,512 B |
| **V4 (latest)** | **122** | **250** | **4,000 B** | **8,000 B** |

#### Controller IC Identification

| Variant | Controller | Evidence |
| --- | --- | --- |
| BC | SSD1608 | Register set: `$06`/`$04`/`$00`/`$50`/`$61`; OTP LUT |
| V3 | SSD1680 | Same register set as BC but different init order; requires `$71` GET_STATUS |
| V4 | SSD1681 | Completely different register set: `$01`/`$11`/`$44`/`$45`/`$24`/`$26`/`$20` |

#### Command Register Comparison

| Function | BC | V3 | V4 |
| --- | --- | --- | --- |
| Init start | `$06` (booster) | `$04` (power on) | `$12` (SW reset) |
| Black data write | `$10` | `$10` | `$24` |
| Red data write | `$13` | `$13` | `$26` |
| Display refresh | `$12` | `$12` | `$20` |
| Deep sleep | `$07` + `$A5` | `$07` + `$A5` | `$10` + `$01` |

#### Busy Pin Polarity

| Variant | Busy State | Idle State | Special Requirement |
| --- | --- | --- | --- |
| BC | LOW (0) | HIGH (1) | None |
| V3 | LOW (0) | HIGH (1) | Must send `$71` before reading |
| **V4** | **HIGH (1)** | **LOW (0)** | None |

#### Reset Timing

| Variant | HIGH→LOW | LOW Duration | LOW→HIGH |
| --- | --- | --- | --- |
| BC | 200 ms | 2–5 ms | 200 ms |
| V3 | 100–200 ms | 2 ms | 10–200 ms |
| V4 | 20 ms | 2 ms | 20 ms |

#### Our Driver Match

Our `DSWS_2in13BV3` driver implementation most closely matches the **V3 variant** (SSD1680):
- Uses `$00`/`$61`/`$50` init pattern
- Uses `$10`/`$13` for data write
- Uses `$12` for refresh
- 104×212 resolution
- BUSY HIGH = idle

---

## Display 2: WaveShare 5.65" 7-Color ACeP (DSWS_5in65F)

Our driver's `DSWS_5in65F` display type. Only one hardware variant exists across all platforms.

---

### Example Set: EPD5IN65F (All Platforms)

**Files studied:** `Arduino/epd5in65f/`, `Arduino_R4/.../EPD_5in65f.*`, `STM32/.../EPD_5in65f.*`, `RaspberryPi_JetsonNano/c/.../EPD_5in65f.*`, `RaspberryPi_JetsonNano/python/.../epd5in65f.py`

#### Controller IC

**Not explicitly named** in any reference code. The register command set is consistent with WaveShare's ACeP (Advanced Color ePaper) proprietary controller family. This may be a custom-masked controller or a variant of the UC8159 (UltraChip). The command set does not match any publicly documented Solomon Systech or UltraChip controller exactly.

**Register set fingerprint:** `$00`/`$01`/`$03`/`$06`/`$30`/`$41`/`$50`/`$60`/`$61`/`$E3` — consistent with UC8159-family controllers.

#### Display Resolution

| Parameter | Value |
| --- | --- |
| Width | 600 pixels |
| Height | 448 pixels |
| Pixel format | 4 bpp — 2 pixels per byte (nibble-packed) |
| Frame buffer size | 134,400 bytes (600 × 448 / 2) |
| Bytes per row | 300 (600 / 2) |

#### Color Values (All Platforms Identical)

| Color | Value | Binary | Notes |
| --- | --- | --- | --- |
| BLACK | `$0` | `000` | Black ink |
| WHITE | `$1` | `001` | White (no ink) |
| GREEN | `$2` | `010` | Green ink |
| BLUE | `$3` | `011` | Blue ink |
| RED | `$4` | `100` | Red ink |
| YELLOW | `$5` | `101` | Yellow ink |
| ORANGE | `$6` | `110` | Orange ink |
| CLEAN | `$7` | `111` | Reserved — afterimage/erase waveform |

**Pixel packing:** Two 4-bit pixels per byte. Upper nibble (bits 7:4) = first pixel, lower nibble (bits 3:0) = second pixel. Packed as `(color1 << 4) | color0`.

#### Reset Sequence

| Platform | HIGH Duration | LOW Duration | HIGH Duration |
| --- | --- | --- | --- |
| Arduino / Arduino R4 / STM32 | 200 ms | 1 ms | 200 ms |
| RPi C | 200 ms | 2 ms | 200 ms |
| RPi Python | 200 ms | 2 ms | 200 ms (initial HIGH: 600 ms) |

**Python anomaly:** RPi Python uses a 600 ms initial HIGH duration before the LOW pulse.

#### Initialization Command Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | *(wait BusyHigh)* | — | — | Wait after reset |
| 2 | PANEL_SETTING | `$00` | `$EF, $08` | ACeP panel config |
| 3 | POWER_SETTING | `$01` | `$37, $00, $23, $23` | VGH/VGL, source voltages |
| 4 | POWER_OFF_SEQ | `$03` | `$00` | 1-frame power-off |
| 5 | BOOSTER_SOFT_START | `$06` | `$C7, $C7, $1D` | Booster phase timing |
| 6 | PLL_CONTROL | `$30` | `$3C` or `$39` | Frame rate (see note) |
| 7 | TEMP_SENSOR_ENABLE | `$41` | `$00` | Internal temp sensor |
| 8 | VCOM_DATA_INTERVAL | `$50` | `$37` | VCOM interval |
| 9 | TCON | `$60` | `$22` | TCON timing |
| 10 | RESOLUTION_SETTING | `$61` | `$02, $58, $01, $C0` | 600×448 (16-bit BE) |
| 11 | PWS | `$E3` | `$AA` | Power-saving |
| 12 | *(delay 100 ms)* | — | — | Settle |
| 13 | VCOM_DATA_INTERVAL | `$50` | `$37` | VCOM interval (repeated) |

**PLL_CONTROL platform difference:** RPi C uses `$39`, all others use `$3C`.

#### LUT Data

**None sent.** The ACeP controller uses internal OTP waveform tables. No external LUT loading is performed in any implementation.

#### Frame Buffer Write Sequence

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | RESOLUTION_SETTING | `$61` | `$02, $58, $01, $C0` | Must set resolution before data |
| 2 | DATA_START_XMIT_1 | `$10` | *(134,400 bytes)* | All pixel data, row by row |
| 3 | POWER_ON | `$04` | — | Power on |
| 4 | *(wait BusyHigh)* | — | — | Power-on complete |
| 5 | DISPLAY_REFRESH | `$12` | — | Trigger refresh |
| 6 | *(wait BusyHigh)* | — | — | Refresh complete (~35 sec) |
| 7 | POWER_OFF | `$02` | — | Power off |
| 8 | *(wait BusyLow)* | — | — | Power-off complete |
| 9 | *(delay 200–500 ms)* | — | — | Post-refresh settle |

**Critical requirement:** The RESOLUTION_SETTING (`$61`) command **must** be sent immediately before data transmission, or the display will fail silently.

**Power management:** The refresh cycle requires an explicit power-on (`$04`), refresh trigger (`$12`), and power-off (`$02`) sequence. All three commands are mandatory.

#### Busy Pin Protocol — DUAL-MODE!

This display uniquely uses **two different busy-wait states** depending on the operation:

| Wait Function | Waits For | Used After |
| --- | --- | --- |
| **BusyHigh()** | BUSY goes HIGH | Reset, Power-on (`$04`), Refresh (`$12`) |
| **BusyLow()** | BUSY goes LOW | Power-off (`$02`) |

```
BusyHigh: while(pin == LOW)  { poll; }   // Wait for HIGH
BusyLow:  while(pin == HIGH) { poll; }   // Wait for LOW
```

Comments in the code clarify:
- "If BUSYN=0 then waiting" → BusyHigh waits while LOW
- "If BUSYN=1 then waiting" → BusyLow waits while HIGH

This dual-polarity busy mechanism is unique among our supported displays.

#### Clear Screen Sequence

Same as display refresh, but all pixels set to WHITE:

```
Each byte = (WHITE << 4) | WHITE = $11
Send 134,400 bytes of $11
Power-on → Refresh → Power-off
Post-clear delay: 500 ms (longer than image display: 200 ms)
```

#### Sleep Sequence

```
delay(100 ms)
Command $07 (DEEP_SLEEP) — Data: $A5 (check code)
delay(100 ms)
RST pin = LOW                  (hold in reset)
```

Wake requires hardware reset (RST HIGH) followed by full re-initialization.

#### Platform-Specific Differences (5in65F)

| Feature | Arduino | Arduino R4 | STM32 | RPi C | RPi Python |
| --- | --- | --- | --- | --- | --- |
| Reset initial HIGH | 200 ms | 200 ms | 200 ms | 200 ms | **600 ms** |
| Reset LOW | 1 ms | 1 ms | 1 ms | 2 ms | 2 ms |
| PLL_CONTROL (`$30`) data | `$3C` | `$3C` | `$3C` | **`$39`** | `$3C` |
| Post-display delay | 200 ms | **500 ms** | **500 ms** | **500 ms** | **500 ms** |
| Post-clear delay | 200 ms | 500 ms | 500 ms | 500 ms | 500 ms |
| Post-sleep delay | 100 ms | 100 ms | 100 ms | 100 ms | **500–2000 ms** |
| SPI clock | 2 MHz | 2 MHz | HAL | HAL | spidev |

---

### 5.65" ACeP Synthesis — Cross-Platform Comparison

All platforms implement an essentially identical protocol with minor timing variations. The key findings are:

1. **Mandatory resolution setting before data** — `$61` must precede `$10`
2. **Three-step refresh** — Power-on → Refresh → Power-off is required every time
3. **Dual-polarity busy** — Different wait conditions for different operations
4. **No external LUT** — Controller uses internal OTP waveforms
5. **~35 second refresh** — Inherent to 7-color ACeP technology
6. **RPi C PLL difference** — `$39` vs `$3C` may indicate a newer firmware revision

---

## Display 3: WaveShare 3.70" B/W (DSWS_370BW)

Our driver's `DSWS_370BW` display type. This display supports both 1-bit monochrome and 4-level grayscale modes via different LUT tables.

---

### Example Set: EPD3IN7 (All Platforms)

**Files studied:** `Arduino/epd3in7/`, `Arduino_R4/.../EPD_3in7.*`, `STM32/.../EPD_3in7.*`, `RaspberryPi_JetsonNano/c/.../EPD_3in7.*`, `RaspberryPi_JetsonNano/python/.../epd3in7.py`

#### Controller IC

**SSD1677** (Solomon Systech) — Confirmed by:
- 16-bit X/Y addressing (not found in SSD1675)
- Commands `$46`/`$47` for auto-measure VCOM
- Command `$37` with 10-byte display options register
- 280-source × 480-gate organization
- 4-gray mode support via dual RAM planes (`$24`/`$26`)

#### Display Resolution

| Parameter | Value |
| --- | --- |
| Width (source) | 280 pixels |
| Height (gate) | 480 pixels |
| Monochrome buffer | 16,800 bytes (280 × 480 / 8) |
| 4-Gray buffer | 33,600 bytes (280 × 480 / 4, 2bpp) |
| Bytes per row (1bpp) | 35 (280 / 8) |
| RAM X range | 0–279 (16-bit addressing) |
| RAM Y range | 0–479 (16-bit addressing) |

#### Reset Sequence

| Platform | HIGH Duration | LOW Duration | HIGH Duration |
| --- | --- | --- | --- |
| Arduino (legacy) | 20 ms | 2 ms | 20 ms |
| Arduino R4 / STM32 | 300 ms | 3 ms | 300 ms |
| RPi C | 30 ms | *(implied)* | 30 ms |
| RPi Python | 200 ms | 5 ms | 200 ms |

**Significant variance** — Arduino legacy uses very short 20 ms, while Arduino R4/STM32 use 300 ms.

#### Initialization Command Sequence

All platforms send an identical command sequence (only timing varies):

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | SW_RESET | `$12` | — | Software reset |
| 2 | *(delay 300 ms)* | — | — | Post-reset settle |
| 3 | AUTO_MEASURE_VCOM_1 | `$46` | `$F7` | Auto-measure VCOM step 1 |
| 4 | *(wait for idle)* | — | — | |
| 5 | AUTO_MEASURE_VCOM_2 | `$47` | `$F7` | Auto-measure VCOM step 2 |
| 6 | *(wait for idle)* | — | — | |
| 7 | DRIVER_OUTPUT_CTRL | `$01` | `$DF, $01, $00` | Gate count = 479 (480−1) |
| 8 | GATE_VOLTAGE | `$03` | `$00` | Negative gate voltage |
| 9 | SOURCE_VOLTAGE | `$04` | `$41, $A8, $32` | VSH1, VSH2, VSL |
| 10 | DATA_ENTRY_MODE | `$11` | `$03` | X incr, Y incr |
| 11 | BORDER_WAVEFORM | `$3C` | `$00` or `$03` | See platform note |
| 12 | BOOSTER_SOFT_START | `$0C` | `$AE, $C7, $C3, $C0, $C0` | 5-byte booster config |
| 13 | TEMP_SENSOR | `$18` | `$80` | Internal temp sensor |
| 14 | WRITE_VCOM | `$2C` | `$44` | VCOM DC = −2.0V |
| 15 | DISPLAY_OPTIONS | `$37` | *(10 bytes, mode-dependent)* | See below |
| 16 | SET_RAM_X_ADDR | `$44` | `$00, $00, $17, $01` | X: 0 to 279 (16-bit) |
| 17 | SET_RAM_Y_ADDR | `$45` | `$00, $00, $DF, $01` | Y: 0 to 479 (16-bit) |
| 18 | DISPLAY_UPDATE_CTRL_2 | `$22` | `$CF` | Enable update sequence |

**Command `$37` Display Options — Mode-Dependent:**

| Mode | 10 Data Bytes |
| --- | --- |
| 4-Gray | `$00, $00, $00, $00, $00, $00, $00, $00, $00, $00` |
| 1-Gray | `$00, $FF, $FF, $FF, $FF, $4F, $FF, $FF, $FF, $FF` |

The `$4F` in byte 5 for 1-Gray mode likely controls the mode-switching circuitry. Setting all bytes to `$00` for 4-Gray mode disables mode switching.

**Border waveform platform difference:** Arduino legacy uses `$00`, all other platforms use `$03`.

#### Auto-Measure VCOM (Commands `$46`/`$47`)

These commands are **unique to the SSD1677** and not found in any other controller in our driver. They trigger the controller's internal VCOM auto-calibration circuit:

```
Command $46, Data $F7    → Start VCOM measurement phase 1
Wait for BUSY idle        → Measurement complete
Command $47, Data $F7    → Start VCOM measurement phase 2
Wait for BUSY idle        → Calibration stored
```

This auto-calibration is performed once during init, before setting any display parameters.

#### LUT Data — FOUR MODES (105 bytes each)

The SSD1677 supports four distinct waveform modes, each with its own 105-byte LUT:

**Mode 0: 4-Gray GC (Grayscale Clear — full quality 4-level grayscale)**

```
Offset 00: $2A $06 $15 $00 $00 $00 $00 $00 $00 $00
Offset 10: $28 $06 $14 $00 $00 $00 $00 $00 $00 $00
Offset 20: $20 $06 $10 $00 $00 $00 $00 $00 $00 $00
Offset 30: $14 $06 $28 $00 $00 $00 $00 $00 $00 $00
Offset 40: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 50: $00 $02 $02 $0A $00 $00 $00 $08 $08 $02
Offset 60: $00 $02 $02 $0A $00 $00 $00 $00 $00 $00
Offset 70: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 80: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 90: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 100: $22 $22 $22 $22 $22
```

**Mode 1: 1-Gray GC (Grayscale Clear — full quality monochrome)**

```
Offset 00: $2A $05 $00 $00 $00 $00 $00 $00 $00 $00
Offset 10: $05 $2A $00 $00 $00 $00 $00 $00 $00 $00
Offset 20: $2A $15 $00 $00 $00 $00 $00 $00 $00 $00
Offset 30: $05 $0A $00 $00 $00 $00 $00 $00 $00 $00
Offset 40: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 50: $00 $02 $03 $0A $00 $02 $06 $0A $05 $00
Offset 60: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 70: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 80: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 90: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 100: $22 $22 $22 $22 $22
```

**Mode 2: 1-Gray DU (Direct Update — fast monochrome)**

```
Offset 00: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 10: $01 $2A $00 $00 $00 $00 $00 $00 $00 $00
Offset 20: $0A $55 $00 $00 $00 $00 $00 $00 $00 $00
Offset 30: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 40: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 50: $00 $00 $05 $05 $00 $05 $03 $05 $05 $00
Offset 60: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 70: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 80: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 90: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 100: $22 $22 $22 $22 $22
```

**Mode 3: 1-Gray A2 (Fast partial refresh — lowest quality, fastest speed)**

```
Offset 00: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 10: $0A $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 20: $05 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 30: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 40: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 50: $00 $00 $03 $05 $00 $00 $00 $00 $00 $00
Offset 60: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 70: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 80: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 90: $00 $00 $00 $00 $00 $00 $00 $00 $00 $00
Offset 100: $22 $22 $22 $22 $22
```

**LUT Structure (all modes):**

| Byte Range | Size | Purpose |
| --- | --- | --- |
| 0–49 | 50 bytes | 5 waveform phase groups × 10 bytes (voltage level patterns) |
| 50–99 | 50 bytes | Phase timing parameters (TP values: pulse counts and durations) |
| 100–104 | 5 bytes | Frame rate / gate scan group settings (always `$22 $22 $22 $22 $22`) |

**LUT loading:** All modes use the same command:
```
Command $32 (WRITE_LUT_REGISTER)
Send 105 data bytes
```

The LUT is reloaded before each display update, allowing dynamic switching between modes.

#### 4-Gray Mode Details

The SSD1677 supports 4-level grayscale via dual RAM planes:

**Pixel encoding (2bpp, 4 pixels per byte):**

| Gray Level | Value | Description |
| --- | --- | --- |
| White | `$C0` | Bits = `11` |
| Light gray | `$80` | Bits = `10` |
| Dark gray | `$40` | Bits = `01` |
| Black | `$00` | Bits = `00` |

Each byte stores 4 pixels: bits [7:6] = pixel 0, bits [5:4] = pixel 1, bits [3:2] = pixel 2, bits [1:0] = pixel 3.

**4-Gray display sequence:**

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | *(unknown)* | `$49` | `$00` | Pre-4Gray command |
| 2 | SET_RAM_X_CTR | `$4E` | `$00` | X pointer = 0 |
| 3 | SET_RAM_Y_CTR | `$4F` | `$00, $00` | Y pointer = 0 |
| 4 | WRITE_RAM | `$24` | *(16,800 bytes)* | Primary gray plane |
| 5 | WRITE_RAM_ALT | `$26` | *(16,800 bytes)* | Alternate gray plane |
| 6 | WRITE_LUT | `$32` | *(105 bytes)* | Load `lut_4Gray_GC` |
| 7 | DISPLAY_UPDATE_CTRL_2 | `$22` | `$C7` | Enable 4-Gray update |
| 8 | MASTER_ACTIVATION | `$20` | — | Trigger refresh |
| 9 | *(wait for idle)* | — | — | |

**Note:** The `$C7` value for DISPLAY_UPDATE_CTRL_2 differs from `$CF` used during init.

#### 1-Gray Mode Frame Buffer Write

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | SET_RAM_X_CTR | `$4E` | `$00, $00` | X pointer = 0 (16-bit) |
| 2 | SET_RAM_Y_CTR | `$4F` | `$00, $00` | Y pointer = 0 (16-bit) |
| 3 | WRITE_RAM | `$24` | *(16,800 bytes)* | Monochrome frame data |
| 4 | WRITE_LUT | `$32` | *(105 bytes)* | Load mode-appropriate LUT |
| 5 | MASTER_ACTIVATION | `$20` | — | Trigger refresh |
| 6 | *(wait for idle)* | — | — | |

**LUT selection for 1-Gray:**
- Full quality: `lut_1Gray_GC` (Mode 1)
- Fast update: `lut_1Gray_DU` (Mode 2)
- Fastest/partial: `lut_1Gray_A2` (Mode 3)

#### Partial Refresh Support

**Yes — supported for 1-Gray mode only.**

| Step | Command | Hex | Data | Description |
| --- | --- | --- | --- | --- |
| 1 | SET_RAM_X_ADDR | `$44` | X_start_lo, X_start_hi, X_end_lo, X_end_hi | 16-bit X region |
| 2 | SET_RAM_Y_ADDR | `$45` | Y_start_lo, Y_start_hi, Y_end_lo, Y_end_hi | 16-bit Y region |
| 3 | SET_RAM_X_CTR | `$4E` | X_start_lo, X_start_hi | Set pointer |
| 4 | SET_RAM_Y_CTR | `$4F` | Y_start_lo, Y_start_hi | Set pointer |
| 5 | WRITE_RAM | `$24` | *(region bytes)* | Partial image data |
| 6 | WRITE_LUT | `$32` | *(105 bytes)* | Typically `lut_1Gray_A2` |
| 7 | MASTER_ACTIVATION | `$20` | — | Trigger refresh |
| 8 | *(wait for idle)* | — | — | |

**Constraints:** X_start must be byte-aligned (multiple of 8).

#### Busy Pin Protocol

- **Polarity:** HIGH = busy, LOW = idle
- **Wait:** `while(pin == HIGH) { poll; }` then `delay(200 ms)`
- **Named:** "ReadBusy_HIGH" in reference code — the name refers to the active state, not what it waits for

All platforms use an additional 200 ms settle delay after BUSY goes LOW, except RPi Python which polls at 10 ms intervals without extra settle.

#### Dithering Support (RPi C Only)

The Raspberry Pi C implementation includes a **hardware dithering** feature not found in any other platform:

| Command | Hex | Data | Description |
| --- | --- | --- | --- |
| DITHER_START | `$4D` | `$80, $00, $78, $00` (Sierra-Lite) or `$83, $00, $78, $00` (Floyd-Steinberg) | Enable dither engine |
| WRITE_DITHER_RAM | `$25` | *(4-bit source image)* | Load 16-color source data |
| DITHER_STOP | `$4D` | `$00, $00, $78, $00` | Disable dither engine |

The dithering engine converts a 4-bit (16-level) grayscale source image to 1-bit monochrome using the specified algorithm. This is a hardware feature of the SSD1677 that other platform implementations do not expose.

#### Sleep Sequence

```
Command $10 (DEEP_SLEEP) — Data: $03 (deep sleep mode 2)
```

Deep sleep mode 2 powers down analog circuits while retaining register settings. Wake requires hardware reset.

#### Platform-Specific Differences (3in7)

| Feature | Arduino | Arduino R4 | STM32 | RPi C | RPi Python |
| --- | --- | --- | --- | --- | --- |
| Reset timing (H/L/H) | 20/2/20 ms | 300/3/300 ms | 300/3/300 ms | 30/?/30 ms | 200/5/200 ms |
| Border waveform (`$3C`) | **`$00`** | `$03` | `$03` | `$03` | `$03` |
| LUT modes available | 3 | 4 | 4 | 4 | 3 |
| Dithering support | No | No | No | **Yes** | No |
| Post-busy settle | 200 ms | 200 ms | 200 ms | 200 ms | 10 ms/poll |
| BUSY polarity | HIGH=busy | HIGH=busy | HIGH=busy | HIGH=busy | HIGH=busy |

---

### E-paper Separate Program: 3in7_e-Paper_G Variant

**Path:** `E-paper_Separate_Program/3in7_e-Paper_G/`

This directory contains additional implementations targeting the same SSD1677 display, possibly for a "G" (grayscale-optimized) variant. The subdirectories include Arduino_R4, ESP32, Raspberry Pi, and STM32 implementations.

The "G" variant examples may focus more heavily on 4-Gray mode usage but target the same SSD1677 controller with identical command registers and LUT structure. The protocol is expected to be identical to the main EPD3IN7 examples.

---

### 3.70" B/W Synthesis — Cross-Platform Comparison

Key findings consolidated from all platforms:

1. **Controller confirmed: SSD1677** — The unique commands `$46`/`$47` (auto-measure VCOM), 16-bit addressing, and `$37` (10-byte display options) are SSD1677-specific
2. **Four LUT modes** — GC (full quality) and A2 (fast) for both 1-Gray and 4-Gray, all 105 bytes each
3. **LUT reloaded per update** — Unlike SSD1675 where LUT is loaded once at init, the SSD1677 reloads the LUT before every display refresh, enabling dynamic mode switching
4. **Auto-VCOM calibration** — `$46`/`$47` commands perform internal VCOM measurement during init; our P2 driver does not currently use these
5. **Hardware dithering** — SSD1677 has a built-in dithering engine (commands `$4D`/`$25`) only exposed in the RPi C implementation
6. **Dual RAM planes** — `$24` (primary) and `$26` (alternate) enable 4-Gray mode; for 1-Gray mode, only `$24` is used
7. **Border waveform discrepancy** — Arduino legacy uses `$00`, all others use `$03`; this affects the border appearance during refresh

---

## Master Summary Tables

### Controller IC Reference

| Display | Our Driver Enum | Controller IC | Evidence Source |
| --- | --- | --- | --- |
| 2.13" B/C (original) | *(closest to DSWS_2in13BV3)* | SSD1608 | Register pattern: `$06`/`$04`/`$00`/`$50`/`$61` |
| 2.13" B V3 (revised) | **DSWS_2in13BV3** | **SSD1680** | Init order change; requires `$71` GET_STATUS |
| 2.13" B V4 (latest) | *(not our display)* | SSD1681 | SSD1675-style registers: `$01`/`$11`/`$44`/`$24`/`$20` |
| 5.65" 7-Color | **DSWS_5in65F** | UC8159 (probable) | ACeP register set; no public datasheet |
| 3.70" B/W | **DSWS_370BW** | **SSD1677** | `$46`/`$47` auto-VCOM; 16-bit addressing; `$37` options |

### Busy Pin Polarity by Display

| Display | Busy State | Idle State | Special Notes |
| --- | --- | --- | --- |
| 2.13" BC | LOW | HIGH | Standard |
| 2.13" V3 | LOW | HIGH | Must send `$71` before reading |
| 2.13" V4 | **HIGH** | **LOW** | Inverted from V3/BC! |
| 5.65" 7-Color | Dual-mode | Dual-mode | BusyHigh after refresh, BusyLow after power-off |
| 3.70" B/W | HIGH | LOW | + 200 ms settle delay |

### LUT Requirements

| Display | LUT Source | LUT Size | Modes | Loaded When |
| --- | --- | --- | --- | --- |
| 2.13" BC | Internal OTP | — | 1 | Never (factory) |
| 2.13" V3 | Internal OTP | — | 1 | Never (factory) |
| 2.13" V4 | Internal | — | 1 | Never (factory) |
| 5.65" 7-Color | Internal OTP | — | 1 | Never (factory) |
| 3.70" B/W | **External** | **105 bytes** | **4** | **Per display update** |

### Data Write Commands

| Display | Black/BW Data | Red/Alt Data | Refresh Trigger |
| --- | --- | --- | --- |
| 2.13" BC | `$10` | `$13` | `$12` |
| 2.13" V3 | `$10` | `$13` | `$12` |
| 2.13" V4 | `$24` | `$26` | `$20` |
| 5.65" 7-Color | `$10` (all colors) | — | `$04` → `$12` → `$02` |
| 3.70" B/W | `$24` | `$26` (4-Gray) | `$20` |

### Reset Timing Ranges (Across All Platforms)

| Display | HIGH→LOW | LOW Duration | LOW→HIGH |
| --- | --- | --- | --- |
| 2.13" BC | 200 ms | 2–5 ms | 200 ms |
| 2.13" V3 | 100–200 ms | 2 ms | 10–200 ms |
| 2.13" V4 | 20 ms | 2 ms | 20 ms |
| 5.65" 7-Color | 200–600 ms | 1–2 ms | 200 ms |
| 3.70" B/W | 20–300 ms | 2–5 ms | 20–300 ms |

---

## Observations for P2 Driver Development

1. **Our DSWS_2in13BV3 implementation matches V3 (SSD1680)** — If users have V4 hardware (SSD1681), the driver will not work due to different resolution (122×250 vs 104×212), different commands (`$24`/`$26` vs `$10`/`$13`), and inverted busy polarity.

2. **The 5.65" busy mechanism is more nuanced than our driver implements** — The reference code uses BusyHigh for refresh and BusyLow for power-off as two distinct operations.

3. **The 3.70" SSD1677 has features our driver doesn't use:**
   - Auto-VCOM calibration (`$46`/`$47`)
   - 4-Gray mode with dual RAM planes
   - DU (Direct Update) fast refresh mode
   - A2 partial refresh mode
   - Hardware dithering (`$4D`/`$25`)

4. **LUT data in our driver should be verified against these reference values** — Especially the 1-Gray GC LUT (Mode 1) which is the primary mode we use.

5. **The border waveform (`$3C`) value `$03` is used by the majority of platforms** — our driver should verify it uses this value rather than `$00`.

---

Additional pages:

- [Display Specifications](./eInk-display-specs.md)
- [Technical Protocol Reference (from our driver)](./eInk-protocol-reference.md)
- [Display Images and Hardware Docs](./README.md)
- [eInk Touch Wiring Guide](./eInk-touch.md)
- [Display Orientation Guide](./Orientation.md)
- [Top README](https://github.com/ironsheep/P2-Click-eINK) — Return to the top-level README

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
