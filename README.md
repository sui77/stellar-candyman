# DIY Stellar Candy Dispenser

Here comes yet another crypto candy dispenser. This one is Wi-Fi enabled, works for any asset on the stellar network and costs less than 20 EUR.

## Parts
All you need is an electric candy dispenser like the one I've bought on amazon for about 11 EUR, a microcontroller and a motor driver.

We are going to use a esp8266 microcontroller, which is an excellent choice because it has built in Wi-Fi, is easily programmed with the Arduino IDE and only costs a couple of bucks. There are various esp boards out there but my favorite one is NodeMCU (you can get them  for ~5 EUR at ebay or alibaba) which exposes most pins, has a built in USB interface for programming and has an ideal form factor for prototyping on a breadboard. 

Since we can not power a motor directly from a microcontroller, we also need a motor driver. The L293d is one of the most frequently used integrated circuits for hobby projects. You connect it to a capable power source (batteries), to a dc-motor and to your microcontroller and that's it.

[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy01t.jpg)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy01.jpg)
[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy02t.jpg)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy02.jpg)

## Assembly and Wiring
Open the top and the bottom (screws are located under the rubber pads) of the dispenser and cut+remove all wires. The NodeMCU as well as the L293d work fine with 4.5V so we are powering them with the 3 AAA batteries from the candy dispenser itself. They won't last too long, look for 1865 Li-Ion batteries and a TP4056 battery module if you want a longer battery life. The L293ds pins are symmetric and it does not matter which side you use for connection, the other side would have been for controlling a second motor. The minus pole in my dispenser was originally wired with a red cable, you should better double check the polarity. Solder the L293d on a perfboard, wire everything as shown in the picture and simply tape it on the back of the NodeMcu.

[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candyCircuitt.png)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candyCircuit.png)
[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy03t.jpg)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy03.jpg)
[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy04t.jpg)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy04.jpg)
[![enter image description here](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy05t.jpg)
](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/candy05.jpg)


## Software
Because I'm not aware of any existing stellar arduino sdk, I've implemented consuming the horizon payment stream http endpoint on my own. It's just a couple of lines so I'll just leave the method in the Arduino sketch for now instead of separating it into a reusable library. The example sketch (re)connects to horizon, parses incoming payment operations and runs the motor for 1.5 seconds whenever a payment was received. The motor is controlled by setting one of it pins HIGH and the other one LOW and stopped by setting both pins to LOW. If you are not familiar with Arduino, you can get the IDE [here](http://arduino.com), add the esp8266 board like [this](https://www.instructables.com/id/Quick-Start-to-Nodemcu-ESP8266-on-Arduino-IDE/) and add the needed ArduinoJson library as described [here](https://www.arduino.cc/en/guide/libraries#toc3).

## See it in action
[![Everything Is AWESOME](https://raw.githubusercontent.com/wiki/sui77/stellar-candyman/img/youtube.jpg)](https://www.youtube.com/watch?v=3D9xDwbBz4U "Everything Is AWESOME")
