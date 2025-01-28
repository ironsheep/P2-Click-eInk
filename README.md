# P2 driver for Mikroe eINK adapter Click board™
Driving the Mikroe eINK Click with our P2

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)


## eINK CLICK board w/2.13" display (122x250 pixels)

[eINK click](https://www.mikroe.com/eINK-click) is an adapter Click board™ from [Mikroe.com](https://www.mikroe.com/), used to interface a [compatible eINK display](https://www.mikroe.com/accessories/e-paper-displays) with the host MCU. The most distinctive feature of the eINK displays is their very low power consumption and the **ability to retain the information, even after disconnecting from the power source**. The power is consumed only when the display content is changing. 

[The Click board™ as delivered from Parallax.com](https://www.parallax.com/product/eINK-click-e-paper-bundle-2/) comes shipped with the 250x122 eINK display, driven by the integrated SSD1606 controller and it can display 4 shades: black, dark gray, light gray and white. The same type of display is used on the popular Kindle E-readers. 

<p align="center">
  <img src="./Images/eink_213.jpg" width="300">
</p>

If you wish to use the click module with your P2 Evaluation board, P2 Edge Module Breadboard, or the P2 Edge Mini Breakout Board then you'll need:

- the Eval adapter board [P2 Eval to MikroBUS Click Adapter](https://www.parallax.com/product/p2-to-mikrobus-click-adapter/)

Additionally, you can buy the eINK displays and the click module separately from Parallax:

- [eINK Click](https://www.parallax.com/product/eink-click/) adapter
- [122 x 250 2.13 inch E-Paper Display for eINK Click](https://www.parallax.com/product/122-x-250-2-13-inch-e-paper-display-for-eink-click/)
- [200 x 200 1.54 inch E-Paper Display for eINK Click](https://www.parallax.com/product/200-x-200-1-54-inch-e-paper-display-for-eink-click/)
- [296 x 128 2.9 inch E-Paper Display for eINK Click](https://www.parallax.com/product/296-x-128-2-9-inch-e-paper-display-for-eink-click/)



## P2 Driver for the eINK Click Module

This project provides a working driver for the P2 developed in spin2.  It started as a port of the C example code for the 2.13" eINK display which Parallax ships.  It has been extended to add capabilities to create more creative displays. 

### Features

Initial ported capabilties:

- Show fullscreen bitmap
- Show text on screen using built-in fonts, w/orientations, and in 4 colors
- Fonts:
  - Tahoma [6, 7, 8, 10, 14, 16, 18pt]
  - Exo2 Condensed [10x16, 15x23, 21x32]
  - Roboto Mono [11x23]
- Font orientations:
  - Vertical, Vertical-Column, Horizontal
- eINK colors:
  - Black, and White *(2.00" display might be Black, Dark Grey, Light Grey and White)*

To this, we added these additional capabilities:

- Set background color
- Set region background color
- Show bitmap in region as color w/rotation
- Draw line, box, circle
- Support for 1.54", 2.00" (older), 2.13" and 2.90" eINK displays

## Current status

Latest Changes:

```
27 Jan 2025
- Added support to WaveShare 2.13" R/B/W eINK display
20 June 2022
- Completed 2.00" EP-EPA20-A (fm Mouser) device support
15 June 2022
- Updated demos to now produce P2 Logo'd display screen specific to device
- Update images in repo documents to show P2 Logo'd eInk displays
- Ver 0.9.1 Release
14 June 2022
- Repaired 1.54" device handling
- Now 1.54", 2.13 " and 2.90" displays are all working well
13 June 2022
- Both 2.13 " and 2.90" displays are now working well
- 1.54" is not quite happy yet...
- finished validation and final cleanup of "place image into region"
  - all images now rotated correctly and aligned properly within rotated field
12 June 2022
- Fixed 2.13" display logic 
- Now is full 122x250 pixels
- Is only displaying black and white (like it should have been!)
11 June 2022
- Placing rotated [0, 90, 180, and 270 degr] bitmap now working
10 June 2022
- Region Fill working
- Placing bitmap onto screen working (missing 180 adn 270 rotations yet)
- General cleanup ensuring all the eINK display sized from Mikroe should work (still need to test them)
09 June 2022 
- Font orientations now correct
- Font sizes now correct for rectangular pixels, sigh.
- Line drawing working
- Box drawing working (rectangular outlines)
- Working on region fill
- Working on placing overlay bitmaps
08 June 2022 
- fixing font orientations
07 June 2022 
- Display 2.13" is working, full screen bitmaps work
- font orientation is not correct
05 June 2022 
- Project Started
```

## Known Issues

Things we know about that still need attention:

```
- We still want to support all display orientations (only two are supported currently.)
  - Landscape (connector to left) and Portrait (connector at bottom)
- Want to add utility methods like: 
  - Calculate length of string in given font
  - Calculate offset to center string of given font in region
  - Invert/remap colors in bitmap (black to white, white to black, etc.)
```


## Table of Contents

On this Page:

- [Driver Features](#features)
- [How to contribute](#how-to-contribute)

Additional pages:

- [Start your project using this object](DEVELOP.md) - Walks thru configuration and setup of your own project using this object
- [Create bitmaps for display on your eINK device](./C-src)
- [See images of all supported displays working!](./Docs) There are a small number of .PDFs in the [Docs](./Docs) directory providing  detailed information on the display and controller chips

## How to Contribute

This is a project supporting our P2 Development Community. Please feel free to contribute to this project. You can contribute in the following ways:

- File **Feature Requests** or **Issues** (describing things you are seeing while using our code) at the [Project Issue Tracking Page](https://github.com/ironsheep/P2-Click-UWB/issues)
- Fork this repo and then add your code to it. Finally, create a Pull Request to contribute your code back to this repository for inclusion with the projects code. See [CONTRIBUTING](CONTRIBUTING.md)

---

> If you like my work and/or this has helped you in some way then feel free to help me out for a couple of :coffee:'s or :pizza: slices!
>
> [![coffee](https://www.buymeacoffee.com/assets/img/custom_images/black_img.png)](https://www.buymeacoffee.com/ironsheep) &nbsp;&nbsp; -OR- &nbsp;&nbsp; [![Patreon](./Images/patreon.png)](https://www.patreon.com/IronSheep?fan_landing=true)[Patreon.com/IronSheep](https://www.patreon.com/IronSheep?fan_landing=true)

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

