/* 
 * File:   main.c
 * Author: daizp
 *
 * Created on June 17, 2025, 4:48 PM
 */

#include <xc.h>         // Biblioteca principal del compilador XC8
#include <stdint.h>     // Para tipos como uint8_t y int16_t
#include <stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include "lcd.h"

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

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7 }; //PORT, RS, EN, D4, D5, D6, D7

volatile unsigned int timer = 0; 
volatile unsigned int count = 0; 
char exec[6];

void TMR0_init(void)
{
    OPTION_REG = 0x07;
    TMR0 = 217; 
    T0IE = 1; 
    GIE = 1; 
}

void __interrupt() ICR(void)
{
    if(T0IF)
    {
        count++; 
        if(count > 100)
        {
            timer++; 
            count = 0; 
        }   
        TMR0 = 217;
        T0IF = 0; 
    }
}

void main(void)
{
    TMR0_init();
    LCD_Init(lcd);
    
    LCD_Clear();
    
    LCD_Set_Cursor(0,0);
    LCD_putrs("Tiempo:");
    
    LCD_Set_Cursor(1,0);
    LCD_putrs("00:00");
    
    while(1)
    {
        LCD_Set_Cursor(1,0);
        sprintf(exec,"%02u:%02u",timer/60,timer%60);
        LCD_puts(exec);
    }
}