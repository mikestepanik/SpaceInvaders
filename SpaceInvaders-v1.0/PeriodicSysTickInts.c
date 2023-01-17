// April 5, 2022
// CPE 355


// PeriodicSysTickInts.c
// Runs on LM4F120 or TM4C123
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano
// September 14, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1, Program 9.6
   
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Volume 2, Program 5.12, section 5.7

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

// oscilloscope or LED connected to PF2 for period measurement


#include "tm4c123gh6pm.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode


volatile unsigned long Counts = 0;


// **************SysTick_Init*********************
// Initialize SysTick periodic interrupts
// Input: interrupt period
//        Units of period are 62.5ns (assuming 16 MHz clock)
//        Maximum is 2^24-1
//        Minimum is determined by length of ISR
// Output: none
void SysTick_Init(unsigned long period){
  NVIC_ST_CTRL_R = 0;         // disable SysTick during setup
  NVIC_ST_RELOAD_R = period-1;// reload value
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
                              // enable SysTick with core clock and interrupts
  NVIC_ST_CTRL_R = 0x07;
  EnableInterrupts();	// make I = 0
}


// Interrupt service routine
// Executed every 62.5ns*(period)
void SysTick_Handler(void){
  GPIO_PORTF_DATA_R ^= 0x04;       // toggle PF2
  Counts = Counts + 1;
}


int main(void){
  Counts = 0;
	SYSCTL_RCGCGPIO_R |= 0x00000020;	// (a) activate clock for Port F, bit 5
	while((SYSCTL_PRGPIO_R&0x00000020) == 0){}; // allow time for clock to start. Wait for status bit to be true
  GPIO_PORTF_DIR_R |= 0x04;   // make PF2 output (PF2 built-in LED)
  GPIO_PORTF_AFSEL_R &= ~0x04;// disable alt funct on PF2
  GPIO_PORTF_DEN_R |= 0x04;   // enable digital I/O on PF2
                              // configure PF2 as GPIO
  GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0xFFFFF0FF)+0x00000000;
  GPIO_PORTF_AMSEL_R = 0;     // disable analog functionality on PF
  //SysTick_Init(16000);        // initialize SysTick timer. 1ms interrupts. Period = 2 ms. LED flashes too fast to see with the eye.
  SysTick_Init(3200000);      // initialize SysTick timer. LED toggles every 200 ms to see with eye. Period = 400 ms.
  //SysTick_Init(8000000);      // initialize SysTick timer. LED toggles every 500 ms. Period = 1 sec; 1 Hz flash.
	
  EnableInterrupts();

  while(1){                   // interrupts every 1ms, period = 2 ms; 500 Hz flash.
    WaitForInterrupt();
  }
}

