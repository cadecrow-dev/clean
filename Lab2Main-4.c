
#include <xc.h>
#include "PIC32_SPI_HAL-4.h"
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

#define MAX_SPI_PERIOD  ((8191+1)*2*50)
#define MAP_SS1 0b0011
#define MAP_SS2 0b0100
#define MAP_SDO1 0b0011
#define MAP_SDO2 0b0100

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


//	Function Prototypes
int main(void);

int main(void) {
    const SPI_Module_t module = SPI_SPI1;
    const SPI_PinMap_t ss_pin =  SPI_RPA0;
    const SPI_PinMap_t sdo_pin =  SPI_RPA1;
    const bool isEnhanced = true;
    const SPI_SamplePhase_t WhichPhase = SPI_SMP_MID;
    const SPI_ActiveEdge_t WhichEdge = SPI_FIRST_EDGE;
    const SPI_Clock_t WhichState = SPI_CLK_HI;
    const SPI_XferWidth_t DataWidth = SPI_16BIT;
    
    
    
    // Must disable analog select
    SPISetup_BasicConfig(module);
    
    // set the SS, SDO, and SCK to the desired pins
    SPISetup_SetLeader(module, WhichPhase);
    SPISetup_MapSSOutput(module, ss_pin);
    SPISetup_MapSDOutput(module, sdo_pin);
    
    // clear the ON bit to reset the SPI 
    SPISetup_DisableSPI(module);
    
    // Set to enhanced buffer mode
    SPISetEnhancedBuffer(module, isEnhanced);
    
    // set the baud rate register to 10kHz
    SPISetup_SetBitTime(module, 1e9 / 10e3);
    
    // All other values to properly initialize SPI
    SPISetup_SetXferWidth(module, DataWidth);
    SPISetup_SetActiveEdge(module, WhichEdge);
    SPISetup_SetClockIdleState(module, WhichState);
    
    //Set ON bit to start SPI operation
    SPISetup_EnableSPI(module);
    
    // Write data to the buffer
    uint32_t data[] = {0x004D, 0x0045, 0x0032, 0x0031, 0x0038, 0x804D, 0x8045, 0x8032, 0x8031, 0x8038};
    int length_data = 0;
    if (sizeof(data) > 0) {
        length_data = sizeof(data)/sizeof(data[0]);
    }
    for (int i=0; i<length_data; i++) {
        if (i == length_data - 1) {
            // final character
            SPIOperate_SPI1_Send16Wait(data[i]);
        } else {
            SPIOperate_SPI1_Send16(data[i]);
            while (SPI1STATbits.SPITBF == 1) {}
        }
    }
}



