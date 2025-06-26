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

unsigned int vidas = 3;
unsigned int nivel = 0; 
uint8_t pos_x = 9;
uint8_t fondo[20];       // Fondo fila 2

void pantalla_inicio(void);
void __nivel_1(void);

void generar_fondo(void);
void dibujar_fondo(void);
void disparo_aliens(void);
void pot_x(void);
void checar_boton(void);

void ADC_Init();
unsigned int ADC_Read(unsigned char channel);
void __interrupt() ISR(void);

void LCD_custom_char(uint8_t pos, const char * x);


const char nave[8] = {
    0b00000100,
    0b00001110,
    0b00001110,
    0b00001010,
    0b00011011,
    0b00011111,
    0b00010101,
};

const char alien_grande[8] = {
    0b00010001,
    0b00001110,
    0b00011111,
    0b00010101,
    0b00011111,
    0b00011111,
    0b00001010,
};

const char alien_chico[8] = {
    0b00001010,
    0b00011111,
    0b00000000,
    0b00011111,
    0b00010101,
    0b00011111,
    0b00001010,
};

const char medio_corazon[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00010101,
    0b00011111,
    0b00011111,
    0b00011111,
};

const char corazon[8] = {
    0b00011111,
    0b00011111,
    0b00011111,
    0b00011111,
    0b00011111,
    0b00011111,
    0b00011111,
};

void main(void){
    ANSEL = 0;
    ANSELH = 0;
    OPTION_REG &= 0b01111111; // Pull-ups activados
    
    TRISB = 0xFF; // entradas 
    PORTB = 0;
    
    INTF = 0;
    INTEDG = 0; // Flanco de bajada
    INTE = 1;   // Interrupción externa (RB0)
    GIE = 1;    // Interrupciones globales
    
    LCD_Init(lcd);
    LCD_Clear();
    
    LCD_custom_char(0, nave);
    LCD_custom_char(1, alien_grande);
    LCD_custom_char(2, alien_chico);
    LCD_custom_char(3, corazon);
    LCD_custom_char(4, medio_corazon);
    LCD_custom_char(5, nave);
    
    srand(time(NULL));
    
    while (1) {
        unsigned int boton1 = !PORTBbits.RB0, bandera = 0;
                
        switch(nivel)
        {
            case 0: 
                pantalla_inicio();
                break;
            case 1:
                __nivel_1();
                break;
            default:
                break;
        }           
    }
}

void pantalla_inicio(void){
    LCD_Clear();
    LCD_Set_Cursor(0,3);
    LCD_putrs("Los Invasores");
    LCD_Set_Cursor(1,4);
    LCD_putrs("De Nuevo Leon");
    for(unsigned char i = 8; i < (8+3); i++){
        LCD_Set_Cursor(2, i);
//        __delay_ms(200);
        LCD_putc(i);
    }
    for(unsigned char j = 0; j < 3; j++){
//        __delay_ms(25);
        LCD_Set_Cursor(3, 7);
        LCD_putrs("Inicio");
//        __delay_ms(100);
    }
    LCD_Set_Cursor(3, 7);
    LCD_putrs("           ");
    LCD_Set_Cursor(3, 3);
    LCD_putrs("Pulsa el boton");
//    __delay_ms(500);
}

void __nivel_1(void){
    LCD_Clear();
    
    uint8_t cantidadAliens = rand() % 15 + 1;
    
    for(unsigned int i = 0; i < cantidadAliens; i++)
    {
        uint8_t posAliens = rand() % 20;
        LCD_Set_Cursor(0,posAliens);
        uint8_t tipoAlien = rand() % 2 + 1;
        LCD_putc(tipoAlien);
    }
    
    // implementar ciclo para el juego
}

void disparo_aliens(void){
    for(uint8_t k = 0; k < 3; k++){
        for(uint8_t i = 0; i < 20; i++){
            if(fondo[i] == 1){
                // Alien grande
                for(uint8_t j = 1; j <= 3; j++){
                    LCD_Set_Cursor(j, i);
                    LCD_putc('!');
                    __delay_ms(5);
                    LCD_Set_Cursor(j, i);
                    LCD_putrs(" ");
                }
            }
            else if(fondo[i] == 2){
                // Alien chico
                for(uint8_t j = 1; j <= 3; j++){
                    LCD_Set_Cursor(j, i);
                    LCD_putc(':');
                    __delay_ms(5);
                    LCD_Set_Cursor(j, i);
                    LCD_putrs(" ");
                }
            }
        }
    }
}


void pot_x(void){
    unsigned int adc_x = ADC_Read(0);

    LCD_Set_Cursor(3, pos_x);
    LCD_putrs(" ");

    if(adc_x > 600){
        pos_x = (pos_x < 19) ? pos_x + 1 : 0;
    } else if(adc_x < 400){
        pos_x = (pos_x > 0) ? pos_x - 1 : 19;
    }

    LCD_Set_Cursor(3, pos_x);
    LCD_putc(0);
}

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

void __interrupt() ISR(void){
    if (INTF) {
        GIE = 0;

        nivel++;

        INTF = 0;
        GIE = 1;
    }
}

void LCD_custom_char(uint8_t pos, const char * x){
    pos = (pos & 0x07) << 3;
    LCD_cg_ram_addr(pos);
    for(uint8_t i = 0; i < 8; i++){
        LCD_ram_write(x[i]);
    }
}