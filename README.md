# cie-PN532
Arduino library for SPI access to the PN532 NFC chip in the Italian Electronic Identity Card (CIE).

## Prerequisites
This library depends upon the **Adafruit_PN532** library which you can obtain from its GitHub Repository.

https://github.com/adafruit/Adafruit-PN532

You must install both the Adafruit_PN532 and cie_PN532 in your Arduino _libraries_ directory.
Plese follow the instructions from the Arduino guide.

https://www.arduino.cc/en/Guide/Libraries


## Wiring PN532 breakout to the Arduino Uno for SPI communication
At present, just the SPI connection is supported. The examples provided will work with this wiring.

![images/pn532-wiring-spi.png](images/pn532-wiring-spi.png)

This wiring is also described in detail on this page at the Adafruit website.

https://learn.adafruit.com/adafruit-pn532-rfid-nfc/breakout-wiring

## Examples
This library comes with an _examples_ directory. You can load and run examples from the Arduino IDE by clicking the File menu -> Examples -> cie 532.


## Useful links
 * The CIE 3.0 chip specification (italian)
 http://www.agid.gov.it/sites/default/files/documentazione/cie_3.0_-_specifiche_chip.pdf
 * Technical specification for the European Card for e-Services and National e-ID Applications
 http://www.unsads.com/specs/IASECC/IAS_ECC_v1.0.1_UK.pdf