
#include "tm4c123gh6pm.h"
#include "slider.h"


void ADC0_Init_portE(void)
{ 
	volatile unsigned long delay;  
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) activate clock for Port E
	while((SYSCTL_PRGPIO_R&0x10) == 0){}; // allow time for clock to start. Wait for status bit to be true
		
	GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
	GPIO_PORTE_AFSEL_R &= ~0x04;     // 3) enable alternate function on PE2
	GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
	GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2		

	SYSCTL_RCGCADC_R |= 0x01;   // 6) activate ADC0
	// These 4 delay statements allow the clock time to stabilize. Simulation did not work without them.
	delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
	delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
	delay = SYSCTL_RCGCADC_R;       // extra time to stabilize
	delay = SYSCTL_RCGCADC_R;				
			
	// swapped to new register name ADC0_PC_R.

	ADC0_PC_R = 0x01;               // 7) configured with 125K 
	ADC0_SSPRI_R = 0x0123;          
	ADC0_ACTSS_R &= ~0x0008;        
	ADC0_EMUX_R &= ~0xF000;         
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1; // 11) channel Ain1 (PE2)
	ADC0_SSCTL3_R = 0x0006;         
	ADC0_ACTSS_R |= 0x0008;         
}


//------------ADC0_InSeq3------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned int volatile result;
void ADC0_Update(void)
{  
  ADC0_PSSI_R = 0x0008;            
  while((ADC0_RIS_R&0x08)==0){};   
  result = ADC0_SSFIFO3_R&0xFFF;   
  ADC0_ISC_R = 0x0008;             
 
}

int slide_meter (void)
{
	if (result > 2200)
		return right;
			else if (result < 1900)
				return left;
			else 
				return 0;
}
	
void Button_Init_portE(void)
{
	SYSCTL_RCGCGPIO_R |= 0x10;     // 1) activate clock for Port E
	while((SYSCTL_PRGPIO_R&0x10) == 0){}; // allow time for clock to start. Wait for status bit to be true
	GPIO_PORTE_DIR_R &= ~0x01;   // make PE0 input
	GPIO_PORTE_AFSEL_R &= ~0x01;// disable alt funct on PE0
	GPIO_PORTE_DEN_R |= 0x01;   // enable digital I/O on PE0
	GPIO_PORTE_PCTL_R = (GPIO_PORTE_PCTL_R&0xFFFFFFF0)+0x00000000;  // configure PE0 as GPIO
	GPIO_PORTE_AMSEL_R &= ~0X01;     // disable analog functionality on PB7-0
		
}
	
int button_logic(void)
{
	return (GPIO_PORTE_DATA_R&0x01);		
}
	
//void Shoot(void)
///{
	//spaceshiplaser.x = SpaceShip.x + SpaceShip.w/2;
	//spaceshiplaser.y = SpaceShip.y;
	//spaceshiplaser.life = alive;
	//spaceshiplaser.needDraw = 1;
//}

