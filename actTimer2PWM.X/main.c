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

void ADC_Initialize(){
    ANSEL = 0x01;
    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_read(){
    __delay_us(5); // Tiempo para que el capacitor interno se cargue
    GO_nDONE = 1;
    while(GO_nDONE);
    return(ADRESH << 8) + ADRESL;
}

void PWM_val(unsigned char intensidad)
{
    CCPR1L = intensidad; 
}

void main(void)
{
    unsigned int adc_result, aux;
    TRISCbits.TRISC2 = 0; 
    ADC_Initialize();
    PWM_init();
    while(1)
    {
        unsigned int adc_result = ADC_read(); 
        unsigned char aux = (unsigned char)((adc_result * 255UL) / 1023);

        PWM_val(aux);  // Esta función asume que el valor es de 8 bits
        __delay_ms(20);
    }

}