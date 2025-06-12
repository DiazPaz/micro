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

unsigned char digito[16] = {
0x3F, 
0x6,
0x5B,
0x4F,
0x66,
0x6D,
0x7D,
0x7,
0x7F,
0x6F,
0x77,
0x7C,
0x39,
0x5E,
0x79,
0x71,

}; // 0,1,2,3,4,5,6,7,8,9 

void main(void){
  TRISD = 0;
  TRISB = 0;
  unsigned char dec = 0, num = 0;
  while(1){
      PORTD = digito[num];
      PORTB = digito[dec];
      __delay_ms(20);
      num++;
      if(num > 15)
      {
          num = 0;
          dec++;
      }
      if(dec > 15)
          dec = 0; 
  }
}

