# P2 eInk Driver source code
The source code of the P2 driver for the Mikroe eInk adapter Click board™

![Project Maintenance][maintenance-shield]

[![License][license-shield]](LICENSE)


## The hardware

These source files demonstrate the use of the eInk display as shipped from Parallax.com. 
<p align="center">
  <img src="../Images/eink_213.jpg" width="300">
</p>

## The Source Code

The following files comprise our demo and driver source code:

| Filename | Purpose | Description |
| --- | --- | --- |
| **Driver FILEs**
| [isp\_eInk_click.spin2](isp_eInk_click.spin2) | DRIVER | the eInk Driver
| [isp\_eInk_fonts.spin2](isp_eInk_fonts.spin2) | DRIVER | the eInk FONTs for driver
| [jm\_ez_spi.spin2](jm_ez_spi.spin2) | DRIVER | SPI smart-pin comms
| **Demo FILEs**
| [demo\_eInk_click.spin2](demo_eInk_click.spin2) | demo | DEMO top-level file
| [isp\_eInk\_demo_images.spin2](isp_eInk_demo_images.spin2) | demo | images shown by demo
| [isp\_image_coffee.spin2](isp_image_coffee.spin2) | demo | demo and sample converted bitmap


Additional pages:

- [Top README](https://github.com/ironsheep/P2-Click-eInk) - Return to the top-level README for this repository



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

Copyright © 2022 Iron Sheep Productions, LLC. All rights reserved.

Licensed under the MIT License.

Follow these links for more information:

### [Copyright](../copyright) | [License](../LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://camo.githubusercontent.com/bc04f96d911ea5f6e3b00e44fc0731ea74c8e1e9/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f6c6963656e73652f69616e74726963682f746578742d646976696465722d726f772e7376673f7374796c653d666f722d7468652d6261646765
