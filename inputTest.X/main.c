#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN (FUSES)
//=============================================================================

// Selección de oscilador (usar XT si estás usando un cristal de 4 MHz)
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

//=============================================================================
// DEFINICIONES
//=============================================================================

#define _XTAL_FREQ 4000000

int password[4] = {1,0,1,0};
int userInput[4];

int correct; 

int confirm(void);
void confirmLED(void);
void offLED(void);
void __interrupt() ISR(void);

void main(void)
{
    // Pasar de entradas analógicas a puertos digitales I/O
    ANSELH = 0;     
    OPTION_REG &= 0b01111111; 
    TRISB = 0xFF; 
    TRISC = 0;
    
    PORTC = 0; 
    
    GIE = 1; // Habilitar interrupciones globales (tanto software como hardware)
    INTE = 1; // Habilitar interrupciones externas, RB0
    INTEDG = 0; 
    
    while(1)
    {
        if(!PORTBbits.RB1)
        {
            userInput[0] = 1; 
            PORTCbits.RC0 = 1; 
        }
            
        if(!PORTBbits.RB2)
        {
            userInput[1] = 1; 
            PORTCbits.RC1 = 1; 
        }
            
        if(!PORTBbits.RB3)
        {
            userInput[2] = 1; 
            PORTCbits.RC2 = 1; 
        }
            
        if(!PORTBbits.RB4)
        {
            userInput[3] = 1; 
            PORTCbits.RC3 = 1; 
        }
            
    }
    
}

int confirm(void)
{
    for(int i = 0; i < 4; i++)
        if(password[i] != userInput[i])
            return 0;
    return 1;
}

void confirmLED(void)
{
    PORTC = 0b11110000;
    __delay_ms(100);

    PORTC = 0b11111111; 
    __delay_ms(100);

    PORTC = 0b11110000;
    __delay_ms(100);

    PORTC = 0b11111111; 
    __delay_ms(100);

    PORTC = 0b11110000;
    __delay_ms(100);

    PORTC = 0b11111111; 
    __delay_ms(100);
}

void offLED(void)
{
    PORTC = 0;
}

void __interrupt() ISR(void){
    if(INTF){
        GIE = 0; // Deshabilita las interrupciones globales
        correct = confirm();
        for(int i = 0; i < 4; i++)
            userInput[i] = 0;
        if(correct)
            confirmLED();
        offLED();
            
        GIE = 1; // Activar las interrupciones globales otra vez
        INTF = 0; // Resetear la bandera de interrupción
    }
}