carp - an open source hardware arpeggiator
================================================================================

carp is an experimental hardware MIDI arpeggiator built around the
[Raspberry Pi RP2040](https://en.wikipedia.org/wiki/RP2040).

Hardware
--------------------------------------------------------------------------------

carp uses the [Adafruit Feather](https://learn.adafruit.com/adafruit-feather/overview)
ecosystem. To get started you will need:

  * [Feather RP2040](https://www.adafruit.com/product/4884)
  * [128x64 OLED FeatherWing](https://www.adafruit.com/product/4650)
  * [MIDI FeatherWing](https://www.adafruit.com/product/4740)
  * [Joy FeatherWing](https://www.adafruit.com/product/3632)

You may also want to get a [FeatherWing Quad](https://www.adafruit.com/product/4254)
or similar to make it easier to connect everything.

Firmware
--------------------------------------------------------------------------------

carp's firmware is built using [PlatformIO](https://platformio.org). Because of
[reasons](https://github.com/platformio/platform-raspberrypi/pull/36) platformio
does not natively support the Feather RP2040 and possibly never will, so we use
[the fork upon which that PR is based](https://github.com/maxgerhardt/platform-raspberrypi).

Copyright and License
--------------------------------------------------------------------------------

carp is copyright (c) 2024 Ben Bleything and released under the terms of the MIT
license. See the [LICENSE] file for details.
