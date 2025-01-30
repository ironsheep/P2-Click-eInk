# Developing a P2 Application interacting with the new Mikroe eInk Click P2 Driver 

Add an eInk display to your own project!

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

You now need to add eInk objects to your project.


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
    
    ' we are using a 2.13" eInk display
    EINK_DISPLAY_TYPE = eInkDisplay.DS_eink213

PUB main() | ok
'' DEMO let's see what our radio hears!

    ' tell object where the module is connected
    ok := eInkDisplay.start(EINK_DISPLAY_TYPE, EINK_CLICK_PINGROUP)
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
| <pre>PUB setOrientation(eRotation)</pre> | Once your device is mounted, the connector will be above, below, left or right of the display. Use this call notify the drawing routines which corner is now at top left (0, 0) in your hardware set up. See [Orientation Pictures](orientation.md) for more detail.</br>This affects all subsequent drawing calls<\BR> NOTE: selectFont() rotation is in addition to this device orientation</br>** eRotation** is [ROTATE\_0, \_90,, \_180, or \*_270 enum value]
| PUB displaySize() : widthInPix, heightInPix | Return display size in pixels</br>**NOTE**: only valid is device is successfully started!</br>(ensure start() is called, first and that it is successful!)
| PUB displayType() : eDisplayType| Return display size in pixels</br>**NOTE**: only valid is device is successfully started!</br>(ensure start() is called, first and that it is successful!)</br>**eDisplayType** is [DS\_* enum value]
| PUB waitForDisplayComplete() |Wait for eINK display to indicate that last update is complete</br>**NOTE**: use this before a driver stop() to ensure the display is completely written.
| **WRITE to BUFFER then immediately flush buffer to DISPLAY** 
| PUB clearScreen() | Reset display to blank (all white)
| PUB  fillScreen(eColor) | Fill display with {eColor}
| PUB  displayImage(pImageBffr) | Write image bitmap to Display
| PUB  displayTextAtXY(X, Y, pText) | Write zstr to Display at device row, column
| **WRITE TO BUFFER** | **NOTE:** when all buffer writes are done call `flushToDisplay()` to write to display </br>(*which can take 5-20 seconds depending up display used*)
| PUB  selectFont(eFontOrientation, eColor, eFont) | Set font configuration
| PUB  setBackground(eColor) | Fill Frame Buffer with {eColor}
| PUB  setTextAtXY(X, Y, pText) | Write zstr to Frame Buffer at device row, column
| PUB  fillRegionAt(X, Y, eColor, width, height) | Fill region of Frame Buffer with eColor
| PUB  borderAtXY(X, Y, eColor, thickness, width, height) | Draw border of thinkness* into Frame Buffer
| PUB  lineAtXY(fmX, fmY, toX, toY, eColor) | Draw line into Frame Buffer
| PUB  circleAtXY(X, Y, diameter, eColor) | Draw circle into Frame Buffer
| PUB  placeImageAtXY(X, Y, pImgBits, imgWidth, imgHeight, eRotation) | Overlay image bits* of color into Frame Buffer
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

Licensed under the MIT License.

Follow these links for more information:

### [Copyright](copyright) | [License](LICENSE)

[maintenance-shield]: https://img.shields.io/badge/maintainer-stephen%40ironsheep%2ebiz-blue.svg?style=for-the-badge

[license-shield]: https://img.shields.io/badge/License-MIT-yellow.svg


