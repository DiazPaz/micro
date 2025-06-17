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

void ADC_init(){
    ANSEL = 0x03;
    ANSELH = 0; 
    ADCON0 = 0x01;
    ADCON1 = 0x80;
}

unsigned int ADC_read(unsigned char channel)
{
    __delay_us(5);
    ADCON0 &= 0x83;
    ADCON0 |= channel << 2; 
    __delay_ms(2);
    GO_nDONE = 1; 
    while(GO_nDONE);
    return ((ADRESH<<8)+ADRESL);
}

int main(int argc, char** argv) 
{
    char buffer1[10], buffer2[10];
    ADC_init();
    LCD_Init(lcd);
    while(1)
    {
        LCD_Clear();
        LCD_Set_Cursor(0,0);
        LCD_putrs("Voltaje 1: ");
        LCD_Set_Cursor(0,11);
        
        unsigned int adc_result1 = ADC_read(0);
        unsigned int volt1 = (adc_result1 * 50000) / 1023;
        unsigned int ent1 = volt1 / 10000;
        unsigned int dec1 = volt1 % 10000;
        sprintf(buffer1, "%u.%u",ent1,dec1);
        LCD_putrs(buffer1);
        
        LCD_Set_Cursor(1,0);
        LCD_putrs("Voltaje 2: ");
        LCD_Set_Cursor(1,11);
        unsigned int adc_result2 = ADC_read(1);
        unsigned int volt2 = (adc_result2 * 50000) / 1023;
        unsigned int ent2 = volt2 / 10000;
        unsigned int dec2 = volt2 % 10000;
        sprintf(buffer2, "%u.%u",ent2,dec2);
        LCD_putrs(buffer2);
        
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}