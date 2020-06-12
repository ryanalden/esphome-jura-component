## ESPhome - Jura Impressa J6 
This is an ESPhome custom component to communicate with a Jura Impressa J6 coffee machine.  It may also work with other serially-controlled Jura models with minimal adjustment.

It allows monitoring and control via HomeAssistant:

<img src="images/HomeAssistant_interface.png" alt="drawing" width=55%/>

***

Hardware is a Wemos D1 Mini connected to the 7-pin Jura service port via a 3.3V<->5V logic level converter.\
The D1 mini is powered from the Jura.

<img src="images/seven-pin-interface.jpg" alt="Jura 7-pin interface">

Above image taken from [here](https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604).

<img src="images/connection-diagram.png" alt="Jura 7-pin interface">

If you have diffuculty, try swapping the TX/RX pins.

The D1 Mini and level converter are placed in an enclosure screwed to the back of the Jura, hidden out of the way.

<img src="images/d1-mini-mounting.jpg" alt="D1 mini mounting on back of J6" width=55%/>

Internal connections to the service connector wires are done with "T" tap/splices, leaving the connector itself alone.

<img src="images/t-splice.png" alt="T-splice" width=25%/>

***

Commands for your machine can be generated using the provided script, `generate_esphome_jura_yaml.py`.  It requires the `bitarray` Python module to be installed.

      $ ./generate_esphome_jura_yaml.py AN:01

            - uart.write: [0xDF, 0xDB, 0xDB, 0xDF]  ## 'A'
            - delay: 8ms
            - uart.write: [0xFB, 0xFF, 0xDB, 0xDF]  ## 'N'
            - delay: 8ms
            - uart.write: [0xFB, 0xFB, 0xFF, 0xDB]  ## ':'
            - delay: 8ms
            - uart.write: [0xDB, 0xDB, 0xFF, 0xDB]  ## '0'
            - delay: 8ms
            - uart.write: [0xDF, 0xDB, 0xFF, 0xDB]  ## '1'
            - delay: 8ms
            - uart.write: [0xDF, 0xFF, 0xDB, 0xDB]  ## '\r'
            - delay: 8ms
            - uart.write: [0xFB, 0xFB, 0xDB, 0xDB]  ## '\n'

Particular commands seem to vary by model.\
These work on the Impressa J6, software `TY: PIM V01.01`, `TL: LOADER V3.0`.
Command | Action
--- | ---
AN:01 | Switch On
AN:02 | Switch Off
FA:01 | Switch off, including rinse
AN:0D | Tray Test? Or date related?
FA:02 | Heat up water
FA:03 | Heat up water & Make steam?
FA:06 | Make hot water
FA:07 | Make 1 Espresso
FA:08 | Make 2 Espressi
FA:09 | Make 1 Coffee
FA:0A | Make 2 Coffees
FA:0B | Cup illumination light comes on. Also exits menu system.
FA:0C | Enters the menu system - displays RINSE as the first option
FA:0D | Cycles through menu options [dial counter-clockwise]
FA:OE | Cycles through menu options clockwise [dial-clockwise]
FA:0F | Reads "B. Full", Freezes up
DA:16 | Shows the final two digits on the display, i.e. "16.."

#### To-Do:
- Determine how to initiate a Force Rinse action on this model
- Status of "Fill Beans", "Need Cleaning", and "Need Flushing"
- Actual machine power state (currently we use an 'Optimistic', 'Assumed State' Template switch in ESPhome)
