Introduzione
===============

Libreria Arduino per la lettura della **Carta Italiana Elettronica** mediante il chip della scheda PN532 NFC.

Prerequisiti
--------------

Questa libreria dipende dalla più complessa e articolata libreria Adafruit_PN532 che è possibile ottenere (in versione modificata) dalla seguente repository su GitHub.

    https://github.com/BrightSoul/Adafruit-PN532

È fondamentale installare entrambe le librerie Adafruit_PN532 e cie_PN532 nella directory ``/libraries`` di Arduino. Puoi seguire le istruzioni anche sulla guida ufficiale messa a diposizione da Arduino.

    https://www.arduino.cc/en/Guide/Libraries

Collegamento tra Arduino Uno e PN532 
--------------------------------------

Al momento è supportata solamente la connessione SPI. La figura sottostante rappresenta lo schema da emulare per il corretto funzionamento dell'intero sistema.

.. image:: ../images/pn532-wiring-spi.png

Questo particolare tipo di schema è anche spiegato in dettaglio sul sito web di Adafruit.
    
    https://learn.adafruit.com/adafruit-pn532-rfid-nfc/breakout-wiring

Getting started
-----------------

Come primo step è fondamentale creare un nuovo progetto Arduino e settarlo nel seguente modo

.. code-block:: cpp

    //Include some libraries
    #include <Wire.h>
    #include <SPI.h>
    #include <cie_PN532.h>

    //Use the cie_PN532 with the typical wiring, as pointed out above
    cie_PN532 cie();

    void setup(void) {
    #ifndef ESP8266
        while (!Serial); // for Leonardo/Micro/Zero
    #endif
    Serial.begin(115200);
    //Initialize the PN532 breakout board
    cie.begin();
    }

Dopo aver fatto ciò, nella sezione *loop*, bisogna aspettare che venga letta la carta CIE e in seguito leggere il suo valore ``ID_Servizi``

.. code-block:: cpp

    void loop(void) {
  //Let's see if a card is present
  bool cardDetected = cie.detectCard();
  if (!cardDetected) {
    //No card present, we wait for one
    delay(100);
    return;
  }

  //Good! A card is present, let's read the ID!
  word bufferLength = EF_ID_SERVIZI_LENGTH;
  byte buffer[EF_ID_SERVIZI_LENGTH];

  if (!cie.read_EF_ID_Servizi(buffer, &bufferLength)) {
    Serial.print(F("Error reading EF.ID_SERVIZI"));
    delay(1000);
    return;
  }

  //We were able to read the ID_Servizi, print it out!
  Serial.print(F("EF.ID_Servizi: "));
  cie.printHex(buffer, bufferLength);
    }

Altri esempi
--------------

Per caricare altri esempi dalla libreria è possibile farlo direttamente dall’IDE di Arduino. 
In particolare essi sono reperibili dal menù: ``File menu -> Examples -> cie_532``.