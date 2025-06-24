/* 
 * File:   main.c
 * Author: daizp
 *
 * Created on June 22, 2025, 9:09 PM
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
void mover_nubes(void);
void generar_fondo(void);
void dibujar_fondo(void);
void disparar(void);

const char bob[8] = {
    0b00001110,
    0b00011111,
    0b00001110,
    0b00011111,
    0b00010101,
    0b00001110,
    0b00001010,
};

const char suelo[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

const char casa1[8] = {
    0b00000001,
    0b00000011,
    0b00000111,
    0b00011111,
    0b00000111,
    0b00000111,
    0b00000110,
};

const char casa2[8] = {
    0b00010000,
    0b00011000,
    0b00011100,
    0b00011111,
    0b00011100,
    0b00011100,
    0b00001100,
};

const char bob_reves[8] = {
    0b00001010,
    0b00001110,
    0b00010101,
    0b00011111,
    0b00001110,
    0b00011111,
    0b00001110,
};

const char arbol[8] = {
    0b00000100,
    0b00001110,
    0b00001110,
    0b00011111,
    0b00000100,
    0b00000100,
    0b00000100,
};

const char nube[8] = {
    0b00000000,
    0b00000110,
    0b00001111,
    0b00011111,
    0b00011111,
    0b00001110,
    0b00000000,
};

const char flor[8] = {
    0b00001010,
    0b00011111,
    0b00011111,
    0b00001010,
    0b00000100,
    0b00000100,
    0b00000100,
};

void LCD_custom_char(uint8_t pos, const char * x){
    pos = (pos & 0x07) << 3;
    LCD_cg_ram_addr(pos);
    __delay_ms(2);
    for(uint8_t i = 0; i < 8; i++){
        LCD_ram_write(x[i]);
    }
}

void dibujar_suelo(void){
    LCD_custom_char(1, suelo);
    LCD_Set_Cursor(3, 0);
    for(uint8_t i = 0; i < 20; i++){
        LCD_putc(1);
    }
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

uint8_t pos_x = 8;
uint8_t pos_y = 2;

uint8_t fondo[20];       // Fondo fila 2
uint8_t nubes[3];        // Posiciones de nubes en fila 1

void main(void){
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 0;
    PORTAbits.RA3 = 0;

    ADC_Init();
    LCD_Init(lcd);
    LCD_Clear();

    // Cargar caracteres
    LCD_custom_char(0, bob);
    LCD_custom_char(1, suelo);
    LCD_custom_char(2, casa1);
    LCD_custom_char(3, casa2);
    LCD_custom_char(4, bob_reves);
    LCD_custom_char(5, arbol);
    LCD_custom_char(6, nube);
    LCD_custom_char(7, flor);

    srand(123); // Semilla fija para reproducibilidad (cambiar por time(NULL) si se quiere aleatorio en tiempo real)

    LCD_Clear();
    dibujar_suelo();
    generar_fondo();
    dibujar_fondo();

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc(0); // Bob

    while(1){
        pot_x();
        pot_y();
        checar_boton();
        mover_nubes();
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

    if((prev_x == 0 && pos_x == 19) || (prev_x == 19 && pos_x == 0)){
        generar_fondo();
        dibujar_fondo();
    }

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc(0);
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
    } else if(adc_y < 400 && pos_y < 2){
        pos_y++;
    }

    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc(0);
    __delay_ms(10);
}

//==================================================
// Bot?n: animaci?n reversa
//==================================================
void checar_boton(void){
    if(PORTAbits.RA2 == 1){
        disparar();
    }
}

//==================================================
// Animaci?n de nubes
//==================================================
void mover_nubes(void){
    for(uint8_t i = 0; i < 3; i++){
        LCD_Set_Cursor(1, nubes[i]);
        LCD_putrs(" ");
        nubes[i] = (nubes[i] == 0) ? 19 : nubes[i] - 1;
        LCD_Set_Cursor(1, nubes[i]);
        LCD_putc(6);
    }
    __delay_ms(250);
}

//==================================================
// Generaci?n aleatoria del fondo
//==================================================
void generar_fondo(void){
    for(uint8_t i = 0; i < 20; i++){
        fondo[i] = ' '; // Espacio vac?o por defecto
    }

    for(uint8_t i = 0; i < 3; i++){
        uint8_t p = rand() % 20;
        fondo[p] = 5; // ?rbol
    }

    for(uint8_t i = 0; i < 3; i++){
        uint8_t p = rand() % 20;
        fondo[p] = 7; // Flor
    }

    for(uint8_t i = 0; i < 2; i++){
        uint8_t p = rand() % 19;
        fondo[p] = 2;     // Casa izq
        fondo[p + 1] = 3; // Casa der
    }

    // Nubes iniciales
    for(uint8_t i = 0; i < 3; i++){
        nubes[i] = rand() % 20;
    }
}

//==================================================
// Dibuja el fondo fijo
//==================================================
void dibujar_fondo(void){
    for(uint8_t i = 0; i < 20; i++){
        LCD_Set_Cursor(2, i);
        if(fondo[i] != ' '){
            LCD_putc(fondo[i]);
        } else {
            LCD_putrs(" ");
        }
    }

    // Nubes fila 1
    for(uint8_t i = 0; i < 3; i++){
        LCD_Set_Cursor(1, nubes[i]);
        LCD_putc(6);
    }
}

void disparar(void){
    // Mostrar a Bob invertido
    LCD_Set_Cursor(pos_y, pos_x);
    LCD_putc(4); // Bob_reves

    // RA3 se activa
    PORTAbits.RA3 = 1;

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
    LCD_putc(0);
    PORTAbits.RA3 = 0;
}