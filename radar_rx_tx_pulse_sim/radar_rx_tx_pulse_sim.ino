////////////////////////////////////////////////////////////
/* Code written by Gaurav Duggal 06-12-2021               */
//Reference: Atmega 328 datasheet and Arduino Reference page
////////////////////////////////////////////////////////////
/*  This code generates the rx-tx switch control timing, the pulse is high for the duration the radar is transmitting
 *   and this is used to switch of the receiver during that duration to protect it from the high transmit power.
 *  It is assumed the high time corresponds to the tx pulse duration + setup time. This generates a 200us high pulse
 *  followed by 200ms 
 *  
*/
void setup() {
  // pin 2 and 7 configured as output in arduino
  DDRD = DDRD | (1<<PD4);
  DDRD = DDRD | (1<<PD7);
  
}

void loop() {
  // bypass the trash in void loop();
  while(1)
  {
   PORTD = PORTD | (1<<PD4) | (1<<PD7);
   delay(200);
   PORTD = PORTD & ~(1<<PD4) & ~(1<<PD7);
   delayMicroseconds(200);  
  }
}
