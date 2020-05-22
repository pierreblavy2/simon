# Summary
This project describe how to do a Simon's say memory game with an arduino nano, 4 RGB leds and switches.

The code for the arduino is provided.

Everything is licensed under GPL 3.0+


# HARDWARE LIST
## Controller
- 1x : arduino nano, the 15 male pins on the arduino must be soldered to face the ground when the USB port faces the sky
- 2x: 15 female pins for arduino support
- 2x: 3 female pins (optional, for visual only) to mark where the 6 arduino pins should go, so the user doesn't plug it in the wrong direction.

## LED output
- 4x : 3v RGB common anode LED (here 5mm + support)
- 3x : 330 ohms resistors
- 4x : 4 pins male connectors (here JST)
- 4x : 4 pins female connector (here JST)

## Switch input
- 4x : momentary switch (red, green, blue, yellow) open : not pressed, closed : pressed
- 4x : 2 pins male connectors (here JST)
- 4x : 2 pins female connectors (here JST)

## Power
- 1x : latch switch (for power on/off)
- 2x: 2pins male connector (here JST)
- 2x: 2pins female connector (here JST)
- 1x: 2 blacks male pins (optional)
- 1x : 2 red male pins (optional)
- 1x : 9V battery (may change, anything that power an arduino nano on VIN is OK)

## Soldering
- 1x : 6 * 7 cm perforated board (24 columns and 18 lines)
- solder (here led free SN99.3, CU0.7 soldering iron must be 371 °C)




# Schematics
## Legend
```text
Component face is shown
- and | : soldering (371°c, lead free)
~~xx~~  : resistors of xx ohms
X       : unconnected pin
(xx)    : wire
```

## Perforated board  
```text
  |------------0-----------||-------------10-------------||------20-----|
  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  
  
A R--------R--------R-----------R---~~330R~~--(1)                    

B G--------G--------G-----------R---~~330R~~--(2)       X  X  X      

C B--------B--------B-----------R---~~330R~~--(3) TX1   X  X  X    VIN-(13)

D P1       P2       P3          P4                RX0              GND-(12) 
  |        |        |           |
E (4)      (5)     (6)         (7)    (13)        RST              RST
                                      |  
F                                     |           GND         (14)-+5V
                                      |   
G                                     Sp Sp   (8)-D2               A7
                                         |
H           (12)                         |    (9)-D3               A6
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

Q S -------S -------S -------S ---(14)       (10)-D12              D13

R
```

## Wires on the board

### RGB (-) LED 3v
- (1) R to D4 on arduino nano : R color control
- (2) G to D5 on arduino nano : G color control
- (3) B to D6 on arduino nano : B color control

### RGB (+) LED 3v
- (4) X1 to D7  on arduino nano  : red    LED1 contol
- (5) X2 to D8  on arduino nano  : green  LED2 control
- (6) X3 to D9  on arduino nano  : blue   LED3 control
- (7) X4 to D10 on arduino nano  : yellow LED4 control

### SWITCH (momentary)
- (8)  S1 to D2  on arduino nano  : red    switch1
- (9)  S2 to D3  on arduino nano  : green  switch2
- (10) S3 to D12 on arduino nano  : blue   switch3
- (11) S4 to D11 on arduino nano  : yellow switch4

### POWER
- (12) to arduino GND
- (13) to arduino VIN (battery power)
- (14) to arduino +5V (arduino power output)




## Connectors and connected stuff

### RGB LEDs 4 pins x4
soldered verticay, holes on the left
```text
]R |
|G |
|B |
]Px|
```

all RGB leds are the same, common anode(+), 5mm
- led1 : red     in game
- led2 : green   in game
- led3 : blue    in game
- led4 : yellow  in game

- R goes to led red (-)
- G goes to led green (-)
- B goes to led blue (-)
- Px goes to led common anode (+)

### Game switches 2 pins x4
soldered verticay, holes on the right
```text
|Sx[
|S [
```

Switches are 12mm momentary switch. pressed=open
- switch1 is red
- switch2 is green
- switch3 is blue
- switch4 is yellow

- Sx goes to one pin of the switch
- S  goes to the other pin


### Power battery 2 pins, x1
WARNING : polarity convention may change between batteries, 
==> so wire your connector accordingly to your power source.

soldered horizontally, hole on the bottom (ajust to your battery if polarity changes)
```text
V- V+
```

- Anything that works with an arduino is OK. Here we use 2*4 AAA batteries

- V- goes to battery -
- V+ goes to battery +


### Power switch 2 pins, x1
soldered horizontally, hole on the top
```text
Sp Sp
```

- power switch is a 12mm latch switch.

- Sp goes to the switch terminal
- Sp goes to the other  terminal




## Debug pins
Debug pins are optional but convenient to mark, with colored stuff, power source polarity.

### ground, 2 black pins
soldered vertically, connected to power source and arduino ground
```text
K
K
```

### Power V+, 2 reds pins
soldered vertically, connected to power source V+ and arduino VIN
*NOT* connected to a regulated voltage port on the arduino.
```text
V
V
```

## Arduino nano
plugged with the USB port on the bottom side.

The 6 pin marked with `X` are not connected, but you may solder something here (like female pins) as a placeorder
to add visual information for the user, so he doesn't plug the arduino in the wrong direction.


