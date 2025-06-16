#include <xc.h>         // Biblioteca principal del compilador XC8

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

#define led_verde PORTBbits.RB5
#define led_amarillo PORTBbits.RB6
#define led_rojo PORTBbits.RB7

unsigned char PASS[4] = {0x73, 0x77, 0x6D, 0x6D};
unsigned char FAIL[4] = {0x71, 0x77, 0x30, 0x38};
unsigned char digits[3] = {0x6, 0x5B, 0x4F};

int password[4] = {1,1,1,1};
int userInput[4] = {0,0,0,0};

unsigned int tries = 3;
int correct;

void printPass();
void printFail(); 
int confirm(void);
void offLED(void);
void __interrupt() ISR(void);

void main(void) {
    ANSEL = 0;
    ANSELH = 0;
    OPTION_REG &= 0b01111111; // Pull-ups activados
    
    TRISB = 0b00011111; // 0x1F: RB0?RB4 entradas, RB5?RB7 salidas
    TRISC = 0; 
    TRISD = 0; 

    PORTB = 0;
    PORTC = 0;
    PORTD = 0;
    
    INTF = 0;
    INTEDG = 0; // Flanco de bajada
    INTE = 1;   // Interrupción externa (RB0)
    GIE = 1;    // Interrupciones globales
    
    while (1) {
        if(!PORTBbits.RB1)
            userInput[0] = 1; 
            
        if(!PORTBbits.RB2)
            userInput[1] = 1; 
            
        if(!PORTBbits.RB3)
            userInput[2] = 1; 
            
        if(!PORTBbits.RB4)
            userInput[3] = 1;  
        
        PORTC = 0b11111110;
        PORTD = digits[tries - 1];  // Mostrar 3, 2 o 1
        __delay_ms(1);
    }
}

void printPass()
{
    for(int i = 0; i < 25; i++)
    {
        PORTC = 0b11111110;
        PORTD = PASS[0];
        __delay_ms(1);
        
        PORTC = 0b11111101;
        PORTD = PASS[1];
        __delay_ms(1);
        
        PORTC = 0b11111011;
        PORTD = PASS[2];
        __delay_ms(1);
        
        PORTC = 0b11110111;
        PORTD = PASS[3];
        __delay_ms(1);

        led_verde = 1; 
        __delay_ms(1);
    }
    offLED(); 
}

void printFail()
{
    for(int i = 0; i < 25; i++)
    {
        PORTC = 0b11111110;
        PORTD = FAIL[0];
        __delay_ms(1);
        
        PORTC = 0b11111101;
        PORTD = FAIL[1];
        __delay_ms(1);
        
        PORTC = 0b11111011;
        PORTD = FAIL[2];
        __delay_ms(1);
        
        PORTC = 0b11110111;
        PORTD = FAIL[3];
        __delay_ms(1);
        
        if(tries > 0)
        {
            led_rojo = 1;
            __delay_ms(1);
        }
        else
        {
            led_amarillo = 1;
            __delay_ms(1);
        }
    }
    offLED();
}

int confirm(void)
{
    for(int i = 0; i < 4 && tries > 0; i++)
        if(password[i] != userInput[i])
            return 0;
    return 1;
}

void offLED(void)
{
    led_amarillo = 0; 
    led_rojo = 0; 
    led_verde = 0; 
}

void __interrupt() ISR(void){
    if(INTF){
        GIE = 0; // Deshabilita las interrupciones globales
        correct = confirm();
        for(int i = 0; i < 4; i++)
            userInput[i] = 0;
        if (correct) {
            printPass();     // Mostrar PASS
            tries = 3;       // Resetear intentos
        } else {
            printFail();     // Mostrar FAIL
            tries--;
        }
        while (!PORTBbits.RB0);  // Evita múltiples activaciones
        if (tries == 0) {
            while (1) {
                printFail(); // Mantener bloqueo visual
            }
        }
        GIE = 1; // Activar las interrupciones globales otra vez
        INTF = 0; // Resetear la bandera de interrupción
    }
}