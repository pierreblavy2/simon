#ifndef Debounce_HPP_
#define Debounce_HPP_


//Usage :
//Step1 : choose what to track : 
//  only push                   => use Debounce_push, 
//  only release                => use Debounce_release, 
//  any push and release        => use Debounce_change 
//  consistent push and release => use Debounce_consistent 

//  NOTE : You cannot use Debounce_push and Debounce_release on the same pin.
//  NOTE : Debounce_change do not handle consistency : you can have a NO push and a Release if push was quick
//         to fix this, use Debounce_consistent. + Debounce consistent do have a get_status function
//  NOTE : Debounce_consistent will output the first debounced CHANGE event.
//         call init_status member in setup to initialie to pin current status
//
//Step2 : define events functions for push/release
//  void my_on_push(){...}
//  void my_on_release(){...}
//
//Step3 : create gloal object for debounce
//  DEBOUNCE_PUSH    (my_push, pin_number, delay_in_ms, my_on_push)
//  DEBOUNCE_RELEASE (my_release, pin_number, delay_in_ms, my_on_release)
//  DEBOUNCE_CHANGE  (my_change,pin_number, delay_in_ms, my_on_push, my_on_release)
//  NOTE : DEBOUNCE_PUSH(my_push, ... ) will create
//         Debounce_push object called my_push 
//         an isr function called my_push_isr()
//         a sethigh function called my_push_sethigh()
//         
//  NOTE : you can change delays latter on, by changing delta_push or delta_release members
//  NOTE : As debounce uses ISR, it requires a global ISR function, so we must use a macro to declare stuff. 
//
//STEP 4: in the sethigh function call the my_xxx_sethigh()  function for each my_xxx object created.
//  
//Step5 : in the main loop call the member function loop_ms(millis() )
// NOTE : this function need to be called faster than debounce delay to avoid input lag.
// NOTE : this function need to be called often enoough to avoid millis overflow (i.e., once every 50 days)



/*=== EXAMPLE ===

//--- Debounce ---
#include "Debounce.hpp"

void push_1()   {Serial.println("p1");} void release_1(){Serial.println("r1");}
void push_2()   {Serial.println("p2");} void release_2(){Serial.println("r2");}
void push_3()   {Serial.println("p3");} void release_3(){Serial.println("r3");}
void push_4()   {Serial.println("p4");} void release_4(){Serial.println("r4");}

DEBOUNCE_PUSH(       button1,IN_1,7, push_1           )
DEBOUNCE_RELEASE(    button2,IN_2,7,         release_2)
DEBOUNCE_CHANGE(     button3,IN_3,7, push_3, release_3)
DEBOUNCE_CONSISTENT( button4,IN_4,7, push_4, release_4)

void setup() { 
  button1_setup();
  button2_setup();
  button3_setup();
  button4_setup();

    
  button4.init_status();
  
  Serial.begin(9600);
}

void loop() {
  //update millis
  now = millis();

  button1.loop_ms(now);
  button2.loop_ms(now);
  button3.loop_ms(now);
  button4.loop_ms(now);                                            
}
*/



//=== MACROS ===
#include <Arduino.h>

#define DEBOUNCE_PUSH(xxx,  pin_number, delay_in_ms, on_push)\
  Debounce_push xxx( pin_number, delay_in_ms, on_push );\
  void xxx##_isr(){xxx .isr_flag=true;}\
  void xxx##_setup(){pinMode(xxx .pin, INPUT); attachInterrupt(digitalPinToInterrupt(xxx .pin), xxx##_isr, RISING);}
//end define
  
#define DEBOUNCE_RELEASE(xxx,  pin_number, delay_in_ms, on_release);\
  Debounce_release xxx( pin_number, delay_in_ms, on_release );\
  void xxx##_isr(){xxx .isr_flag=true;}\
  void xxx##_setup(){pinMode(xxx .pin, INPUT); attachInterrupt(digitalPinToInterrupt(xxx .pin), xxx##_isr, FALLING);}
//end define

#define DEBOUNCE_CHANGE(xxx,  pin_number, delay_in_ms, on_push, on_release);\
  Debounce_change xxx( pin_number, delay_in_ms, on_push, on_release );\
  void xxx##_isr(){xxx .isr_flag=true;}\
  void xxx##_setup(){pinMode(xxx .pin, INPUT); attachInterrupt(digitalPinToInterrupt(xxx .pin), xxx##_isr, CHANGE);}
//end define

#define DEBOUNCE_CONSISTENT(xxx,  pin_number, delay_in_ms, on_push, on_release);\
  Debounce_consistent xxx( pin_number, delay_in_ms, on_push, on_release );\
  void xxx##_isr(){xxx .isr_flag=true;}\
  void xxx##_setup(){\
    pinMode(xxx .pin, INPUT); \
    attachInterrupt(digitalPinToInterrupt(xxx .pin), xxx##_isr, CHANGE);\
  }


