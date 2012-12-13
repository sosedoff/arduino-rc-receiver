# arduino-rc-receiver

Module to display readings from RC receiver for debugging purposes.

## Background

This module is designed to get data from your RC receiver and display each of its channels PWM reading in terminal monitor. It uses [PinChangeInt](http://code.google.com/p/arduino-pinchangeint/) library to read signals from arduino pins and does not require any hardware or anything, just jumper wires. 

One of the possible use cases would be an adjustment of a flight controller board, like KK Multicopter, or even a custom one. Technically, it is possible to include default `Servo` library and hook up any ESC to control brushless motors directly with arduino. Since most of the ESC's do not have a PWD standards it would be extremely useful to know which signal level arms it, or an operational range for motors. Plus, some of the receivers behave in a different manner once the connection link with transmitter is lost: some receivers keep the same PWM output, some generate a random number out of the bounds, which will make the ESC either to stop immediately or go on a crazy ride. 

## Usage

Currently, module should only read from 4 basic RC channels: aileron, elevator, throttle and rudder. You'll need a few jumper cables and wires to connect outputs directly to arduino. 

The only piece of code you'll have to change is the channel input pins definitions:

```
#define AILERON_IN  4
#define ELEVATOR_IN 5
#define THROTTLE_IN 6
#define RUDDER_IN   7
```

Connect receiver directly to arduino's power +5V and GND, compile and upload.

In the terminal monitor you'll see something like this:

![Log](https://raw.github.com/sosedoff/arduino-rc-receiver/master/log.png)

Example:

![RC](https://raw.github.com/sosedoff/arduino-rc-receiver/master/photo.jpg)