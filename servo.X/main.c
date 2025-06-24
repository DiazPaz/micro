#include <xc.h>         

// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN
#pragma config FOSC = XT        
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = ON       
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

#define _XTAL_FREQ 500000      

void PWM_init(void)
{
    OSCCONbits.SCS = 1; // enable internal oscillator
    OSCCONbits.IRCF = 0b011; // internal oscillator frequency 500kHz
    
    CCP1CONbits.CCP1M = 0b1100; // PWM mode; P1A, P1C active-high; P1B, P1D active-high
    CCP1CONbits.P1M = 0b00; // 00 = Single output; P1A modulated; P1B, P1C, P1D assigned as port pins
    
    // Timer2
    T2CONbits.T2CKPS = 0b11; // prescaler 16
    T2CONbits.TMR2ON = 1; // enable timer 2
    
    PR2 = 155;       
                    // El periodo PWM = T_osc * (PR2 + 1) * 4 * prescaler
                    // recordamos que T_osc = 1 / F_osc
                    // Con un PR2 = 255 se obtiene un periodo de 1.024ms 
                    // PR2 es un parametro de 8 bits
}

void PWM_val(unsigned char intensidad)
{
    CCPR1L = intensidad; 
}

void main(void)
{
    TRISCbits.TRISC2 = 0; 
    PWM_init();
    
    while(1)
    {
        for(unsigned int aux = 5; aux <= 20; aux++)
        {
            PWM_val(aux);  
            __delay_ms(20);
        }
        __delay_ms(2000);
        for (unsigned int aux = 20; aux >= 5; aux--) 
        {
            PWM_val(aux);
            __delay_ms(20);
        }
        __delay_ms(2000);
    }
}