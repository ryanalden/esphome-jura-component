# ESPhome - Jura Impressa J6 
This is an ESPhome custom component to communicate with a Jura Impressa J6 coffee machine.

It may also work with other serially-controlled Jura models with minimal adjustment.

It allows monitoring and control via HomeAssistant:

<img src="HomeAssistant_interface.png" alt="drawing" width=55%/>

Readout of water tank level, coffee bean level, tray present/removed, actual machine power state, need cleaning, need flushing, etc, are not currently implemented.

Hardware is a Wemos D1 Mini connected to the 7-pin Jura service port via a 3.3V<->5V logic level converter.

More hardware info in this thread: https://community.home-assistant.io/t/control-your-jura-coffee-machine/26604
