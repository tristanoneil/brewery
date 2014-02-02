## About Brewery

Arduino source to replace the firmware that comes with
the [picoBrew EBIAB System](http://www.electricbiab.com). The goal
of this firmware is to provide an "API" via USB that that can recieve
temperature inputs that turn on/off the main heating element, as well recieve
a digital signal that can turn on/off the chugger pump. All of the user interface
is handled in a seperate application that interacts with Brewery's API.

## Harware Notes

The following are the pin locations of various devices that are
connected to the Arduino Mega 2560.

- 11: Temperature Probe
- 49: Alarm
- 50: 110 V SSR
- 52: 220 V SSR
