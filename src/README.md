# Demonstration of PWM using Timer 1

This example shows how to set up Timer 1 (the Advanced Control Timer) to generate
Pulse-Width Modulation (PWM) with complementary outputs on two output pins.
This mode would be used, for example, to drive a motor with a H-bridge.

This program shows the effect of changing the PWM deadtime settings with complementary outputs.

Adapted to the CH32V003J4M6 pinout

## Use

This uses the following pins (using AFIO remapping):

| Pin | Function                         |
| --- | -------------------------------- |
| PA1 | T1CH2 PWM output                 |
| PA2 | T1CH2_N complementary PWM output |
