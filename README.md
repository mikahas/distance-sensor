# Distance sensor

Measure distance using sonar sensor and output the results using a small 128x32 oled display

[See breadboard demo video](https://www.instagram.com/p/BiFvj3Ig2gO/)

## Used third party libraries
- [NewPing](https://bitbucket.org/teckel12/arduino-new-ping/wiki/Home)
- [U8x8lib](https://github.com/olikraus/u8g2/wiki/u8x8reference)

## Known issues
- Some cheap HC-SR04 sonar sensors get stuck when the target is out of range
- OLED display may require an initialisation time before an I2C connection is established