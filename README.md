# radar_ra_fall21
 RA work for fall 2021
Video: https://www.youtube.com/watch?v=W1xQdVQF4TQ&ab_channel=GauravDuggal

Pulse width measurement - with a microcontroller.

/* This code uses the INT O hardware interrupt and timer1 in the atmega 328 to measure the pulse high and low time
 *  It is assumed the high time corresponds to the tx pulse duration + setup time. This is generally used to control
 *  the rx-tx switch in a radar. This code also generates a NOT signal on PD4 (pin 7 ) to protect the secondary rx array
 *  from the high tx transmit power. pin 7 is toggled within 5 us of the rx-tx switch being toggled.
 *  NOTE: Multiple the readings outputted by the serial port by 4us to get the on and off time in seconds.
 */
