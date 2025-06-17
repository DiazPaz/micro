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

#define _XTAL_FREQ 4000000      // Frecuencia del oscilador (para __delay_ms y __delay_us)

void main(void)
{
    // Pasar de entradas analógicas a puertos digitales I/O
    ANSELH = 0; 
    
    // Configurar las resistencias Pull-Up internas del puerto B
    OPTION_REG = OPTION_REG & 0b01111111; 
    
    // Declarar las entradas y salidas digitales
    TRISB = 0xFF; // 0xFF = 11111111 hex -> bin 
    TRISD = 0; 
    PORTD = 0;
    
    // Configurar el funcionamiento de los botones
    unsigned int a = 0, b = 0, c = 0;
    while(1)
    {
        unsigned int aux1 = !PORTBbits.RB0; 
        unsigned int aux2 = !PORTBbits.RB1;
        unsigned int aux3 = !PORTBbits.RB2;
        
        if(aux1 == 1)
            a = !a;
        if(aux2 == 1)
            b = !b;
        if(aux3 == 1)
            c = !c;
        
        // 000000000111111110000000 a
        // 100000000100000001000000 aux
        
        __delay_ms(50);
        
        PORTDbits.RD0 = a; 
        PORTDbits.RD1 = b;
        PORTDbits.RD2 = c;
        
    }
}