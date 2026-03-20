# P2 eINK Driver - Supported Display Technical Specifications
Detailed technical specifications for each eINK display supported by this driver.

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)

---

## 1.54" eINK Display (DS_eink154)

| Specification | Value |
| --- | --- |
| **Physical Size** | 1.54" diagonal (27.6mm × 27.6mm) |
| **Resolution** | 200 × 200 pixels |
| **Colors** | 2 — Black, White |
| **Bits Per Pixel** | 1 bpp (8 pixels/byte) |
| **Controller** | SSD1608 (Solomon Systech) |
| **Frame Buffer** | 5,000 bytes |
| **SRAM Width** | 200 pixels |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [Parallax.com](https://www.parallax.com/product/200-x-200-1-54-inch-e-paper-display-for-eink-click/) |

---

## 2.00" eINK Grayscale Display (DS_eink200gs)

| Specification | Value |
| --- | --- |
| **Physical Size** | 2.00" diagonal |
| **Resolution** | 72 × 172 pixels |
| **Colors** | 4 — Black, Dark Grey, Light Grey, White |
| **Bits Per Pixel** | 2 bpp (4 pixels/byte) |
| **Controller** | SSD1606 (Solomon Systech) |
| **Frame Buffer** | 3,096 bytes |
| **SRAM Width** | 72 pixels |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [Mouser.com](https://www.mouser.com/ProductDetail/Display-Visions/EA-EPA20-A?qs=EU6FO9ffTwdcD%252BcseurWVQ%3D%3D) — Display Visions EA-EPA20-A |

**Notes:** This display has an inverted Y-axis orientation which the driver compensates for internally. Older display model.

---

## 2.13" eINK B/W Display (DS_eink213)

| Specification | Value |
| --- | --- |
| **Physical Size** | 2.13" diagonal (2.37mm × 48.44mm active) |
| **Resolution** | 122 × 250 pixels (visible) |
| **Colors** | 2 — Black, White |
| **Bits Per Pixel** | 1 bpp (8 pixels/byte) |
| **Controller** | SSD1675 (Solomon Systech) |
| **Frame Buffer** | 4,000 bytes |
| **SRAM Width** | 128 pixels (122 visible + 6 padding) |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [Parallax.com](https://www.parallax.com/product/122-x-250-2-13-inch-e-paper-display-for-eink-click/) or as [eINK Click bundle](https://www.parallax.com/product/eink-click-e-paper-bundle-2/) |

**Notes:** Each row has 6 padding bits (128 source channels, 122 visible). This is the display included in the Parallax eINK Click bundle.

---

## 2.90" eINK B/W Display (DS_eink290)

| Specification | Value |
| --- | --- |
| **Physical Size** | 2.90" diagonal (29.06mm × 66.90mm active) |
| **Resolution** | 128 × 296 pixels |
| **Colors** | 2 — Black, White |
| **Bits Per Pixel** | 1 bpp (8 pixels/byte) |
| **Controller** | SSD1675 (Solomon Systech) |
| **Frame Buffer** | 4,736 bytes |
| **SRAM Width** | 128 pixels |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [Parallax.com](https://www.parallax.com/product/296-x-128-2-9-inch-e-paper-display-for-eink-click/) |

---

## 2.13" WaveShare 3-Color R/B/W Display (DSWS_2in13BV3)

| Specification | Value |
| --- | --- |
| **Physical Size** | 2.13" diagonal (29.06mm × 66.90mm active) |
| **Resolution** | 104 × 212 pixels |
| **Colors** | 3 — Black, White, Red |
| **Bits Per Pixel** | 1 bpp × 2 planes (black plane + red plane) |
| **Controller** | SSD1680 variant |
| **Frame Buffer** | 5,512 bytes (2 × 2,756 bytes) |
| **SRAM Width** | 104 pixels |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [WaveShare.com](https://www.waveshare.com/product/displays/e-paper/epaper-3/2.13inch-e-paper-hat-b.htm) or [Amazon](https://www.amazon.com/waveshare-2-13inch-HAT-Resolution-Raspberry/dp/B07Q22WDB9) |

**Notes:** Uses dual-plane frame buffer format — a black/white plane followed by a red/white plane. The FPC ribbon detaches from the WaveShare HAT and plugs directly into the Mikroe eINK Click module.

---

## 2.13" Adafruit Quad-Color B/R/Y/W Display (DSAF_213BRYW)

| Specification | Value |
| --- | --- |
| **Physical Size** | 2.13" diagonal |
| **Resolution** | 122 × 250 pixels (visible) |
| **Colors** | 4 — Black, White, Red, Yellow |
| **Bits Per Pixel** | 2 bpp (4 pixels/byte) |
| **Controller** | JD79661AA (JADARD) |
| **Frame Buffer** | 11,000 bytes |
| **SRAM Width** | 176 pixels (122 visible, offset 8 pixels from left) |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | [Adafruit.com](https://www.adafruit.com/product/6373) |

**Notes:** The JD79661AA's internal SRAM is 176 pixels wide regardless of resolution setting. The 122 visible panel pixels are centered within the SRAM starting at column 8. The driver applies an automatic +8 pixel horizontal offset. Panel LUT color mapping: Gray0=Black, Gray1=White, Gray2=Yellow, Gray3=Red. Requires 10ms reset timing (longer than standard displays).

---

## 3.70" Good Display B/W with Touch (DSGD_370Tch)

| Specification | Value |
| --- | --- |
| **Physical Size** | 3.70" diagonal (53.00mm × 92.99mm active) |
| **Resolution** | 240 × 416 pixels |
| **Colors** | 2 — Black, White |
| **Bits Per Pixel** | 1 bpp (8 pixels/byte) |
| **Controller** | UC8253 (display) + GT911 (touch) |
| **Frame Buffer** | 12,480 bytes (+ 12,480 bytes prior frame buffer) |
| **SRAM Width** | 240 pixels |
| **Interface** | SPI (display) + I2C (touch) via P2 Eval Click Adapter |
| **Source** | [Good Display](https://www.good-display.com/product/474.html) — GDEY037T03-T02 |

**Notes:** Does NOT use the Mikroe eINK Click module. The display and touch overlay connect directly to the P2 Eval Click Adapter via wire harness from the HAT adapter board. The display signals use the SPI side of the adapter, and the touch signals use the I2C side. Maintains a prior-frame buffer for differential updates. Supports partial refresh for fast screen updates. See [eInk Touch wiring guide](eInk-touch.md) for connection details.

---

## 5.65" WaveShare 7-Color ACeP Display (DSWS_5in65F)

| Specification | Value |
| --- | --- |
| **Physical Size** | 5.65" diagonal (53.00mm × 92.99mm active) |
| **Resolution** | 600 × 448 pixels |
| **Colors** | 7 — Black, White, Blue, Orange, Yellow, Green, Red |
| **Bits Per Pixel** | 4 bpp (2 pixels/byte, 3-bit color + 1-bit clean) |
| **Controller** | UC8159 |
| **Frame Buffer** | 134,400 bytes |
| **SRAM Width** | 600 pixels |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | WaveShare.com |

**Notes:** This is an ACeP (Advanced Color ePaper) display with the largest frame buffer of any supported display. The refresh cycle is slow (~35 seconds). The driver includes a pre-clean phase before each update to prevent ghosting. Each pixel uses a 3-bit color value packed into 4-bit nibbles (2 pixels per byte); value $07 is reserved for the CLEAN waveform.

---

## 3.70" WaveShare B/W Display (DSWS_370BW)

| Specification | Value |
| --- | --- |
| **Physical Size** | 3.70" diagonal |
| **Resolution** | 480 × 280 pixels (480 wide × 280 tall physical) |
| **Colors** | 2 — Black, White |
| **Bits Per Pixel** | 1 bpp (8 pixels/byte) |
| **Controller** | SSD1677 (Solomon Systech) |
| **Frame Buffer** | 16,800 bytes |
| **SRAM Organization** | 280-source (35 bytes/row) × 480-gate |
| **Interface** | SPI via Mikroe eINK Click adapter |
| **Source** | WaveShare.com |

**Notes:** The SSD1677 uses 16-bit addressing for both X and Y coordinates (unlike the 8-bit addressing of the SSD1675). Supports two LUT modes: GC (Grayscale Clear) for full-quality refresh and A2 for fast partial-quality updates. The LUT is reloaded before each display update, allowing dynamic mode switching. Reset timing requires longer HIGH-state holds (200 ms) with a shorter LOW pulse (5 ms).

---

## Display Comparison Summary

| Display | Size | Resolution | Colors | BPP | Buffer | Controller |
| --- | --- | --- | --- | --- | --- | --- |
| DS_eink154 | 1.54" | 200×200 | 2 (B/W) | 1 | 5,000 B | SSD1608 |
| DS_eink200gs | 2.00" | 72×172 | 4 (grays) | 2 | 3,096 B | SSD1606 |
| DS_eink213 | 2.13" | 122×250 | 2 (B/W) | 1 | 4,000 B | SSD1675 |
| DS_eink290 | 2.90" | 128×296 | 2 (B/W) | 1 | 4,736 B | SSD1675 |
| DSWS_2in13BV3 | 2.13" | 104×212 | 3 (B/W/R) | 1×2 | 5,512 B | SSD1680 |
| DSAF_213BRYW | 2.13" | 122×250 | 4 (B/R/Y/W) | 2 | 11,000 B | JD79661AA |
| DSGD_370Tch | 3.70" | 240×416 | 2 (B/W) | 1 | 12,480 B | UC8253 |
| DSWS_5in65F | 5.65" | 600×448 | 7 (color) | 4 | 134,400 B | UC8159 |
| DSWS_370BW | 3.70" | 480×280 | 2 (B/W) | 1 | 16,800 B | SSD1677 |

---

Additional pages:

- [Technical Protocol Reference](./eInk-protocol-reference.md) — SPI commands, init sequences, LUTs, memory maps
- [Display Images and Hardware Docs](./README.md)
- [eInk Touch Wiring Guide](./eInk-touch.md)
- [Display Orientation Guide](./Orientation.md)
- [Top README](https://github.com/ironsheep/P2-Click-eINK) - Return to the top-level README for this repository

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
