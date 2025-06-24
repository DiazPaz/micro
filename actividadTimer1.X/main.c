#include <xc.h>         // Biblioteca principal del compilador XC8
#include <stdint.h>     // Para tipos como uint8_t y int16_t
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

//=============================================================================
// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN (FUSES)
//=============================================================================

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
#define LED PORTDbits.RD0 

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7}; //PORT, RS, EN, D4, D5, D6, D7

volatile unsigned int timer0 = 0, timer1 = 0;
volatile unsigned int count0 = 0;
volatile bool onOff = false;  // Bandera controlada desde la interrupción
char exec[6];

//=============================================================================
// FUNCIONES
//=============================================================================

void TMR1_init(void) {
    T1CON = 0b00110001;  // TMR1ON=1, prescaler 1:8, oscilador interno
    TMR1H = 0xFB;
    TMR1L = 0x1D;
    TMR1IE = 1;          // Habilita interrupción TMR1
    PEIE = 1;            // Habilita interrupciones periféricas
    GIE = 1;             // Habilita interrupciones globales
}

void Timer0_Init() {
    OPTION_REG = 0x07;
    TMR0 = 217;
    T0IE = 1;
    GIE = 1;
}

void ADC_Initialize() {
    ANSEL = 0x01;
    ADCON0 = 0x81;
    ADCON1 = 0x80;
}

unsigned int ADC_read() {
    __delay_us(5); // Tiempo para que el capacitor interno se cargue
    GO_nDONE = 1;
    while (GO_nDONE);
    return (ADRESH << 8) + ADRESL;
}

//=============================================================================
// INTERRUPCIONES
//=============================================================================

void __interrupt() ISR(void) {
    if (T0IF) {
        count0++;
        if (count0 >= 100) {
            timer0++;
            count0 = 0;
        }
        TMR0 = 217;
        T0IF = 0;
    }

    if (TMR1IF) {
        timer1++;
        if (timer1 > 50) 
            timer1 = 0;

        TMR1H = 0xFB;
        TMR1L = 0x1D;

        onOff = !onOff;  // Solo se cambia la bandera, NO el LED directamente

        TMR1IF = 0;
    }
}

//=============================================================================
// MAIN
//=============================================================================

void main(void) {
    ADC_Initialize();
    Timer0_Init();
    TMR1_init();

    TRISD0 = 0;     // RD0 como salida (LED)
    LED = 0;        // Asegurar que el LED inicia apagado

    LCD_Init(lcd);
    LCD_Clear();

    LCD_Set_Cursor(0, 0);
    LCD_putrs("Voltaje: ");
    
    LCD_Set_Cursor(1, 0);
    LCD_putrs(" Tiempo: ");
    LCD_Set_Cursor(1, 9);
    LCD_putrs("00:00");

    char buffer[10];

    while (1) {
        // Encender o apagar el LED según la bandera
        LED = onOff;

        // Mostrar tiempo
        LCD_Set_Cursor(1, 9);
        sprintf(exec, "%02u:%02u", timer0 / 60, timer0 % 60);
        LCD_puts(exec);

        // Leer voltaje y mostrarlo
        unsigned int adc_result = ADC_read();
        unsigned int voltaje = (adc_result * 50000) / 10230;
        unsigned int part_ent = voltaje / 1000;
        unsigned int part_dec = voltaje % 1000;

        sprintf(buffer, "%u.%03u", part_ent, part_dec);
        LCD_Set_Cursor(0, 9);
        LCD_putrs(buffer);

        __delay_ms(100); // Pequeño delay para evitar parpadeo
    }
}
