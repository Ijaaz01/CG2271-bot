#include "MKL25Z4.h"                    // Device header
#define PTB0_Pin  0
#define PTB1_Pin  1
#define LED_RED 18
#define MASK(x) 1 << x

/* initPWM() */
void initPWM(void)
{
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(1);

    PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(1);
	
		PORTB->PCR[LED_RED] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[LED_RED] |= PORT_PCR_MUX(1);

		PTB->PDDR |= (MASK(PTB0_Pin) | MASK(PTB1_Pin) | MASK(LED_RED));
		PTB->PDOR |= (MASK(PTB0_Pin) | MASK(PTB1_Pin));
		PTB->PDOR &= ~(MASK(LED_RED));
}

void initUART(int BR) {
	int divisor;
	SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
	PORTE->PCR[22] = PORT_PCR_MUX(4);
	PORTE->PCR[23] = PORT_PCR_MUX(4);
 // ensure tx and rx are disabled before configuration
	UART2->C2 &=  ~(UARTLP_C2_TE_MASK | UARTLP_C2_RE_MASK);
	divisor = 20971520/(BR*16);
	UART2->BDH = UART_BDH_SBR(divisor>>8);
	UART2->BDL = UART_BDL_SBR(divisor);
	 // No parity, 8 bits, two stop bits, other settings;
	UART2->C1 = UART2->S2 = UART2->C3 = 0;
 // Enable transmitter and receiver
	UART2->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK; 
}

uint8_t UART2_Receive_Poll(void) {
 // wait until receive data register is full
 while (!(UART2->S1 & UART_S1_RDRF_MASK));
 return UART2->D;
 } 

int main(void) {	
	initPWM();
	initUART(9600);
	int LED = 0;
	while (1) {
		LED = UART2_Receive_Poll();
		if (LED == 2) { 
			PTB->PCOR |= MASK(LED_RED);
		} 
		else {
			PTB->PSOR |= MASK(LED_RED);
		}
	}
}