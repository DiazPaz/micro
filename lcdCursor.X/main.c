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

void LED1(void);
void LED2(void);
bool boton = false;


void main(void){
    
    ANSEL = 0;
    ANSELH = 0;
    OPTION_REG &= 0b01111111; // Pull-ups activados
    TRISB = 0b00000011;
    PORTB = 0;
    
    
    LCD_Init(lcd);
    while(1){
        if(!PORTBbits.RB1){
            boton = !boton;
            LCD_Set_Cursor(boton, 0);
            LCD_putrs(">");
            LCD_Set_Cursor(!boton, 0);
            LCD_putrs(" ");
        }
        LCD_Set_Cursor(0, 3);
        LCD_putrs("LED 1: ON");
        LCD_Set_Cursor(1, 3);
        LCD_putrs("LED 2: ON");
        
        __delay_ms(10);
    
        if(!PORTBbits.RB0){
            LCD_Clear();
            if(boton == false){
                LED1();
            } else{
                LED2();
            }
            LCD_Clear();
        }
    }
}

void LED1(void){
    LCD_Set_Cursor(0,0);
    LCD_putrs("8-- LED 1");
    LCD_Set_Cursor(1,0);
    LCD_putrs("   STATUS: ON");
    __delay_ms(3000);
}

void LED2(void){
    LCD_Set_Cursor(0,0);
    LCD_putrs("8-- LED 2");
    LCD_Set_Cursor(1,0);
    LCD_putrs("   STATUS: ON");
    __delay_ms(3000);
}