//=== CODE ===

  
class Debounce_push{
  public: 
  
  typedef decltype( millis() ) millis_t; //timestamp or duration, in milliseconds
  typedef decltype(LED_BUILTIN) pin_t; //pin number
  
  const pin_t    pin;   // pin number
  millis_t  delta_push; // debounce delay(ms)
  void (*on_push)();    //callback : inject code here to do something on a debounced push
  volatile bool  isr_flag  = false;

  Debounce_push(
    pin_t     pin_,        //pin number
    millis_t  delta_push_, //milliseconds delay
    void(*on_push_)()      //callback
  );
  
  void loop_ms(millis_t now);  //Must be called in main loop, pass now in milliseconds ( millis() )


  public : 
  millis_t       timestamp = 0;     //track last rising edge
  bool           tracking  = false;
};



class Debounce_release{
  public:
 
  typedef decltype(millis()) millis_t; //timestamp or duration, in milliseconds
  typedef decltype(LED_BUILTIN) pin_t; //pin number

  const pin_t    pin;      //pin number
  millis_t delta_release;  //debounce delay(ms)
  void (*on_release)();    //callback : inject code here to do something on a debounced release
  volatile bool  isr_flag  = false;

  Debounce_release(
    pin_t     pin_,        //pin number
    millis_t  delta_push_, //milliseconds delay
    void(*on_push_)()      //callback
  );

  //Must be called in main loop, pass now in milliseconds ( millis() )
  void loop_ms(millis_t now);

  private : 
  std::function<void()> isr_fn;
  millis_t       timestamp = 0;     //track last falling edge
  bool           tracking  = false;
};





class Debounce_change{
   public:
 
  typedef decltype(millis())    millis_t;     //timestamp or duration, in milliseconds
  typedef decltype(LED_BUILTIN) pin_t;        //pin number
  typedef decltype(HIGH)        pin_status_t; //pin status (HIGH or LOW)
  
  Debounce_change(
    pin_t pin_,            //pin number
    millis_t delta_,       //milliseconds delay for both push and release.
    void (*on_push_)(),    //callback on push
    void (*on_release_)()  //callback on release
  );

  const pin_t    pin;           //pin number
  const millis_t delta_push;    //milliseconds debounce delay for push
  const millis_t delta_release; //milliseconds debounce delay for release
  void (*on_push)();            //callback : inject code here to do something on a debounced push
  void (*on_release)();         //callback : inject code here to do something on a debounced release
  volatile bool  isr_flag  = false;


  inline void loop_ms(millis_t now); //Must be called in main loop, pass now in milliseconds ( millis() )

  private : 
  millis_t       timestamp = 0;
  std::function<void()> isr_fn;

    public :  //todo remove me

  //track status
  enum struct Tracking:byte{none, falling,rising};
  Tracking tracking=Tracking::none;
  
};





class Debounce_consistent{
   public:
 
  typedef decltype(millis())    millis_t;     //timestamp or duration, in milliseconds
  typedef decltype(LED_BUILTIN) pin_t;        //pin number
  typedef decltype(HIGH)        pin_status_t; //pin status (HIGH or LOW)
  
  Debounce_consistent(
    pin_t pin_,            //pin number
    millis_t delta_,       //milliseconds delay for both push and release.
    void (*on_push_)(),    //callback on push
    void (*on_release_)()  //callback on release
  );

  const pin_t    pin;           //pin number
  const millis_t delta_push;    //milliseconds debounce delay for push
  const millis_t delta_release; //milliseconds debounce delay for release
  void (*on_push)();            //callback : inject code here to do something on a debounced push
  void (*on_release)();         //callback : inject code here to do something on a debounced release
  volatile bool  isr_flag  = false;

  pin_status_t get_status() const;  //Return a consistent status for the pin, after debouncing and tracking high and low
  pin_status_t init_status();       //Read the pin, set the status according to pin


  inline void loop_ms(millis_t now); //Must be called in main loop, pass now in milliseconds ( millis() )

  private : 
  millis_t       timestamp = 0;
  std::function<void()> isr_fn;

  public :  //todo remove me

  //track status
  enum struct Tracking:   byte{none, falling,rising};  Tracking   tracking=Tracking::none;
  enum struct LastStatus: byte{unknown, high,low};      LastStatus last_status=LastStatus::unknown;

};





//=== Implementation Debounce_push ===
inline Debounce_push::Debounce_push(
    pin_t     pin_, 
    millis_t  delta_push_, 
    void(*on_push_)() 
):
      pin(pin_),
      delta_push(delta_push_),
      on_push(on_push_)
{
      //pinMode(pin, INPUT);
      //auto  isr_fn = [this](){this->isr_flag=true;};
      //attachInterrhight(digitalPinToInterrhight(pin_), isr_fn, RISING);
      //
}



