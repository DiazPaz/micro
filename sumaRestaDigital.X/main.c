
/* 
 * Microprocesadores
 * Archivo:   main.c
 * Autor: Alejandro Omar Reyes Guía
 * Práctica: 1
 * Descripción: LED Parpadeante. 
 *
 * Created on 18 de mayo de 2023, 06:41 PM
 */
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
};

void main(void)
{
    // Pasar de entradas analógicas a puertos digitales I/O
    ANSELH = 0; 
    
    // Configurar las resistencias Pull-Up internas del puerto B
    OPTION_REG &= 0b01111111; 
    
    // Declarar las entradas y salidas digitales
    TRISB = 0xFF; // 0xFF = 11111111 hex -> bin 
    TRISD = 0; 
    PORTD = 0;
    
    // Configurar el funcionamiento de los botones
    unsigned int cont = 0; 
    unsigned int a = 0, b = 0, c = 0;
    unsigned int aux1 = !PORTBbits.RB0; 
    unsigned int aux2 = !PORTBbits.RB1;
    while(1)
    {
        if(aux1)
            cont++;

        if(aux2)
        {
            if(cont > 0)
                cont--;
            else
                cont = 9;
        }   
        
        cont %= 10;
        PORTD = digito[cont];
        
        __delay_ms(20);
    }
}
