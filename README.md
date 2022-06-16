# P2 driver for Mikroe eInk Click
Driving the Mikroe eInk Click with our P2

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)


## eInk CLICK board w/2.13" display

[eINK click](https://www.mikroe.com/eink-click) is an adapter Click board™ from [Mikroe.com](https://www.mikroe.com/), used to interface a [compatible eINK display](https://www.mikroe.com/accessories/e-paper-displays) with the host MCU. The most distinctive feature of the eINK displays is their very low power consumption and the **ability to retain the information, even after disconnecting from the power source**. The power is consumed only when the display content is changing. 

[The Click board™ as delivered from Parallax.com](https://www.parallax.com/product/eink-click-e-paper-bundle-2/) comes shipped with the 250x122 eINK display, driven by the integrated SSD1606 controller and it can display 4 shades: black, dark gray, light gray and white. The same type of display is used on the popular Kindle E-readers. 

<p align="center">
  <img src="Images/eink_213.jpg" width="300">
</p>

## P2 Driver for the eInk Click Module

This project provides a working driver for the P2 developed in spin2.  It started as a port of the C example code for the 2.13" eInk display which Parallax ships.  It has been extended to add capabilities to create more creative displays. 

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
- eInk colors:
  - Black, light Gray, dark Gray, and White

To this, we added these additional capabilities:

- Set background color
- Set region background color
- Show bitmap in region as color w/rotation
- Draw line, box, circle
- Support for 1.54", 2.00" (older), 2.13" and 2.90" eInk displays

## Current status

Latest Changes:

```
11 June 2022
- Placing rotated [0, 90, 180, and 270 degr] bitmap now working
10 June 2022
- Region Fill working
- Placing bitmap onto screen working (missing 180 adn 270 rotations yet)
- General cleanup ensuring all the eInk display sized from Mikroe should work (still need to test them)
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
- Only 2.13" tested and working so far...
  - seems to be monochrome, not 4 color
  - seems to respond as 61x250 not 122x250 trying to figure this out 
- We still want to support all display orientations (only two are supported currently.)
- We want to test against the click supported displays [1.54", 2.00", 2.13" and 2.90"]
```


## Table of Contents

On this Page:

- [Driver Features](#features)
- [How to contribute](#how-to-contribute)

Additional pages:

- [Start your project using this object](DEVELOP.md) - Walks thru configuration and setup of your own project using this object
- [Create bitmaps for display on your eInk device](./C-src)
- There are a small number of .PDFs in the [Docs](./Docs) directory providing  detailed information on the display and controller chips

## How to Contribute

This is a project supporting our P2 Development Community. Please feel free to contribute to this project. You can contribute in the following ways:

- File **Feature Requests** or **Issues** (describing things you are seeing while using our code) at the [Project Issue Tracking Page](hhttps://github.com/ironsheep/P2-Click-UWB/issues)
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

Copyright © 2022 Iron Sheep Productions, LLC. All rights reserved.

Licensed under the MIT License.

Follow these links for more information:

### [Copyright](copyright) | [License](LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://camo.githubusercontent.com/bc04f96d911ea5f6e3b00e44fc0731ea74c8e1e9/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f6c6963656e73652f69616e74726963682f746578742d646976696465722d726f772e7376673f7374796c653d666f722d7468652d6261646765
