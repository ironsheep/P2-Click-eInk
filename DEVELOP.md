# Developing a P2 Application interacting with the new Mikroe eInk Click P2 Driver 

Add a BLDC drive control subsystem to your own project!

![Project Maintenance][maintenance-shield]
[![License][license-shield]](LICENSE)

## Table of Contents

On this Page:

*Follow these steps to add Mikroe eInk Click P2 driver to your project:*

- [Download the latest release .zip file]( #download-the-latest-release-demo-archive-setzip-file) - get project files
- [Include project object(s) in your top-object-file]( #include-project-objects-in-your-top-object-file) - adjust your top-level file
- [Add your own display code]( #and-youre-off--add-your-own-motor-control-code) 
Additional pages:

- [Main Page](https://github.com/ironsheep/P2-Click-eInk) - Return to the top of this repo

---

## Download the latest release file

Go to the project [Releases page](https://github.com/ironsheep/P2-Click-eInk/releases) expand the **Assets** heading to see the demo-archive-set.zip file link. Click on it to download the .zip file. Unpack it and move the files into your project. 

Lastly you'll start the objects and then add your writte to display code.


## Include Project Objects in your top-object-file

You now need to select objects based on if you are a one-wheel or two-wheel confuration.


### Using the P2 Click driver

- isp\_eInk_click.spin2 - the new click driver code. It also includes the fonts

You simply include them with something like:

```script
OBJ { Objects Used by this Object }

    eInkDisplay : "isp_eInk_click"	' include the driver, fonts
```

#### Start the eInk Driver

Starting eInk driver object in Spin2 is also pretty simple:

```script

CON { click module io pins }

    ' our Click module is on 16-31!
    EINK_CLICK_PINGROUP = eInkDisplay.PINS_P16_P31

PUB main() | ok
'' DEMO let's see what our radio hears!

    ' tell object where the module is connected
    ok := eInkDisplay.start(EINK_CLICK_PINGROUP)
    if not ok
        debug("* SPI FAILED to start")
    else
        debug("* SPI started OK")

  ... and do your app stuff from here on ...
  
        eInkDisplay.stop()   ' if you wish to shutdown COGs and release motor pins
   
```


### And you're off!  Add your own display code

You are now at the `... and do your app stuff from here on ...` section of this page.
From here on, just use any of the Public Methods found in the interface description.  Here's a quick (maybe partial) summary:

| Method Signature | Purpose |
| --- | --- |
| PUB  start(eDisplayType, eClickBasePin) : ok | Specify the eval board connect location for the EINK click and start SPI</br>** eDisplayType** is [DS\_eink154, DS\_eink200, DS\_eink213, or DS\_eink290]</br>**eClickBasePin** is [PINS\_P0\_P15, PINS\_P16\_P31, or PINS\_P32\_P47]
| PUB  stop() | Stop the EINK Click SPI Object
| PUB  resetDevice() | Reset the EINK device
| PUB displaySize() : widthInPix, heightInPix | Return display size in pixels</br>**NOTE**: only valid is device is successfully started!</br>(ensure start() is called, first and that it is successful!)
PUB displayType() : eDisplayType| Return display size in pixels</br>**NOTE**: only valid is device is successfully started!</br>(ensure start() is called, first and that it is successful!)</br>** eDisplayType** is [DS\_eink154, DS\_eink200, DS\_eink213, or DS\_eink290]
| **WRITE DIRECTLY TO DISPLAY**
| PUB  fillScreen(color) | Fill Display with {color}
| PUB  displayImage(pImageBffr) | Write image bitmap to Display
| PUB  displayTextAtXY(X, Y, pText) | Write zstr to Display at device row, column
| **WRITE TO BUFFER** | (**NOTE:** when all buffer writes done call `flushToDisplay()` to commit)
| PUB  setFont(orientation, color, eFont) | Set font configuration
| PUB  setBackground(color) | Fill Frame Buffer with {color}
| PUB  setTextAtXY(X, Y, pText) | Write zstr to Frame Buffer at device row, column
| PUB  fillRegionAt(X, Y, color, width, height) | Fill region of Frame Buffer with color
| PUB  borderAtXY(X, Y, color, thickness, width, height) | Draw border of thinkness* into Frame Buffer
| PUB  lineAtXY(fmX, fmY, toX, toY, color) | Draw line into Frame Buffer
| PUB  circleAtXY(X, Y, diameter, color) | Draw circle into Frame Buffer
| PUB  placeImageAtXY(X, Y, pImgBits, imgWidth, imgHeight, rotation) | Overlay image bits* of color into Frame Buffer
| PUB  flushToDisplay() | Flush Frame Buffer content to Display

* *thickness - of the `borderAtXY()` method may not yet be working...*
* See [Create bitmaps for display on your eInk device](../C-src) for creating bitmaps to use with `placeImageAtXY()`.


Have Fun!



---

> If you like my work and/or this has helped you in some way then feel free to help me out for a couple of :coffee:'s or :pizza: slices!
>
> [![coffee](https://www.buymeacoffee.com/assets/img/custom_images/black_img.png)](https://www.buymeacoffee.com/ironsheep) &nbsp;&nbsp; -OR- &nbsp;&nbsp; [![Patreon](./images/patreon.png)](https://www.patreon.com/IronSheep?fan_landing=true)[Patreon.com/IronSheep](https://www.patreon.com/IronSheep?fan_landing=true)


---

## Disclaimer and Legal

> *Parallax, Propeller Spin, and the Parallax and Propeller Hat logos* are trademarks of Parallax Inc., dba Parallax Semiconductor

---

## License

Copyright © 2022 Iron Sheep Productions, LLC. All rights reserved.

Licensed under the MIT License.

Follow these links for more information:

### [Copyright](copyright) | [License](LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://camo.githubusercontent.com/bc04f96d911ea5f6e3b00e44fc0731ea74c8e1e9/68747470733a2f2f696d672e736869656c64732e696f2f6769746875622f6c6963656e73652f69616e74726963682f746578742d646976696465722d726f772e7376673f7374796c653d666f722d7468652d6261646765

