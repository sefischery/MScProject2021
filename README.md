# MSc Project - 2021 - IoT Security And Performance

<img src="https://images.squarespace-cdn.com/content/5b052242506fbe7ea6c0969c/1539868936426-869NHDYJ3T0P9JJE2G5J/DTU_Logo_Corporate_Red_RGB.png?format=1500w&content-type=image%2Fpng" width="96">

## Members
- Sebastian Fischer, s164158
- Magnus Glasdam Jakobsen, s164156

## Make new environment
Make a new environment in platformio.ini, where all build properties are specified in relation to the environment.
Go to Settings -> Build, Execution -> CMake and press '+'. If done correctly, it should add the specified environment name automatically. 

Next, go to Tools -> PlatformIO, and press re-init. This should allow you to choose the environment at the left hand side of the upper run button.

## Listening on COMs in cmd
To listen on different coms via windows command line use the following code:

pio device monitor -p COM5 -b 115200 

Whereas -p is a shortcut for PORT and -b is for BAUDRATE. Furthermore, in the example the port is set to COM5 and the baudrate to 115200, which both can changed.

## Use Sigfox
When uploading the Sigfox sketch, the defined message will be transmitted and be available online.

The message can be verified and seen on https://backend.sigfox.com/.

- Login to https://backend.sigfox.com/
- Press 'Device' in the top left hand-side.
- Under the list of devices, press the device id.
- In the left menu, press messages.

All message send from that device will be located here, and can be decode with a hex decoder.