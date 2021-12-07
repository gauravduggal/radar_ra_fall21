////////////////////////////////////////////////////////////
/* Code written by Gaurav Duggal 06-12-2021               */
//Reference: Atmega 328 datasheet and Arduino Reference page
////////////////////////////////////////////////////////////
/* This code uses the INT O hardware interrupt and timer1 in the atmega 328 to measure the pulse high and low time
 *  It is assumed the high time corresponds to the tx pulse duration + setup time. This is generally used to control
 *  the rx-tx switch in a radar. This code also generates a NOT signal on PD4 (pin 7 ) to protect the secondary rx array
 *  from the high tx transmit power. pin 7 is toggled within 5 us of the rx-tx switch being toggled.
 *  NOTE: Multiple the readings outputted by the serial port by 4us to get the on and off time in seconds.
 */
//use internal arduino library interrupt or direct register level code, the latter is faster 
#define ARDUINO_INTERRUPTS 0
//use to enable print messages on the Serial UART
#define ENABLE_PRINTS 1
#define sig 2
#define PD4_bm1  (1<<PD4)
#define PD4_bm2 ~(1<<PD4)
#define PD2_bm1  (1<<PD2)
#define PD2_bm2 ~(1<<PD2)

volatile unsigned int prev_state = 0;
volatile unsigned int state = 0;
#if ENABLE_PRINTS
volatile unsigned int ti = {0};
volatile unsigned int gtime[2] = {0};
volatile byte flag = 0;
volatile byte f_idx = 0;
volatile int ctr = 0;
#endif
void setup() {

  cli();
  #if ENABLE_PRINTS
  Serial.begin(230400);
  #endif
  // Pin 2 / PD2 sig  on the arduino is input
  DDRD = DDRD & PD2_bm2;
  //This pin4/PD4 controls the rf switch
  DDRD = DDRD | PD4_bm1;

 
#if ARDUINO_INTERRUPTS
  attachInterrupt(digitalPinToInterrupt(sig), INT_ISR, CHANGE);
#else
  EIMSK &= ~(1 << INT0);               /* disable INT0 while we configure */
  EICRA = 0;
  EICRA |= (1 << ISC00);                /* trigger INT0 when pin 2 changes */
  EIMSK |= (1 << INT0);                 /* enable INT0 */    
#endif 
#if ENABLE_PRINTS
  timer1_init();
#endif
  
  sei();

  
}
#if ARDUINO_INTERRUPTS
void INT_ISR()
#else
ISR(INT0_vect)
#endif
{
  state = PIND & PD2_bm1;
  if (state != prev_state)
    {

      #if ENABLE_PRINTS
      ti = TCNT1;
      flag=1;  
      #endif
      
      prev_state = state;
      if (state==0)
        {
          PORTD = PORTD | PD4_bm1;
          #if ENABLE_PRINTS
          TCNT1 = 0;
          gtime[0] = ti;
          #endif     
        }
      else
        {
          PORTD = PORTD & PD4_bm2;
          #if ENABLE_PRINTS
          TCNT1 = 0;
          gtime[1] = ti;
          #endif
        }
    }
  
  
}

// This sets the timer1 to count every 16 us. 
void timer1_init()
{
  cli();
    // set up timer with no prescaling
    TCCR1A = 0;
    //prescaler = 64
    TCCR1B = (0 << CS12)| (1<<CS11)| (1<<CS10);
/*
CS12 CS11 CS10 Description
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkI/O/1 (no prescaling)
0 1 0 clkI/O/8 (from prescaler)
0 1 1 clkI/O/64 (from prescaler)
1 0 0 clkI/O/256 (from prescaler)
1 0 1 clkI/O/1024 (from prescaler)
*/
    // initialize counter
    TCNT1 = 0;
    
    sei();
}


void loop()
{

while(1)
{
#if ENABLE_PRINTS
   if (flag == 1)
  {
    ctr++;
    //print timings over serial once every 2000 interrupts
    if (ctr > 2000)
    {
      //Multiply value by 4e-6 to get time in seconds;
    Serial.println(gtime[f_idx]);
    ctr = 0;
    flag = 0;
    f_idx++;
    f_idx &= 1;
    }
  }
#endif
}
}
