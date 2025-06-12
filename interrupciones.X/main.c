/* 
 * Microprocesadores
 * Archivo:   main.c
 * Autor: Alejandro Omar Reyes Gu�a
 * Pr�ctica: 1
 * Descripci�n: LED Parpadeante. 
 *
 * Created on 18 de mayo de 2023, 06:41 PM
 */
#include <xc.h>         // Biblioteca principal del compilador XC8

//=============================================================================
// CONFIGURACI�N DE BITS DE CONFIGURACI�N (FUSES)
//=============================================================================

// Selecci�n de oscilador (usar XT si est�s usando un cristal de 4 MHz)
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
#define LED PORTCbits.RC0

unsigned char digito[10] = {
0x3F, 
0x6,
0x5B,
0x4F,
0x66,
0x6D,
0x7D,
0x7,
0x7F,
0x6F
}; // 0,1,2,3,4,5,6,7,8,9,A,b,C,d,E,F

void main(void)
{
    // Cambiamos las entradas anal�gicas por pines digitales
    ANSEL = 0;
    ANSELH = 0;
    
    // Habilitar las resistencias de pull up
    OPTION_REG &= 0b01111111;
    
    // Declarar entradas y salidas 
    TRISC = 0; // salida
    TRISD = 0; // salida
    TRISB = 0xFF; // entrada
    
    // Se inicializan los puertos en valor bajo
    PORTC = 0; 
    
    unsigned char cont = 0; 
    
    // Declaramos interrupciones
    GIE = 1;  // Habilitar las interrupciones globales; se prioriza la interrupci�n sobre todo (excepto por el master reset)
    INTE = 1; // Habilitar las interrupciones externas, RB0
    INTEDG = 0; // Activaci�n por flancos:  0 para flanco de bajada y 1 para flanco de subida 
    
    while(1)
    {
        PORTD = digito[cont];
        __delay_ms(100);
        cont = (cont + 1)%10; 
    }
}

void blink_LED()
{
    for(int i = 0; i < 4; i++)
    {
        LED = 1;
        __delay_ms(200);
        LED = 0; 
        __delay_ms(200);
    }
}

void __interrupt() ISR(void)
{
    if(INTF) // bandera del PIC, se hace uno cuando detecta una interrupci�n externa
    {
        GIE = 0; // Deshabilitas interrupciones porque las interrupciones tamb se pueden interrumpir LOL 
        blink_LED();
        GIE = 1; 
        INTF = 0; 
    }
}