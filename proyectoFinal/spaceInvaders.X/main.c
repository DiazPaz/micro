/* 
 * File:   main.c
 * Author: daizp
 *
 * Created on June 24, 2025, 5:31 PM
 */

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "lcd.h"

//=============================================================================
// CONFIGURACI?N DE BITS DE CONFIGURACI?N (FUSES)
//=============================================================================
#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};

void pot_x(void);
void pot_y(void);
void checar_boton(void);
void disparar(void);

void ADC_Init(){
    ANSEL = 0x03;
    ANSELH = 0;
    ADCON1 = 0x80;
    ADCON0 = 0x01;
}

unsigned int ADC_Read(unsigned char channel){
    __delay_us(5);
    ADCON0 &= 0x83;
    ADCON0 |= channel << 2;
    __delay_ms(2);
    GO_nDONE = 1;
    while(GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

uint8_t pos_x = 8;
uint8_t pos_y = 2;

void main(void){
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 0;
    PORTAbits.RA3 = 0;
    
    ADC_Init();
    LCD_Init(lcd);

    LCD_Clear();
    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc('|'); // Bob

    while(1){
        pot_x();
        pot_y();
        checar_boton();
    }
}

//==================================================
// Movimiento horizontal (potenci?metro X)
//==================================================
void pot_x(void){
    unsigned int adc_x = ADC_Read(0);
    uint8_t prev_x = pos_x;

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putrs(" ");

    if(adc_x > 600){
        pos_x = (pos_x < 19) ? pos_x + 1 : 0;
    } else if(adc_x < 400){
        pos_x = (pos_x > 0) ? pos_x - 1 : 19;
    }

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc('|'); //Bob
    __delay_ms(10);
}

//==================================================
// Movimiento vertical (potenci?metro Y)
//==================================================
void pot_y(void){
    unsigned int adc_y = ADC_Read(1);

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putrs(" ");

    if(adc_y > 600 && pos_y > 0){
        pos_y--;
    } else if(adc_y < 400 && pos_y < 3){
        pos_y++;
    }

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc('|'); //Bob
    __delay_ms(10);
}

//==================================================
// Bot?n: animaci?n reversa
//==================================================
void checar_boton(void){
    if(!PORTAbits.RA2){
        disparar();
    }
}

void disparar(void){
    // Mostrar a Bob invertido
    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc('/'); // Bob_reves

    // Disparo hacia ambos lados
    for(uint8_t offset = 1; offset < 20; offset++){
        bool moved = false;

        // Derecha
        if(pos_x + offset < 20){
            LCD_Set_Cursor(pos_y, pos_x + offset);
            LCD_putc('-');
            moved = true;
        }

        // Izquierda
        if(pos_x >= offset){
            LCD_Set_Cursor(pos_y, pos_x - offset);
            LCD_putc('-');
            moved = true;
        }

        __delay_ms(20);

        // Borrar proyectiles despu?s de mostrar
        if(pos_x + offset < 20){
            LCD_Set_Cursor(pos_y, pos_x + offset);
            LCD_putrs(" ");
        }

        if(pos_x >= offset){
            LCD_Set_Cursor(pos_y, pos_x - offset);
            LCD_putrs(" ");
        }

        if(!moved) break; // Cuando no hay m?s espacio que recorrer
    }

    // Restaurar a Bob normal
    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc('|');
}