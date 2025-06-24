#include <xc.h>         
#include <stdint.h>     
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lcd.h"

// CONFIGURACIÓN DE BITS DE CONFIGURACIÓN
#pragma config FOSC = XT        
#pragma config WDTE = OFF       
#pragma config PWRTE = OFF      
#pragma config BOREN = ON       
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF        
#pragma config CP = OFF         

#define _XTAL_FREQ 4000000      

LCD lcd = {&PORTC, 2, 3, 4, 5, 6, 7 };

volatile unsigned int count1 = 0; 
volatile unsigned int timer1 = 0; 
char exec[6];

// Inicializa TMR1 para interrupciones cada ~10ms (con cristal de 4MHz y prescaler de 1:8)
void TMR1_init(void)
{
    T1CON = 0b00110001;  // TMR1ON=1, prescaler 1:8, oscilador interno
    TMR1H = 0xB;
    TMR1L = 0xDC;           // Valor bajo para recargar
    TMR1IE = 1;          // Habilita interrupción TMR1           
    PEIE = 1;            // Habilita interrupciones periféricas
    GIE = 1;             // Habilita interrupciones globales
}

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

void __interrupt() ICR(void)
{
    if (TMR1IF)
    {
        timer1++;
        if(timer1 > 100)
        {
            count1++;
            timer1 = 0;
        }

        // Recargar el TMR1 para que vuelva a desbordarse a ~10 ms
        TMR1H = 0xB;
        TMR1L = 0xDC;
        
        TMR1IF = 0;  // Limpiar bandera de interrupción
    }
}

void main(void)
{
    TMR1_init();
    LCD_Init(lcd);
    char buffer1[10];
    ADC_init();
    
    LCD_Clear();
    
    
    LCD_Set_Cursor(1,0);
    LCD_putrs("00:00");
    
    while(1)
    {
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
        sprintf(exec,"%02u:%02u",timer1/60,timer1%60);
        LCD_puts(exec);
    }
}
