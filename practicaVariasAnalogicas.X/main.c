#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN (FUSES)
//=============================================================================
#pragma config FOSC = XT        // Oscilador XT
#pragma config WDTE = OFF       // Watchdog Timer deshabilitado
#pragma config PWRTE = OFF      // Power-up Timer deshabilitado
#pragma config BOREN = ON       // Brown-out Reset habilitado
#pragma config LVP = OFF        // Low-Voltage Programming deshabilitado
#pragma config CPD = OFF        // Protección EEPROM deshabilitada
#pragma config WRT = OFF        // Escritura Flash deshabilitada
#pragma config CP = OFF         // Protección código Flash deshabilitada

#define _XTAL_FREQ 4000000  

//=============================================================================
// VARIABLES GLOBALES
//=============================================================================
LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7};  // LCD conectado a PORTC
unsigned int adc_result, voltaje, part_ent, part_dec;
char buffer1[10], buffer2[10], buffer3[10];

bool prevRB0 = 1;
bool prevRB1 = 1;
unsigned int channel = 1;
unsigned int menu = 1;

//=============================================================================
// FUNCIONES
//=============================================================================
void ADC_init() {
    ANSEL = 0x07;      // AN0, AN1, AN2 habilitados como entradas analógicas
    ADCON0 = 0x01;     // Encender módulo ADC, canal 0
    ADCON1 = 0x80;     // Justificación a la derecha, Fosc/32
}

unsigned int ADC_read(unsigned int ch) {
    __delay_us(5);
    ADCON0 &= 0x83;        // Limpiar selección de canal
    ADCON0 |= ch << 2;     // Seleccionar nuevo canal
    __delay_ms(2);
    GO_nDONE = 1; 
    while(GO_nDONE);       // Esperar fin de conversión
    return ((ADRESH << 8) + ADRESL);
}

void volt(char buffer[]) {
    LCD_Set_Cursor(0, 3);
    LCD_putrs("Voltaje: ");
    
    voltaje = (adc_result * 50000) / 10230;
    part_ent = voltaje / 1000;
    part_dec = voltaje % 1000;

    sprintf(buffer, "%u.%u", part_ent, part_dec);
    LCD_Set_Cursor(1, 0);
    LCD_putrs(buffer);
    __delay_ms(200);
}

void porcentaje(char buffer[]) {
    LCD_Set_Cursor(0, 3);
    LCD_putrs("Porcentaje: ");
    
    voltaje = (adc_result * 1000000) / 102300;
    part_ent = voltaje / 100;
    part_dec = voltaje % 100;

    sprintf(buffer, "%u.%u", part_ent, part_dec);
    LCD_Set_Cursor(1, 0);
    LCD_putrs(buffer);
    __delay_ms(200);
}

void mil(char buffer[]) {
    LCD_Set_Cursor(0, 3);
    LCD_putrs("Mil: ");
    
    sprintf(buffer, "%u", adc_result);
    LCD_Set_Cursor(1, 0);
    LCD_putrs(buffer);
    __delay_ms(200);
}

void viewOpt(unsigned int menuOpt) {
    switch(menuOpt) {
        case 1: 
            volt(buffer1); 
            break;
        case 2: 
            porcentaje(buffer2); 
            break;
        case 3: 
            mil(buffer3); 
            break;
    }
}

//=============================================================================
// FUNCIÓN PRINCIPAL
//=============================================================================
int main(void) {
    ANSELH = 0;                    // Deshabilitar entradas analógicas en PORTB
    OPTION_REG &= 0b01111111;      // Pull-ups habilitados en PORTB
    TRISB = 0b00000011;            // RB0 y RB1 como entrada
    PORTB = 0;

    ADC_init();
    LCD_Init(lcd);

    while(1) {
        LCD_Clear();
        LCD_Set_Cursor(0, 0);
        switch(channel)
        {
            case 1: 
                LCD_putrs("1.");
                break; 
            case 2:
                LCD_putrs("2.");
                break; 
            case 3:
                LCD_putrs("3.");
                break; 
        }

        // Cambio de canal con RB0
        if(!PORTBbits.RB0 && prevRB0) {
            channel++;
            if(channel > 3) channel = 1;
        }
        prevRB0 = PORTBbits.RB0;

        // Cambio de visualización con RB1
        if(!PORTBbits.RB1 && prevRB1) {
            menu++;
            if(menu > 3) menu = 1;
        }
        prevRB1 = PORTBbits.RB1;

        adc_result = ADC_read(channel - 1);
        viewOpt(menu);

        __delay_ms(20);  // Antirebote básico
    }

    return EXIT_SUCCESS;
}
