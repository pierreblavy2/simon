
//--- ARDUINO NANO ---
#define nano_RX0 0 //30
#define nano_TX1 1 //31

#define nano_D2  2//32
#define nano_D3  3// 1
#define nano_D4  4// 2
#define nano_D5  5// 9
#define nano_D6  6//10
#define nano_D7  7//11
#define nano_D8  8//12
#define nano_D9  9//13
#define nano_D10 10//14
#define nano_D11 11//15
#define nano_D12 12//16
#define nano_D13 13//17

#define nano_A0 14 //23 
#define nano_A1 15 //24
#define nano_A2 16 //25
#define nano_A3 17 //26
#define nano_A4 18 //27
#define nano_A5 19 //28


//--- RGB LED PIN (common anode +) ---

const byte PIN_LED_R = nano_D4; //-
const byte PIN_LED_G = nano_D5; //-
const byte PIN_LED_B = nano_D6; //-

const byte PIN_CTRL1 = nano_D7;  //+
const byte PIN_CTRL2 = nano_D8;  //+
const byte PIN_CTRL3 = nano_D9;  //+
const byte PIN_CTRL4 = nano_D10; //+


//--- Button PIN (high = pushed) ---

const byte PIN_B1    = nano_D2; //interrupt
const byte PIN_B2    = nano_D3; //interrupt
const byte PIN_B3    = nano_D12;
const byte PIN_B4    = nano_D11; 


//--- digital write requires a clock pin, so ... we clock on something that does nothing
const byte PIN_DUMMY = A7;


//--- Colors ---
 
// PWM, 0=a lot, 1 = no color
// alternate as often as possible to avoid flickering
const byte C0 = 0b11111111;
const byte C1 = 0b01111111;
const byte C2 = 0b01110111;
const byte C3 = 0b01101101;
const byte C4 = 0b10101010;
const byte C5 = 0b10001001;
const byte C6 = 0b00100100;
const byte C7 = 0b00000001;
const byte C8 = 0b00000000;

//--- LED colors ---
const byte COLOR1[3]={C8,C0,C0}; //red
const byte COLOR2[3]={C0,C8,C0}; //green
const byte COLOR3[3]={C0,C0,C8}; //blue
const byte COLOR4[3]={C8,C4,C0}; //yellow

//--- Win/loose colors ---
const byte COLOR_WIN  [3]={C0,C8,C0}; //green
const byte COLOR_LOOSE[3]={C8,C0,C0}; //red

//--- BUTTON status ---
bool IO_B1;
bool IO_B2;
bool IO_B3;
bool IO_B4;


//--- GAME ---
unsigned long game_seed;   //random number generator is deterministic
bool          game_player; //true : human, false : CPU
unsigned long game_end;    //number of turns
unsigned long game_i;      //current  turn


//--- timings ---
const unsigned int delta_zero = 1500;        //how long a color is displayed at the begining of the game
      unsigned int delta      = delta_zero;  //how long a color is displayed
const unsigned int delta_min  = 575;         //delta never get smaller than this value
const unsigned int delta_step = 100;         //each win decrease delta




//---display ---

void displayColor(byte r, byte g, byte b) {
  shiftOut(PIN_LED_R,PIN_DUMMY, MSBFIRST, r);
  shiftOut(PIN_LED_G,PIN_DUMMY, MSBFIRST, g);
  shiftOut(PIN_LED_B,PIN_DUMMY, MSBFIRST, b);
}

void displayColor3(const byte* rgb) {
  shiftOut(PIN_LED_R,PIN_DUMMY, MSBFIRST, rgb[0]);
  shiftOut(PIN_LED_G,PIN_DUMMY, MSBFIRST, rgb[1]);
  shiftOut(PIN_LED_B,PIN_DUMMY, MSBFIRST, rgb[2]);
}


//echo display a LED and BLOCK IO 
inline void led_echo (byte PIN_CTRL_xxx,const byte* COLOR_xxx ){
  unsigned long beg = millis();
  
  digitalWrite(PIN_CTRL_xxx,HIGH);
  while(millis() - beg < delta) { 
    for(byte i = 1; i !=0; ++i){  displayColor3(COLOR_xxx);}  //overflow
  }
  displayColor(C0,C0,C0);
  digitalWrite(PIN_CTRL_xxx,LOW);
}

inline void led_echo1(){led_echo(PIN_CTRL1,COLOR1 );}
inline void led_echo2(){led_echo(PIN_CTRL2,COLOR2 );}
inline void led_echo3(){led_echo(PIN_CTRL3,COLOR3 );}
inline void led_echo4(){led_echo(PIN_CTRL4,COLOR4 );}


inline void led_all(const byte * COLOR_xxx){
  unsigned long beg = millis();
  digitalWrite(PIN_CTRL1,HIGH);
  digitalWrite(PIN_CTRL2,HIGH);
  digitalWrite(PIN_CTRL3,HIGH);
  digitalWrite(PIN_CTRL4,HIGH);
  while(millis() - beg < delta) { 
    for(byte i = 1; i !=0; ++i){  displayColor3(COLOR_xxx);}  //overflow 255
  }
  displayColor(C0,C0,C0);
  digitalWrite(PIN_CTRL1,LOW);
  digitalWrite(PIN_CTRL2,LOW);
  digitalWrite(PIN_CTRL3,LOW);
  digitalWrite(PIN_CTRL4,LOW);
}

inline void led_win()  {led_all(COLOR_WIN);}
inline void led_loose(){led_all(COLOR_LOOSE);}


inline void game_reset(){
  ++game_seed;
  randomSeed(game_seed);
  delta       = delta_zero;
  game_player = false;
  game_end    = 1;
  game_i      = 0;
}

