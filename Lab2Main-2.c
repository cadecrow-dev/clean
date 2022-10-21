
#include <xc.h>
// PIC32MX170F256B Configuration Bit Settings for ME218 operating at 40MHz from internal fast oscillator (8Mhz)

// 'C' source line config statements

// DEVCFG3
#pragma config USERID = 0xFFFF          // Enter Hexadecimal value (Enter Hexadecimal value)
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = FRCPLL           // Oscillator Selection Bits (Fast RC Osc with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Enable (Disabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_2           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/2)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#define MAX_SPI_PERIOD ((8191+1)*2*0)
#define MAP_SS1 0b0011
#define MAP_SS2 0b0100
#define MAP_SDO1 0b0011
#define MAP_SDO2 0b0100

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//	Function Prototypes
int main(void);

int main(void) {
    // Must disable analog select
    ANSELA = 0;
    ANSELBbits.ANSB14 = 0;

    // set the SS, SDO, and SCK to the desired pins
    TRISAbits.TRISA0 = 0; // SS
    TRISAbits.TRISA1 = 0; // SDO
    TRISBbits.TRISB14=0; //SCK
    RPA0R = MAP_SS1;
    RPA1R = MAP_SDO1;

    // todo
    //SDI MAP INPUT - not needed for this case

    // clear the ON bit to reset the SPI 
    SPI1CONbits.ON = 0;

    // clear the receive buffer
    SPI1BUF;

    // Set to enhanced buffer mode
    SPI1CONbits.ENHBUF = 1;

    // set the baud rate register to 10kHz
    SPI1BRG = 999;
    
    // todo
    // clear spirov - not needed for this

    // Write appropriate Leader Follower settings
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.MSSEN = 1;

    // All other values to properly initialize SPI
    SPI1CONbits.DISSDO = 0;
    SPI1CONbits.SMP = 0;
    SPI1CONbits.FRMPOL = 0;
    SPI1CONbits.MCLKSEL = 0;

    // set SPI for 8 bit communication
    SPI1CONbits.MODE32 = 0;
    SPI1CONbits.MODE16 = 0;

//Data edge and idle state
    SPI1CONbits.CKE = 0;
    SPI1CONbits.CKP = 1;

    //Set ON bit to start SPI operation
    SPI1CONbits.ON = 1;

    // Write data to the buffer
    SPI1BUF = 'M';
    SPI1BUF = 'E';
    SPI1BUF = '2';
    SPI1BUF = '1';
    SPI1BUF = '8';
    SPI1BUF = 'A';

    // execute for loop
    while(SPI1STATbits.SPITBF){}
    
    // wait before sending SS high
    for(int i = 0; i < 500; i = i + 1 ){} 

}



