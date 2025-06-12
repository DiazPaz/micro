/* 
 * Microprocesadores
 * Archivo:   main.c
 * Autor: Alejandro Omar Reyes Guía
 * Práctica: 4
 * Descripción: Contador binario ascendente y descendente.
 *
 * Created on 18 de mayo de 2023, 06:41 PM
 */
#include <xc.h>         // Biblioteca principal del compilador XC8
#include <stdint.h>     // Para tipos como uint8_t y int16_t

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

unsigned char digito[10] ={0x3f, //0
0x6, //1
0x5b, //2
0x4f, //3
0x66, //4
0x6d, //5
0x7d, //6
0x7, //7
0x7f, //8
0x6f, //9

};

void main(void){
    TRISD = 0;
    TRISB = 0;
    
    int num = 0;
    int cent = 0;
    int mil = 0;
    
    while(1){
        int dec = num/10;
        int uni = num%10;
        
        
        // Display de numeros
        
        for(int i = 0; i < 2; i++){
            // Display mil
            if(mil>0)
            {
                PORTB = 0b11110111;
                PORTD = digito[mil];
                __delay_ms(1);
                PORTB = 0b11111011;
                PORTD = digito[cent];
                __delay_ms(1);
                PORTB = 0b11111101;
                PORTD = digito[dec];
                __delay_ms(1);
                // Display unidades
                PORTB = 0b11111110;
                PORTD = digito[uni];
                __delay_ms(1);
            }
            // Display centena
            else if(cent>0)
            {
                PORTB = 0b11110111;
                PORTD = digito[cent];
                __delay_ms(1);
                PORTB = 0b11111011;
                PORTD = digito[dec];
                __delay_ms(1);
                // Display unidades
                PORTB = 0b11111101;
                PORTD = digito[uni];
                __delay_ms(1);
            }
            //Mostrar las decenas
            else if(dec>0)
            {
                PORTB = 0b11110111;
                PORTD = digito[dec];
                __delay_ms(1);
                // Display unidades
                PORTB = 0b11111011;
                PORTD = digito[uni];
                __delay_ms(1);
            }
            // Display unidades
            else
            {
                PORTB = 0b11110111;
                PORTD = digito[uni];
                __delay_ms(1);
            }
            
        }
        num++;
        
        if(num == 100){
            cent++;
            num = 0;
        }
        if(cent == 10){
            mil++;
            cent = 0;
        }
        if(mil == 9 && cent == 9 && num == 9){
                num = 0;
                cent = 0;
                mil = 0;
        }
    }
}