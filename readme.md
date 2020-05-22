# HARDWARE LIST
## Controller
    1x : arduino nano, the 2*15 male pins must be soldered to face the ground.
    2x: 15 female pins for arduino support

## LED output
    4x : 3v RGB common anode LED (here 5mm + support)
    3x : 330 ohms resistors
    4x : 4 pins male connectors (here JST)
    4x : 4 pins female connector (here JST)

## Switch input
    4x : momentary switch (red, green, blue, yellow)
    4x : 2 pins male connectors (here JST)
    4x : 2 pins female connectors (here JST)

## Power
    1x : latch switch (for power on/off)
    2x: 2pins male connector (here JST)
    2x: 2pins female connector (here JST)
    1x: 2 blacks male pins (optional)
    1x : 2 red male pins (optional)
    1x : 9V battery (may change, anything that power an arduino nano on VIN is OK)

Soldering
    1x : 6 * 7 cm perforated board (24 columns and 18 lines)
    solder (here led free SN99.3, CU0.7 soldering iron must be 371 °C)



# Schematics
Component face is shown
- and | : soldering (371°c, lead free)
~~xx~~  : resistors of xx ohms
X       : unconnected pin
(xx)    : wire


```text
  |------------0-----------||-------------10-------------||------20-----|
  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  
  
A R--------R--------R-----------R---~~330R~~--(1)                    

B G--------G--------G-----------R---~~330R~~--(2)       X  X  X      

C B--------B--------B-----------R---~~330R~~--(3) TX1   X  X  X    VIN-(51)

D P1       P2       P3          P4                RX0              GND-(50) 
  |        |        |           |
E (4)      (5)     (6)         (7)    (51)        RST              RST
                                      |  
F                                     |           GND         (52)-+5V
                                      |   
G                                     Sp Sp   (8)-D2               A7
                                         |
H           (50)                         |    (9)-D3               A6
              |                          |
I ------------------------------------|  |        D4-(1)           A5
  |        |        |        |        |  |
J ~        ~        ~        ~        K  V        D5-(2)           A4
  ~        ~        ~        ~        |  |
K 10K      10K      10K      10K      K  V        D6-(3)           A3
  ~        ~        ~        ~        |  |
L ~        ~        ~        ~        |  |    (4)-D7               A2
  ~        ~        ~        ~        |  |
M |        |        |        |        V- V+   (5)-D8               A1
  |        |        |        |
N (8)     (9)      (10)     (11)              (6)-D9               A0
  |        |        |        | 
O |        |        |        |                (7)-D10              REF
  |        |        |        |
P S1       S2       S3       S4              (11)-D11              3V3

Q S -------S -------S -------S ---(52)       (10)-D12              D13

R

```





## WIRES (14 wires)

### RGB (-) LED 3v
(1) R to D4 on arduino nano : R color control
(2) G to D5 on arduino nano : G color control
(3) B to D6 on arduino nano : B color control

### RGB (+) LED 3v
(4) X1 to D7  on arduino nano  : red    LED1 contol
(5) X2 to D8  on arduino nano  : green  LED2 control
(6) X3 to D9  on arduino nano  : blue   LED3 control
(7) X4 to D10 on arduino nano  : yellow LED4 control

### SWITCH (momentary)
0 = not pressed
1 = pressed

(8)  S1 to D2  on arduino nano  : red    switch1
(9)  S2 to D3  on arduino nano  : green  switch2
(10) S3 to D12 on arduino nano  : blue   switch3
(11) S4 to D11 on arduino nano  : yellow switch4

### POWER
(50) to arduino GND
(51) to arduino VIN (battery power)
(52) to arduino +5V (arduino power output)




## Connectors

--- LEDS 4 pins *4 ---
4 pins : RGBX, ] (holes on the left) : LEDs
]R |
|G |
|B |
]Px|

RGB goes to red,green,blue cathodes (-)
Px  goes to anode
all RGB leds are the same, common anode(+), 5mm
led1 : red     in game
led2 : green   in game
led3 : blue    in game
led4 : yellow  in game


--- game switch 2 pins *4---
2 pins switch [ (holes on the right)
|S[
|S[

Switches are 12mm momentary switch
switch1 is red
switch2 is green
switch3 is blue
switch4 is yellow


--- power battery 2pins *1 ---
2 pin power battery (horisontal, hole on the bottom)
V- V+

V- goes to battery -
V+ goes to battery +

Anything that works with an arduino is OK. Here we use 2*4 AAA batteries


--- power switch (latch) 2 pins *1 ---
2 pin power switch (horisontal, hole on the top)
Sp Sp

power switch is a 12mm latch switch. 
=> NOTE as the switch has its own LED a resistor + wires are soldered next to the switch,
in order to make it work on its own as if it was a classical 2 pole switch.




=== debug pins ===
power ground, 2 black pins
K 
K

power V+, 2 red pins
V
V


=== Arduino nano, usb on bottom ===

6 pin SPI (not connected, pins are used as a visual reference to put arduino in the right direction)
XXX
XXX

15 pin left   : C17 to Q17
15 pins right : C23 to Q27


*/
