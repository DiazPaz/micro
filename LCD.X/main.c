/* 
 * File:   main.c
 * Author: daizp
 *
 * Created on June 12, 2025, 4:26 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (enabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection (disabled)
#pragma config WRT = OFF        // Flash Program Memory Write Enable (disabled)
#pragma config CP = OFF         // Flash Program Memory Code Protection (disabled)

#define _XTAL_FREQ 4000000  

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7}; // PORT, RS, EN, D4, D5, D6, D7 

int main(int argc, char** argv) 
{
    LCD_Init(lcd);
    while(1)
    {
        LCD_Clear();
        LCD_Set_Cursor(0,0);
        LCD_putrs("Hello World!");
        
        LCD_Set_Cursor(1,0);
        for(char c = 'A'; c < 'Q'; c++)
        {
            LCD_putc(c);
            __delay_ms(300);
        }
        __delay_ms(1000);
    }
    return (EXIT_SUCCESS);
}