inline void game_win(){
  randomSeed(game_seed);
  ++game_end;
  game_i=0;
  game_player = false;
  if(delta > delta_min + delta_step){delta -= delta_step;}else{delta=delta_min;}
  
}

inline void say(byte x){
    byte answer = random(1,4);
    if(x == answer){
      ++ game_i;
      if(game_i >= game_end){
        game_win();
        led_win();
      }  
    }else{
        game_reset();
        led_loose();
    }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED_R , OUTPUT);  digitalWrite(PIN_LED_R,LOW);
  pinMode(PIN_LED_G , OUTPUT);  digitalWrite(PIN_LED_G,LOW);
  pinMode(PIN_LED_B , OUTPUT);  digitalWrite(PIN_LED_B,LOW);

  pinMode(PIN_CTRL1 , OUTPUT);  digitalWrite(PIN_CTRL1,LOW);
  pinMode(PIN_CTRL2 , OUTPUT);  digitalWrite(PIN_CTRL2,LOW);
  pinMode(PIN_CTRL3 , OUTPUT);  digitalWrite(PIN_CTRL3,LOW);
  pinMode(PIN_CTRL4 , OUTPUT);  digitalWrite(PIN_CTRL4,LOW);

  pinMode(PIN_B1,INPUT);  IO_B1 = false;
  pinMode(PIN_B2,INPUT);  IO_B2 = false;
  pinMode(PIN_B3,INPUT);  IO_B3 = false;
  pinMode(PIN_B4,INPUT);  IO_B4 = false;

  displayColor(C0,C0,C0);

  //--- game init ---
  game_seed=analogRead(nano_A1) xor analogRead(nano_A2);
  game_reset();

}

void loop() {
 
  if(game_player){

      bool b_new1 = digitalRead(PIN_B1);
      bool b_new2 = digitalRead(PIN_B2);  
      bool b_new3 = digitalRead(PIN_B3);
      bool b_new4 = digitalRead(PIN_B4);

      //something chages and it's a press => update LED
      if(b_new1 and b_new1 != IO_B1){led_echo1();say(1);}
      if(b_new2 and b_new2 != IO_B2){led_echo2();say(2);}
      if(b_new3 and b_new3 != IO_B3){led_echo3();say(3);}
      if(b_new4 and b_new4 != IO_B4){led_echo4();say(4);}

      //renumber
      IO_B1=b_new1;
      IO_B2=b_new2;
      IO_B3=b_new3;
      IO_B4=b_new4;

      //delay (save battery)
      //poweroff timeout
      //TODO power on on interrupt
      
  }else{
      // CPU plays (blocking)
      randomSeed(game_seed);
      for(unsigned int i = 0; i < game_end; ++i){
           byte  showid = random(1,4);
            switch(showid){
              case 1 : led_echo1(); break;
              case 2 : led_echo2(); break;
              case 3 : led_echo3(); break;
              case 4 : led_echo4(); break;
            }
            delay( delta/3 );          
      }
      
      randomSeed(game_seed);
      game_player = true;
  }
}



/*
=== HARDWARE LIST ===


Updated 25 seconds ago

This simple project describes how to build a 4 color Simon say game with

=== HARDWARE LIST ===

(controller)
    1x : arduino nano, the 2*15 male pins must be soldered to face the ground.
    2x: 15 female pins for arduino support

(LED output)
    4x : 3v RGB common anode LED (here 5mm + support)
    3x : 330 ohms resistors
    4x : 4 pins male connectors (here JST)
    4x : 4 pins female connector (here JST)

(switch input)
    4x : momentary switch (red, green, blue, yellow)
    4x : 2 pins male connectors (here JST)
    4x : 2 pins female connectors (here JST)

(power)
    1x : latch switch (for power on/off)
    2x: 2pins male connector (here JST)
    2x: 2pins female connector (here JST)
    1x: 2 blacks male pins (optional)
    1x : 2 red male pins (optional)
    1x : 9V battery (may change, anything that power an arduino nano on VIN is OK)

(soldering)
    1x : 6 * 7 cm perforated board (24 columns and 18 lines)
    solder (here led free SN99.3, CU0.7 soldering iron must be 371 °C)





 
 

 
  |------------0-----------||-------------10-------------||------20-----|
  1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4  
  
A R--------R--------R--------R------~~330R~~--(1)                    

B G--------G--------G--------R------~~330R~~--(2)       X  X  X      

C B--------B--------B--------R------~~330R~~--(3) TX1   X  X  X    VIN-(51)

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



=== LEGEND  ===
Component face is shown
- and | : soldering (371°c, lead free)
~~xx~~  : resistors of xx ohms
X       : unconnected pin


=== WIRES (14 wires) ===

--- RGB (-) LED 3v ---
(1) R to D4 on arduino nano : R color control
(2) G to D5 on arduino nano : G color control
(3) B to D6 on arduino nano : B color control

--- RGB (+) LED 3v ---
(4) X1 to D7  on arduino nano  : red    LED1 contol
(5) X2 to D8  on arduino nano  : green  LED2 control
(6) X3 to D9  on arduino nano  : blue   LED3 control
(7) X4 to D10 on arduino nano  : yellow LED4 control

--- SWITCH (momentary) ---
pull down resistor 10K
0 = not pressed
1 = pressed
(8)  S1 to D2  on arduino nano  : red    switch1
(9)  S2 to D3  on arduino nano  : green  switch2
(10) S3 to D12 on arduino nano  : blue   switch3
(11) S4 to D11 on arduino nano  : yellow switch4

--- POWER ---
(50) to arduino GND
(51) to arduino VIN (battery power)
(52) to arduino +5V (arduino power output)




=== Connectors  ===

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


