/* 
 * Microprocesadores
 * Archivo:   main.c
 * Autor: Alejandro Omar Reyes Guía
 * Práctica: 1
 * Descripción: Contador binario ascendente y descendente.
 *
 * Created on 18 de mayo de 2023, 06:41 PM
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
char buffer[10];
unsigned int adc_result, voltaje, part_ent, part_dec; 

void ADC_Initialize(){
    ANSEL = 0x01;
    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_read(){
    __delay_us(5); // Tiempo para que el capacitor interno se cargue
    GO_nDONE = 1;
    while(GO_nDONE);
    return(ADRESH << 8) + ADRESL;
}

void volt(void);
void porcentaje(void);
void mil(void);

void main(void){
    ADC_Initialize();
    LCD_Init(lcd);
    
    ANSEL = 0;
    ANSELH = 0;
    OPTION_REG &= 0b01111111; // Pull-ups activados
    TRISB = 0b00000011;
    PORTB = 0;
    
    unsigned int aux = 1;
    
    while(1){
        LCD_Clear();
        
        if(!PORTBbits.RB0)
        {
            aux++;
            while(!PORTBbits.RB0);
            if(aux > 3) aux = 1;
        }
            
        adc_result = ADC_read();
        switch(aux)
        {
            case 1: 
                volt();
                break; 
            case 2: 
                porcentaje(); 
                break; 
            case 3: 
                mil(); 
                break;  
        }
    }
}

void volt(void)
{
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Voltaje: ");
        
    voltaje = (adc_result * 50000) / 10230;
    part_ent = voltaje / 1000;
    part_dec = voltaje%1000;
    sprintf(buffer, "%u.%u", part_ent, part_dec);
    
    LCD_Set_Cursor(1,0);
    LCD_putrs(buffer);
    __delay_ms(200);
}

void porcentaje(void)
{
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Porcentaje: ");
    voltaje = (adc_result*1000000) / 102300;  // Resultado: 0 - 1000 (corresponde a 0.0% - 100.0%)
    part_ent = voltaje / 100;     // Parte entera del porcentaje (por ejemplo 74)
    part_dec = voltaje % 100;     // Parte decimal (por ejemplo 6 para 74.6)
    sprintf(buffer, "%u.%u", part_ent, part_dec);  // Agregamos símbolo de porcentaje

    LCD_Set_Cursor(1, 0);
    LCD_putrs(buffer);
    __delay_ms(200);
}


void mil(void)
{
    LCD_Set_Cursor(0, 0);
    LCD_putrs("Mil: ");
    sprintf(buffer, "%u", adc_result);
    
    LCD_Set_Cursor(1, 0);
    LCD_putrs(buffer);
    __delay_ms(200);
}