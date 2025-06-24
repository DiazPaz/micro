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

#define _XTAL_FREQ 4000000      

void PWM_init(void)
{
    CCP1CONbits.CCP1M = 0b1100; 
    CCP1CONbits.P1M = 0b00;
    
    // Timer2
    T2CONbits.T2CKPS = 0b11; 
    T2CONbits.TMR2ON = 1; 
    
    PR2 = 255;      // El periodo PWM = T_osc * (PR2 + 1) * 4 * prescaler
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
        for(unsigned char i = 255; i > 0; i--)
        {
            PWM_val(i);
            __delay_ms(20);
        }
        __delay_ms(20);
        for(unsigned char i = 0; i < 255; i++)
        {
            PWM_val(i);
            __delay_ms(20);
        }
    }
}