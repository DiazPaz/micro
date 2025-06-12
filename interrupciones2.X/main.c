/* 
 * Microprocesadores
 * Archivo:   main.c
 * Autor: Alejandro Omar Reyes Guía
 * Práctica: 6
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

unsigned char digito[10] = {
0x3f, //0
0x6, //1
0x5b, //2
0x4f, //3
0x66, //4
0x6d, //5
0x7d, //6
0x7, //7
0x7f, //8
0x6f //9
};

int num = 0, dec = 0, aux = 0; 
    
void math(){
    num = (num + 1) % 10; 
    aux++;

    dec = aux / 10;
    if (dec >= 10) {
        aux = 0;
        dec = 0;
    }
}

void main(void){
    // Cambiar entradas analógicas por pines digitales
    ANSELH = 0;
    ANSEL = 0;
    
    // Habilitar resistencias de pull up del puerto B
    OPTION_REG &= 0b01111111;
    
    // Declarar entradas y salidas
    TRISC = 0;
    TRISD = 0;
    TRISB = 0XFF;
    
    // Enviar valor baho a puertos de salida
    PORTD = 0;
    PORTC = 0;
    
    unsigned char count = 0;
    
    // Declarar interupciones
    GIE = 1; // Habilitar interrupciones globales (tanto software como hardware)
    INTE = 1; // Habilitar interrupciones externas, RB0
    INTEDG = 0; // Declarar la activación por flanco de bajada (0 para flanco de bajada, 1 para flanco de subida)
    while(1){
            // Display mil
            PORTC = 0b11110111;
            PORTD = digito[dec];
            __delay_ms(1);
            
            // Display centena
            PORTC = 0b11111011;
            PORTD = digito[num];
            __delay_ms(1);
    }
}

void __interrupt() ISR(void){
    if(INTF){
        GIE = 0; // Deshabilita las interrupciones globales
        // Aquí ponemos las funciones que se ejecutan en la interrupcion
        math();
        GIE = 1; // Activar las interrupciones globales otra vez
        INTF = 0; // Resetear la bandera de interrupción
    }
}
