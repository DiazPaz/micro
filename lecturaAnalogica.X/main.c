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

#define _XTAL_FREQ 4000000

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7 }; //PORT, RS, EN, D4, D5, D6, D7

void ADC_initialize()
{
    ANSEL = 0x01; 
    ADCON0 = 0x81; 
    ADCON1 = 0x80;
}

unsigned int ADC_read()
{
    __delay_us(5);  // da tiempo al capacitor de la lectura analógica del PIC a que se cargue
    GO_nDONE = 1;   // 
    while(GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void main()
{
    ADC_initialize();
    LCD_Init(lcd);
    
    char buffer[10];
    
    while(1)
    {
        LCD_Clear();
        LCD_Set_Cursor(0,0);
        LCD_putrs("Voltaje: ");
        
        LCD_Set_Cursor(0,9);
        
        // Lectura analógica No optimizada
//        unsigned int adc_result = ADC_read();
//        unsigned float voltaje = (adc_result * 5.0) / 1023.0;
//        sprintf(buffer, "%0.1f", voltaje);
//        LCD_putrs(buffer);
//        __delay_ms(200);
        
        // Lectura analógica Optimizada
        unsigned int adc_result = ADC_read();
        unsigned int voltaje = (adc_result * 50000) / 10230;
        unsigned int part_ent = voltaje/1000; 
        unsigned int part_dec = voltaje%1000;
        
        sprintf(buffer, "%u.%u", part_ent, part_dec);
        LCD_putrs(buffer);
        __delay_ms(200);

    }
}