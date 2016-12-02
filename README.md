# Tic-Tac-Turtle: Arcade Button Controller Firmware

## Overview
This repository contains firmware for enabling an Arduino Nano to drive up to 4
backlit arcade buttons and detect button press events. The Nano's serial-over-USB
interface is used to propagate buttons events to a host as well as allowing the
host to upload lighting sequences for the button backlights.

## Switches
Switch contacts are expected to be normally open. Each switch must have one
contact share a common ground with the Nano. The other contacts from each switch
are connected to A0, A1, A2 and A3 respectively.

Button events are debounced in firmware.

## Backlights
The Nano can drive up to 40mA per GPIO pin provided total VCC current does not
exceed 200mA. If button backlights are to be driven directly by the Nano, it
is recommended that each backlight is LED-based and does not draw more than 30mA.
All backlight cathods must share a common ground with the Nano. Anodes for each
backlight are connected to D4, D5, D6 and D7 respectively.

Switch input A0 should correspond with backlight output D4, A1 with D5 and so on.

## Host Interface Protcol
A simple ASCII encoded hexadecimal control protocol is implemented using the Nano's
serial-over-USB interface. The line format used is 38400 baud, 8-bit word width,
no parity, 1 stop bit.

Whenever a button is pressed or released, or a new backlight sequence is committed
(see below), a single upper-case hexademical digit is transmitted to the host
presenting the current state of the switch inputs A0-A3 as a 4-bit value. The
state is inverted such that open contacts are represented by cleared bits and closed
contacts are represented by set bits.

The backlight outputs are controlled by a continuously cycling 12 stage sequencer.
To define a sequence, the host must first send a single hexadecimal digit
denoting the sequence step speed (20Hz / (n+1), where n is the digit value).
This must be followed by 12 further hexadecimal digits representing the 12 4-bit output
states to sequentially write to backlight outputs D4-D7. Note bit 0 corresponds to
output D4, bit 1 corresponds to output D5 etc. The new sequence is committed on
reception of an ASCII carriage-return character (0x0D).

Any received strings that do not conform to the pattern described are discarded.
Lower and upper case hexadecimal characters are accepted (0-9, A-F, a-f).
