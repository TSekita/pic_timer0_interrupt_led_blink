
// PIC16F18857 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT1  // Power-up default value for COSC bits (HFINTOSC (1MHz))
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (FSCM timer disabled)

// CONFIG2
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = ON       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = ON     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = OFF     // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will not cause a reset)

// CONFIG3
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled, SWDTEN is ignored)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG4
#pragma config WRT = OFF        // UserNVM self-write protection bits (Write protection off)
#pragma config SCANE = not_available// Scanner Enable bit (Scanner module is not available for use)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (Low Voltage programming enabled. MCLR/Vpp pin function is MCLR.)

// CONFIG5
#pragma config CP = OFF         // UserNVM Program memory code protection bit (Program Memory code protection disabled)
#pragma config CPD = OFF        // DataNVM code protection bit (Data EEPROM code protection disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 1000000 // INTERNAL OSCILLATOR Hz

void timer0_init(void) {
    T0CON1bits.T0CS = 0b011;    // HFINTOSC
    T0CON1bits.T0CKPS = 0b1001; // 1:256
    TMR0H = 0xF0;               // 65535 - (10000000 / 512 * 2) = 61629
    TMR0L = 0x8D;               // TMR0H = 61629 >> 8 , TMR0L = 61629 & 0xFF

    PIR0bits.TMR0IF = 0;        // Timer0 overflow flag = 0
    PIE0bits.TMR0IE = 1;        // Timer0 enabled

    T0CON0bits.T016BIT = 1;     // Timer0 is a 16-bit timer
    T0CON0bits.T0EN = 1;        // Timer0 start
}

void init(void) {
    TRISAbits.TRISA1 = 0;
    LATAbits.LATA1 = 0;

    timer0_init();

    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
}

void __interrupt() isr(void) {
    if (PIR0bits.TMR0IF) {
        PIR0bits.TMR0IF = 0;
        TMR0H = 0xF0;
        TMR0L = 0x8D;

        LATAbits.LATA1 ^= 1;
    }
}

void main(void) {
    init();
    
    while(1);
}