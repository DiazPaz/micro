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

unsigned char patrones1[8] = {0x10,0x28,0x28,0x28,0x28,0x7C,0x54,0x7C};
unsigned char patrones4[8] = {0x0,0xE0,0xBE,0xE1,0xBE,0xE0,0x0,0x0};
unsigned char patrones5[8] = {0x3E,0x2A,0x3E,0x14,0x14,0x14,0x14,0x8};
unsigned char patrones6[8] = {0x0,0x0,0x7,0x7D,0x87,0x7D,0x7,0x0};

unsigned char patrones2[8] = {0xF0,0x88,0x84,0x84,0x84,0x84,0x88,0xF0};
unsigned char patrones3[8] = {0x81,0x81,0xC3,0x42,0x42,0x66,0x24,0x18};

unsigned char patrones2[8] = {0xF0,0x88,0x84,0x84,0x84,0x84,0x88,0xF0};
unsigned char patrones3[8] = {0x81,0x81,0xC3,0x42,0x42,0x66,0x24,0x18};      

void main(void)
{
    TRISB = 0; 
    TRISD = 0; 
    unsigned int aux = 50; 
    
    while(1)
    {
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i; 
                PORTD = ~patrones2[i];
                __delay_ms(1);
            }
        }
        aux = 50;
        
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i; 
                PORTD = ~patrones3[i];
                __delay_ms(1);
            }
        }
        aux = 50;
        
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i; 
                PORTD = ~patrones1[i];
                __delay_ms(1);
            }
        }
        aux = 50; 
        
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i;  
                PORTD = ~patrones4[i];
                __delay_ms(1);
            }
        }
        aux = 50; 
        
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i; 
                PORTD = ~patrones5[i];
                __delay_ms(1);
            }
        }
        aux = 50; 
        
        while(aux--)
        {
            for(unsigned char i = 0; i < 8; i++)
            {
                PORTB = 1 << i; 
                PORTD = ~patrones6[i];
                __delay_ms(1);
            }
        }
        aux = 50; 
        
    }
}