inline void Debounce_push::loop_ms(millis_t now){
    //track
    if(isr_flag){isr_flag=false; timestamp =now; tracking=true; return;}

    //push if delay is long enough
    if(tracking and (now-timestamp > delta_push)  ){
      if(digitalRead(pin)==HIGH){
        tracking=false;
        on_push(); 
      } 
    }
}



//=== Implementation Debounce_release ===
inline Debounce_release::Debounce_release(
  pin_t pin_, 
  millis_t delta_release_, 
  void (*on_release_)() 
):
    pin(pin_),
    delta_release(delta_release_),
    on_release(on_release_)
{
    //pinMode(pin, INPUT);
    //auto  isr_fn = [this](){this->isr_flag=true;};
    //attachInterrhight(digitalPinToInterrhight(pin_), isr_fn, FALLING);
}


inline void Debounce_release::loop_ms(millis_t now){
    //track
    if(isr_flag){isr_flag=false; timestamp =now; tracking=true;return;}

    //push if delay is long enough
    if(tracking and (now-timestamp > delta_release)  ){
      if(digitalRead(pin)==LOW){
        tracking=false;
        on_release(); 
      } 
    }
}



//=== Implementation Debounce_change ===
inline Debounce_change::Debounce_change(
  pin_t pin_, 
  millis_t delta_, 
  void (*on_push_)(), 
  void (*on_release_)() 
):
    pin(pin_),
    delta_push   (delta_),
    delta_release(delta_),
    on_push(on_push_),
    on_release(on_release_) 
{
    //pinMode(pin, INPUT);
    //auto  isr_fn = [this](){this->isr_flag=true;};
    //attachInterrhight(digitalPinToInterrhight(pin_), isr_fn, CHANGE);
}

inline void Debounce_change::loop_ms(millis_t now){
    //track
    if(isr_flag){
      isr_flag  = false; 
      timestamp = now; 
      if(digitalRead(pin)==HIGH){tracking = Tracking::rising;}
      else                      {tracking = Tracking::falling;}
      return;
    }

    switch(tracking){
      case Tracking::none:break;
      case Tracking::rising : {
        if(now-timestamp > delta_push ){
          pin_status_t pin_status =digitalRead(pin); 
          if(pin_status==HIGH){
            tracking=Tracking::none;
            on_push();
          } 
        }
        break;
      }
      case Tracking::falling : {
        if(now-timestamp > delta_release ){
          pin_status_t pin_status =digitalRead(pin); 
          if(pin_status==LOW){
            tracking=Tracking::none;
            on_release();
          } 
        } 
        break;
    }
    }//end switch
}





//=== Implementation Debounce_consistent ===
inline Debounce_consistent::Debounce_consistent(
  pin_t pin_, 
  millis_t delta_, 
  void (*on_push_)(), 
  void (*on_release_)() 
):
    pin(pin_),
    delta_push   (delta_),
    delta_release(delta_),
    on_push(on_push_),
    on_release(on_release_) 
{
    //pinMode(pin, INPUT);
    //auto  isr_fn = [this](){this->isr_flag=true;};
    //attachInterrhight(digitalPinToInterrhight(pin_), isr_fn, CHANGE);
}

inline void Debounce_consistent::loop_ms(millis_t now){
    //track
    if(isr_flag){
      isr_flag  = false; 
      timestamp = now; 
      if(digitalRead(pin)==HIGH){tracking = Tracking::rising;}
      else                      {tracking = Tracking::falling;}
      return;
    }

    switch(tracking){
      case Tracking::none:break;
      case Tracking::rising : {
        if(now-timestamp > delta_push ){
          pin_status_t pin_status =digitalRead(pin); 
          if(pin_status==HIGH){
            tracking=Tracking::none;
            if(last_status==LastStatus::unknown or last_status==LastStatus::low){
              on_push();
              last_status=LastStatus::high;
            }
          } 
        }
        break;
      }
      case Tracking::falling : {
        if(now-timestamp > delta_release ){
          pin_status_t pin_status =digitalRead(pin); 
          if(pin_status==LOW){
            tracking=Tracking::none;
            if(last_status==LastStatus::unknown or last_status==LastStatus::high){
              on_release();
              last_status=LastStatus::low;
            }
          } 
        } 
        break;
    }
    }//end switch
}

Debounce_consistent::pin_status_t Debounce_consistent::get_status()const{
  switch(last_status){
    case LastStatus::unknown: return digitalRead(pin);
    case LastStatus::high:    return HIGH;
    case LastStatus::low:     return LOW;
  }
}


Debounce_consistent::pin_status_t Debounce_consistent::init_status(){
  pin_status_t p=digitalRead(pin);
  if(p==HIGH){last_status=LastStatus::high;}
  else{last_status=LastStatus::low;}
  return p;
}


#endif
