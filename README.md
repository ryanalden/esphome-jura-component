# ESPhome - Jura Impressa J6 
This is an ESPhome custom component to communicate with a Jura Impressa J6 coffee machine.

It may also work with other serially-controlled Jura models with minimal adjustment.

It allows monitoring and control via HomeAssistant:

<img src="HomeAssistant_interface.png" alt="drawing" width=55%/>

To-Do:
- Determine how to initiate the Force Rinse action on this model
- Status of "Fill Beans", "Need Cleaning", "Need Flushing"
- Actual machine power state (currently we use an 'Optimistic', 'Assumed State' Template switch in ESPhome)

Hardware is a Wemos D1 Mini connected to the 7-pin Jura service port via a 3.3V<->5V logic level converter.

More hardware info in this thread: https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604

***

Particular commands seem to vary by model.  These work on the Impressa J6:
Command | Action
--- | ---
AN:01 | Switch On
AN:02 | Switch Off
FA:01 | Switch off, including rinse
AN:0D | Tray Test? Or date related?
FA:02 | Heat up water
FA:03 | Heat up water & Make steam?
FA:06 | Make hot water
FA:07 | 1 Espresso
FA:08 | 2 Espressi
FA:09 | 1 Coffee
FA:0A | 2 Coffees
FA:0B | Cup illumination light comes on. Also exits menu system.
FA:0C | Enters the menu system - displays RINSE as the first option...
FA:0D | Cycles through menu options [dial counter-clockwise]
FA:OE | Cycles through menu options clockwise [dial-clockwise]
FA:0F | Reads "B. Full", Freezes up
DA:16 | Shows the final two digits on the display, i.e. "16.."
