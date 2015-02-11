Open-IO
=======


A drop-in replacement for the EXT-IO. If you're not sure what the EXT-IO is, you probably don't need a replacement.

> **Required Parts:**

> - Arduino Mega 2560
> - Max232 - To convert rs232 12v from BemaniPC/Python 2 to 5v for Arduino
> - Hookup pins/wires

---

Connector & Cable Identification
------------

To install the Open-IO, we first need to identify a few connectors inside of your DDR cabinet - Pad connectors, Neon Light connector, and Serial Communication connector.

**Pad Connectors**
There are 2 pad connectors (P1 - White; P2 - Orange) that each have 7 cables.
Pin Name    | JP Cab Color | Kor Cab Color
----------- | ------------ | -------------
Ground      | Black        | Black
FL1         | Green        | Blue
FL1         | Blue         | Green
FL1         | Purple       | Yellow
FL1         | Grey         | Orange
FL1         | White        | Red
TEST        | Brown        | Brown

**Neon Connector**
There is a Neon light connector (White) that has 2 cables (Red & Black).

**Serial Communication Connector**
Lastly we need to identify the serial communication cable from your BemaniPC or Python2. This connector has 3 cables.
Pin Name | Color
-------- | -----
GND      | Black
RX       | Grey
TX       | Purple

Installation
---

Connect your Arduino using this pin-out:

Arduino Pin | Destination
----------- | -----------
0           | RX on TTL side of Max232
1           | TX on TTL side of Max232
2           | P1 FL1
3           | P1 FL2
4           | P1 FL3
5           | P1 FL4
6           | P1 FL5
7           | P1 TEST
8           | P2 FL1
9           | P2 FL2
10          | P2 FL3
11          | P2 FL4
12          | P2 FL5
13          | P2 TEST
22          | Neon Red
GND         | P1 GND
GND         | P2 GND
GND         | Neon GND
GND         | Max232 GND

And connect your Max232 with this pin-out:
> **Note**
> All of these Max232 pins are on the RS232 side
> 
> If your Max232 has a db9 port, refer to the db9 pin column.

Max232 Pin | db9 Pin | Serial Data Connector Pin
---------- | ------- | -------------------------
RX         | 2       | RX
TX         | 3       | TX
GND        | 5       | GND
