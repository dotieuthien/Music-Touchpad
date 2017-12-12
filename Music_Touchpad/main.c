/*
 * Author: Do Tieu Thien
 * Date: 13/11/2017
 */


//**********************************************************************************************************
// Import
//**********************************************************************************************************
#include <msp430g2553.h>
#include "Library/LCD16x2.h"
#include "Library/Key.h"
#include "Library/Note.h"
#include "Library/Song.h"
#define WDT_meas_setting    (WDT_MDLY_8)


//**********************************************************************************************************
// Global Variables
//**********************************************************************************************************
#define BUZZER  BIT6
#define F_CPU   1200000L
unsigned char dem = 0;
int m = 0;
unsigned int *melody;
unsigned int melody1[] = {C1, D1, E1, F1, G1, A1, B1, C2};
unsigned int melody2[] = {C2, D2, E2, F2, G2, A2, B2, C3};
unsigned int melody3[] = {C3, D3, E3, F3, G3, A3, B3, C4};
unsigned int melody4[] = {C4, D4, E4, F4, G4, A4, B4, C5};
unsigned int melody5[] = {C5, D5, E5, F5, G5, A5, B5, C6};
volatile unsigned long count;


//**********************************************************************************************************
// Prototype
//**********************************************************************************************************
void base_calib(void);
int measure_count(struct Element pad);
void playNote(unsigned int note, unsigned int duration);


//**********************************************************************************************************
// Main
//**********************************************************************************************************
int main(void)
{
    int delta_count;
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    BCSCTL1 = CALBC1_16MHZ;             // Set DCO to 16MHz
    DCOCTL =  CALDCO_16MHZ;
    P1DIR |= BUZZER;
    Config_Switch();
    LCD_Init();
    LCD_Clear();
    LCD_Home();
    LCD_PrintString("Hello!!!");
    base_calib();
    __bis_SR_register(GIE);
    while(1)
    {
        while((m <= 97)&(m>0)) {
            playNote(Song[m-1], (5500));
            m++;
            _delay_cycles(100000);
        }
        __delay_cycles(500000);
        delta_count = *(KEY_1.baseline) - measure_count(KEY_1);
        if(delta_count > 100){
            playNote(melody[0], (5000));
        }

        delta_count = *(KEY_2.baseline) - measure_count(KEY_2);
        if(delta_count > 100){
            playNote(melody[1], (5000));
        }

        delta_count = *(KEY_3.baseline) - measure_count(KEY_3);
        if(delta_count > 100){
            playNote(melody[2], (5000));
        }

        delta_count = *(KEY_4.baseline) - measure_count(KEY_4);
        if(delta_count > 100){
            playNote(melody[3], (5000));
        }

        delta_count = *(KEY_5.baseline) - measure_count(KEY_5);
        if(delta_count > 100){
            playNote(melody[4], (5000));
        }

        delta_count = *(KEY_6.baseline) - measure_count(KEY_6);
        if(delta_count > 100){
            playNote(melody[5], (5000));
        }

        delta_count = *(KEY_7.baseline) - measure_count(KEY_7);
        if(delta_count > 100){
            playNote(melody[6], (5000));
        }

        delta_count = *(KEY_8.baseline) - measure_count(KEY_8);
        if(delta_count > 100){
            playNote(melody[7], (5000));
        }
    }
}


//**********************************************************************************************************
// Program of subroutines
//**********************************************************************************************************
// Measure the baseline capacitance each pad
void base_calib(void)
{
    char i;
    for(i = 0; i < 8; i++){
        base_cap[i] = measure_count(*KEY[i]);
    }
}

//**********************************************************************************************************
// Measure raw capacitance
int measure_count(struct Element pad)
{
    int meas_cnt;
    IE1 |= WDTIE;                       // Enable WDT interrupt
    TA0CTL = TASSEL_3 + MC_2;           // TACLK, continous mode
    TA0CCTL1 = CM_3 + CCIS_2 + CAP;     // Pos & Neg, GND, capture mode

    *(pad.Pxsel) &= ~pad.inputBits;     // TACLK input
    *(pad.Pxsel2) |= pad.inputBits;
    *(pad.Pxdir) &= ~pad.inputBits;     // input pins

    // Setup Gate Timer
    WDTCTL = WDT_meas_setting;          // WDT, SMCLK, interval timer
    TA0CTL |= TACLR;                    // Clear Timer_A TAR
    __bis_SR_register(LPM0_bits + GIE); // Wait for WDT interrupt
    meas_cnt = TA0CCR1;                 // Save result
    WDTCTL = WDTPW + WDTHOLD;           // Stop Watchdog timer
    TA0CTL = MC_0;                      // Stop timer0
    *(pad.Pxsel2) &= ~pad.inputBits;    // GIPO mode
    return meas_cnt;
}

//**********************************************************************************************************
void playNote(unsigned int note, unsigned int duration)
{
    volatile unsigned int period, cycles;

    period = F_CPU / note;                          // Timer Period = F_CPU / Fnote
    cycles = (F_CPU * duration)/(1000L * period);   // Note duration as number of Timer cycles

    count = cycles;                                 // Set global count variable
    TA0CCR0 = period;                               // Set timer period
    TA0CCR1 = period/2;                             // Generate output on TA1.1 at 50% duty cycle
    TA0CTL = TACLR + TASSEL_2 +  MC_1;              // Timer -> SMCLK, Up Mode, Clear
    TA0CCTL0 |= CCIE;                               // Enable CCR0 interrupt
    TA0CCTL1 |= OUTMOD_7;                           // Output mode for TA1.1

    if(note > 0 && duration > 0)                    // Only if note/duration is non zero
        P1SEL |= BUZZER;                            // Send Timer Output to pin

    __bis_SR_register(LPM0_bits + GIE);             // Goto LPM and wait for note to complete
    TA0CTL = MC_0;                                  // Stop Timer
    P1SEL &= ~BUZZER;                               // Turn off timer output to pin
}


//**********************************************************************************************************
// Interrupt service routine
//**********************************************************************************************************
// Interrupt for Capacitive touch
#pragma vector = WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    TA0CCTL1 ^= CCIS0;                          // Toggle CM0 to initiate capture
    __bic_SR_register_on_exit(LPM3_bits);       // Exit LPM3 on reti
}

//**********************************************************************************************************
// Interrupt for Switch
#pragma vector= PORT1_VECTOR
__interrupt void INT(void)
{
    _delay_cycles(50000);
    dem++;
    PIFG_SWITCH &= ~ SWITCH;
    if (dem == 1){
        m = -1;
        melody = melody1;
        LCD_Clear();
        LCD_PrintString ("Song 1");
    }
    else if (dem == 2){
        melody = melody2;
        LCD_Clear();
        LCD_PrintString ("Song 2");
    }
    else if (dem == 3) {
        melody = melody3;
        LCD_Clear();
        LCD_PrintString ("Song 3");
    }
    else if (dem == 4){
        melody = melody4;
        LCD_Clear();
        LCD_PrintString ("Song 4");
    }
    else {
        dem = 0;
        m = 1;
        LCD_Clear();
        LCD_PrintString ("Xe Dap");
    }
}

//**********************************************************************************************************
// Interrupt for Note
#pragma vector = TIMER0_A0_VECTOR                   // Timer 1 CCR0 Interrupt Vector
__interrupt void CCR0_ISR(void)
{
    count--;                                        // Decrement duration counter
    if(count == 0)
        __bic_SR_register_on_exit(LPM0_bits);       // Exit LPM when note is complete
}
