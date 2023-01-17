#ifndef PeriodicSysTickInts_h
#define PeriodicSysTickInts_h
//                                     Images
//
// It seems that images are saved in reverse; i.e. top row is actually the bottom row of pixels.
//
// See end of Section 15.3.2. Buffer-based Graphics on the Nokia 5110 discussing Figure 15.7.
// on web page: Chapter 15: Complete Embedded System, Modified to be compatible with EE319K Lab 10
// http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C15_CompleteEmbeddedSystem.htm
// The 2-D image is stored in row-major format. The image is stored up-side down. 
// When plotting it on the screen the Nokia5110_PrintBMP() function will reverse it so it is seen right-side up.

// Extern -> extend visibility of a function throughout the whole program

// Black Enemy is all zeros.
extern void SysTick_Init(unsigned long period);
// Black Laser is all zeros.
extern void SysTick_Handler(void);

#endif
