

#ifndef slider_h
#define slider_h
#define right 1
#define left 2 



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode
void ADC0_Init_portE(void);

//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
void ADC0_Update(void);

int slide_meter(void);

void Button_Init_portE(void);

int button_logic(void);

void Shoot(void);

#endif
