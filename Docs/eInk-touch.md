# P2 driver for Click adapter driving the Good Display eInk 3.70" e-paper w/Touch  

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)


## eInk 3.70" display (240x416 pixels) w/Touch

If you wish to use the eInk Touch display with your P2 Evaluation board, P2 Edge Module Breadboard, or the P2 Edge Mini Breakout Board then you'll need:

the Eval adapter board [P2 Eval to MikroBUS Click Adapter](https://www.parallax.com/product/p2-to-mikrobus-click-adapter/)

Additionally, you can buy the eInk displays from [Good Display](https://www.good-display.com/):

- [E-Ink Touch Screen for 3.7 inch High Transparency E-Paper Display GDEY037T03-T02](https://www.good-display.com/product/474.html) - display with touch overlay

<p align="center">
  <img src="../Images/GDEY037T03-T02.png" width="400"></br>
  <caption><B>Display w/Touch only</B></caption>
</p>

- or you can select the following at checkout:<br> **SKU "GDEY037T03-T02 with STM32 adapter board"** - adds adapter HAT board

<p align="center">
  <img src="../Images/GDEY037T03-T02-wHAT.png" width="400"></br>
  <caption><B>Display w/Touch, cables, and HAT board</B></caption>
</p>



## Table of Contents

On this Page:

- [The eInk Display](#features)
- [Wiring the Display](#how-to-contribute)

Additional pages:

- [Driver Public Interface](./isp_eInk_click.txt) - shows full driver annotated API 
- [Start your project using this object](DEVELOP.md) - Walks thru configuration and setup of your own project using this object
- [Configure Display Orientation](../Docs/Orientation.md) - set driver to match your hardare set up
- [Create bitmaps for display on your eInk device](./C-src)
- [See images of all supported displays working!](./Docs) There are a small number of .PDFs in the [Docs](../Docs) directory providing  detailed information on the display and controller chips



## Connecting the eInk display w/Touch to the P2

When using the GDEY037T03-T02 eInk display from Good Displays you do NOT use the **Mikroe eInk adapter Click board™**, instead, you wire the HAT adapter directly to the P2 Eval Click Adapter. This is due to the support electronics being different from that on the eInk Click board.  This also allows us to route the touch signals to the same adapter board so the entire display with touch consumes just the P2 Eval. Header pair.

Here's the signal mapping:

<p align="center">
  <img src="../Images/p2-click-adapter.png" width="300"></br>
  <caption><B>Mapping the eInk signals and the Touch signals to the Click Adapter</B></caption>
</p>

**NOTE:** we connect our **eInk-display** to the SPI-signals-side of the adapter ("eInk" on left) and the connect **Touch** to the I2C-signals-side of the adapter ("touch" on the right.)

### Connecting the HAT eInk display signals

For the display side, we connect the signals from HAT Header P6 to our P2 Eval Click Adapter (on the SPI signals side.) 

<p align="center">
  <img src="../Images/e-paper-pins.jpg" width="300"></br>
  <caption><B>Authors' eInk display connections from HAT - Header P6</B></caption>
</p>

Detailed list of the iInk connections:

| Signal Name | HAT Pin | Wire Color | Adapter Pin 
| --- | --- | ---  | --- 
| | ** **eInk Signals**  **| 
| 3.3v | P6-8  | Red | +3.3v 
| GND | P6-7 | Black | GND 
| SDI | P6-6 | Green | MOSI 
| SCLK | P6-5 | Yellow | SCK 
| CS | P6-4 | Orange | CS 
| D/C | P6-3  | Blue | MISO 
| RES | P6-2 | Violet | RST 
| BUSY | P6-1 | White | AN 


### Connecting the HAT Touch signals

For the touch side, we connect the signals from HAT Header P8 to our P2 Eval Click Adapter (on the I2C signals side.) 

<p align="center">
  <img src="../Images/touch-pins.jpg" width="300"></br>
  <caption><B>Authors' Touch connecctions from HAT Header P8</B></caption>
</p>

Detailed list of the Touch connections:

| Signal Name | HAT Pin | Wire Color | Adapter Pin 
| --- | --- | ---  | --- 
| | ** **Touch Signals** ** | 
| GND | P8-1 | Brown | GND 
| INT | P8-2 - PB14 - A0 INT | Blue | INT 
| RST | P8-3 - PB12 - A3 RST | Green | PWM 
| SCL | P8-4 - PB10 - A4 SCL | Orange | SCL 
| SDA | P8-5 - PE10 - A5 SDA | Yellow | SDA 

**NOTE:** There are three female 8-pin headers on the HAT board labeled P6, P7, and P8. P6 provides eInk display signals, while P8 provides the Touch signals.

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

### [Copyright](copyright) | [License](LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://img.shields.io/badge/License-MIT-yellow.svg

