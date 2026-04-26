// Configuration Registers.
#pragma config FEXTOSC = OFF, RSTOSC = HFINT32, CLKOUTEN = OFF, CSWEN = OFF
#pragma config FCMEN = ON, MCLRE = ON, PWRTE = OFF, LPBOREN = OFF, BOREN = OFF
#pragma config BORV = LO, ZCD = OFF, PPS1WAY = OFF, STVREN = ON
#pragma config WDTCPS = WDTCPS_31, WDTE = OFF, WDTCWS = WDTCWS_7, WDTCCS = SC
#pragma config WRT = OFF, SCANE = available, LVP = ON
#pragma config CP = OFF, CPD = OFF

#include <xc.h>
#include <stdint.h>
#include <pic16f18856.h>
#define _XTAL_FREQ 8000000
// PIC16F18856 - Compile with XC8(v3.00).
// PIC16F18856 - @8MHz Internal Oscillator.

// PCM1795 - v0.1 - Q2/2025.
// 

// todo: faire un tablo pour l'init
// todo: lire l'ID pour verifier i2c pcm1795
// verifier R18ATLD, a priori fonctionne comme PWM
// il permet de charger les registres d attenuation 16 et 17

// PIC8-Bit Mini Trainer.
// Jumpers.
// URX - Close.
// UTX - Close.
// SDA - Close.
// SCL - Close.
// BCKL - Open.
// VCAP - Open.

// Pinout.
// MCU.RA0 <-
// MCU.RA1 <-
// MCU.RA2 <-
// MCU.RA3 <-
// MCU.RA4 <-
// MCU.RA5 -> CAT4002.EN.
// MCU.RA6 -> (DEBUG).
// MCU.RA7 <-

// MCU.RB0 <->
// MCU.RB1 <->
// MCU.RB2 <->
// MCU.RB3 <-
// MCU.RB4 <-
// MCU.RB5 <->.
// MCU.RB6 ->  EUSART.UTX.
// MCU.RB7 <-  EUSART.URX.

// MCU.RC0 ->
// MCU.RC1 ->
// MCU.RC2 ->
// MCU.RC3 ->  MSSP1.SCL.
// MCU.RC4 <-> MSSP1.SDA.
// MCU.RC5 ->
// MCU.RC6 <->
// MCU.RC7 <->

// Definitions.
#define PCM1795_RESET       LATCbits.LATC2
// CAT4002 Pinout.
#define CAT4002_ENABLE      LATAbits.LATA5
// I2C.
#define I2C_READ            0b1
#define I2C_WRITE           0b0
#define I2C_ACK             0x00
#define I2C_NACK            0x01
// MSSP1 I2C.
#define MSSP1_FSCL_HZ       400000
#define MSSP1_BAUDRATE      ((_XTAL_FREQ/MSSP1_FSCL_HZ/4)-1)
// EUSART.
#define BAUDRATE            9600
#define BAUDRATE_GENERATOR  ((_XTAL_FREQ/BAUDRATE/16)-1)
// ASCII Characters.
#define ASCII_CR            0x0D
#define ASCII_d             0x64
#define ASCII_MINUS         0x2D
#define ASCII_DOT           0x2E
// CAT4002A.
#define CAT4002_DELAY_DIM_MS                                5
#define CAT4002_DELAY_LED_US                                10
// ST7036 I2C Address.
#define ST7036_I2C_ADDRESS_78                               0x78
#define ST7036_I2C_CONTROL_CONTINUOUS_COMMAND               0x00
#define ST7036_I2C_CONTROL_CONTINUOUS_DATA                  0x40
#define ST7036_I2C_CONTROL_NO_CONTINUOUS_COMMAND            0x80
#define ST7036_I2C_CONTROL_NO_CONTINUOUS_DATA               0xC0
// ST7036 Instruction Table IS2=0, IS1=0.
#define ST7036_CLEAR_DISPLAY                                0x01
#define ST7036_RETURN_HOME                                  0x02
#define ST7036_ENTRY_MODE_SET_DDRAM_DECREMENT_NOSHIFT       0x04
#define ST7036_ENTRY_MODE_SET_DDRAM_DECREMENT_SHIFT_RIGHT   0x05
#define ST7036_ENTRY_MODE_SET_DDRAM_INCREMENT_NOSHIFT       0x06
#define ST7036_ENTRY_MODE_SET_DDRAM_INCREMENT_SHIFT_LEFT    0x07
#define ST7036_DISPLAY_OFF                                  0x08
#define ST7036_DISPLAY_ON_CURSOR_OFF                        0x0C
#define ST7036_DISPLAY_ON_CURSOR_ON_NOBLINK                 0x0E
#define ST7036_DISPLAY_ON_CURSOR_ON_BLINK                   0x0F
#define ST7036_DISPLAY_CURSOR_SHIFT_LEFT                    0x10
#define ST7036_DISPLAY_CURSOR_SHIFT_RIGHT                   0x14
#define ST7036_DISPLAY_DISPLAY_SHIFT_LEFT                   0x18
#define ST7036_DISPLAY_DISPLAY_SHIFT_RIGH                   0x1C
#define ST7036_FUNCTION_SET_4_BIT_ONE_LINE_FONT5x8          0x20
#define ST7036_FUNCTION_SET_4_BIT_ONE_LINE_FONT5x8_IS1      0x21
#define ST7036_FUNCTION_SET_4_BIT_ONE_LINE_FONT5x8_IS2      0x22
#define ST7036_FUNCTION_SET_4_BIT_TWO_LINE_FONT5x8          0x28
#define ST7036_FUNCTION_SET_4_BIT_ONE_LINE_DHFONT5x8        0x24
#define ST7036_FUNCTION_SET_4_BIT_TWO_LINE_DHFONT5x8        0x2C
#define ST7036_FUNCTION_SET_8_BIT_ONE_LINE_FONT5x8          0x30
#define ST7036_FUNCTION_SET_8_BIT_ONE_LINE_FONT5x8_IS1      0x31
#define ST7036_FUNCTION_SET_8_BIT_ONE_LINE_FONT5x8_IS2      0x32
#define ST7036_FUNCTION_SET_8_BIT_ONE_LINE_DHFONT5x8        0x34
#define ST7036_FUNCTION_SET_8_BIT_TWO_LINE_FONT5x8          0x38
#define ST7036_FUNCTION_SET_8_BIT_TWO_LINE_DHFONT5x8        0x3C
#define ST7036_SET_ICON_RAM_ADDRESS                         0X40
#define ST7036_SET_CGRAM_ADDRESS                            0x40
#define ST7036_DDRAM_ADDRESS_FIRST_LINE                     0x80
#define ST7036_DDRAM_ADDRESS_SECOND_LINE                    0xC0
// ST7036 Instruction Table IS2=0, IS1=1.
#define ST7036_BIAS_SET_1_5                                 0x14
#define ST7036_BIAS_SET_1_5_3_LINE                          0x15
#define ST7036_BIAS_SET_1_4                                 0x1C
#define ST7036_BIAS_SET_1_4_3_LINE                          0x1D
#define ST7036_POWER_ICON_OFF_BOOST_OFF_NO_CONTRAST         0x50
#define ST7036_POWER_ICON_ON_BOOST_OFF_NO_CONTRAST          0x58
#define ST7036_POWER_ICON_OFF_BOOST_ON_CONTRAST_MSB_0       0x54
#define ST7036_POWER_ICON_OFF_BOOST_ON_CONTRAST_MSB_1       0x55
#define ST7036_POWER_ICON_OFF_BOOST_ON_CONTRAST_MSB_2       0x56
#define ST7036_POWER_ICON_OFF_BOOST_ON_CONTRAST_MSB_3       0x57
#define ST7036_POWER_ICON_ON_BOOST_ON_CONTRAST_MSB_0        0x5C
#define ST7036_POWER_ICON_ON_BOOST_ON_CONTRAST_MSB_1        0x5D
#define ST7036_POWER_ICON_ON_BOOST_ON_CONTRAST_MSB_2        0x5E
#define ST7036_POWER_ICON_ON_BOOST_ON_CONTRAST_MSB_3        0x5F
#define ST7036_FOLLOWER_CONTROL_OFF                         0x60
#define ST7036_FOLLOWER_CONTROL_ON_RAB_0                    0x68
#define ST7036_FOLLOWER_CONTROL_ON_RAB_1                    0x69
#define ST7036_FOLLOWER_CONTROL_ON_RAB_2                    0x6A
#define ST7036_FOLLOWER_CONTROL_ON_RAB_3                    0x6B
#define ST7036_FOLLOWER_CONTROL_ON_RAB_4                    0x6C
#define ST7036_FOLLOWER_CONTROL_ON_RAB_5                    0x6D
#define ST7036_FOLLOWER_CONTROL_ON_RAB_6                    0x6E
#define ST7036_FOLLOWER_CONTROL_ON_RAB_7                    0x6F
#define ST7036_CONTRAST_LSB_0                               0x70
#define ST7036_CONTRAST_LSB_1                               0x71
#define ST7036_CONTRAST_LSB_2                               0x72
#define ST7036_CONTRAST_LSB_3                               0x73
#define ST7036_CONTRAST_LSB_4                               0x74
#define ST7036_CONTRAST_LSB_5                               0x75
#define ST7036_CONTRAST_LSB_6                               0x76
#define ST7036_CONTRAST_LSB_7                               0x77
#define ST7036_CONTRAST_LSB_8                               0x78
#define ST7036_CONTRAST_LSB_9                               0x79
#define ST7036_CONTRAST_LSB_10                              0x7A
#define ST7036_CONTRAST_LSB_11                              0x7B
#define ST7036_CONTRAST_LSB_12                              0x7C
#define ST7036_CONTRAST_LSB_13                              0x7D
#define ST7036_CONTRAST_LSB_14                              0x7E
#define ST7036_CONTRAST_LSB_15                              0x7F
// ST7036 Instruction Table IS2=1, IS1=0.
#define ST7036_DOUBLE_HEIGHT_FONT_COM9_COM24                0x10
#define ST7036_DOUBLE_HEIGHT_FONT_COM1_COM16                0x11
// ST7036 Delays.
#define ST7036_CLEAR_DISPLAY_DELAY_MS                       2
#define ST7036_INITIALIZATION_DELAY_MS                      40
// LCD Patterns.
#define LCD_PATTERN_PAUSE                   0x00
#define LCD_PATTERN_PLAY                    0x01
#define LCD_PATTERN_STOP                    0x02
// PCM5142 I2C Address.
#define PCM1795_I2C_ADDRESS_98              0x98
#define PCM1795_I2C_ADDRESS_9A              0x9A
#define PCM1795_I2C_ADDRESS_9C              0x9C
#define PCM1795_I2C_ADDRESS_9E              0x9E
#define PCM1795_READ                        0x80
#define PCM1795_WRITE                       0x00

// Configurations.
// NHD-C0220BiZ Configuration.
#define C0220BiZ_CONFIGURATION_I2C_ADDRESS  ST7036_I2C_ADDRESS_78
#define C0220BiZ_CONFIGURATION_FIRST_LINE   ST7036_DDRAM_ADDRESS_FIRST_LINE
#define C0220BiZ_CONFIGURATION_SECOND_LINE  ST7036_DDRAM_ADDRESS_SECOND_LINE
#define C0220BiZ_CONFIGURATION_CHARACTERS   20
// PCM1795 Configuration.
#define PCM1795_CONFIGURATION_I2C_ADDRESS   PCM1795_I2C_ADDRESS_98

// Function Prototypes.
uint8_t eusartReadCharacter(void);
void eusartWriteCharacter(uint8_t u8Data);
void eusartWriteString(const uint8_t * u8Data);
uint8_t mssp1Read(uint8_t u8ACKDT);
uint8_t mssp1ReadACK(void);
uint8_t mssp1ReadNACK(void);
void mssp1Restart(void);
void mssp1Start(void);
void mssp1Stop(void);
void mssp1Write(uint8_t u8Data);
void lcd_setCursor(uint8_t u8Cursor);
void lcd_writeCharacter(uint8_t u8Data);
void lcd_writeInstruction(uint8_t u8Data);
void lcd_writeString(const uint8_t * u8Data);
void lcd_writeStringSetCursor(const uint8_t * u8Data, uint8_t u8Cursor);
uint8_t pcm1795_readRegister(uint8_t u8Register);
void pcm1795_writeRegister(uint8_t u8Register, uint8_t u8Data);
void u8toASCII(uint8_t u8Data, uint8_t * au8Buffer, uint8_t u8Base);


// Strings & Patterns.
// EUSART Strings.
const uint8_t au8EUSARTTronix[] = "\r\n\r\nTronix I/O";
const uint8_t au8EUSARTWWW[] = "\r\nhttps://www.tronix.io/\r\n";
const uint8_t au8EUSARTReady[] = "\r\nREADY> ";
//const uint8_t au8Dump[] = "\r\nDump PCM4152 Registers:";
//const uint8_t au8ReadPage[] = "\r\nRead Page ";
//const uint8_t au8WritePage[] = "\r\nWrite Page ";
//const uint8_t au8EUSARTNL[] = "\r\n> ";
// LCD Strings & Pattern..
const uint8_t au8LCDTronix[] = "     Tronix I/O     ";
//const uint8_t au8LCDStartup[] = "Startup";
//const uint8_t au8LCDShutdown[] = "Shutdown";
//const uint8_t au8LCDPowerOff[] = "Power OFF";
//const uint8_t au8LCDClearLine[] = "                    ";
//const uint8_t au8LCDdB[] = "dB";

const uint8_t au8LCDAudioPattern[3][8] = {
    {0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x1B, 0x00}, // Pause.
    {0x18, 0x1C, 0x1E, 0x1F, 0x1E, 0x1C, 0x18, 0x00}, // Play.
    {0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00}  // Stop.
};

// Global Variables.
const uint8_t au8encoder[16] = {0, 1, 255, 0, 255, 0, 0, 1, 1, 0, 0, 255, 0, 255, 1, 0};

int8_t i8encoderDelta;

uint8_t u8FLAG = 0;
uint8_t u8FLAGPLAY = 0;
uint8_t u8VOLUME = 160;
uint8_t u8VOLUMELAST = 0;
uint8_t u8PAGECURRENT = 0;

// Interrupts Service Routines.
void __interrupt() ISR(void){
    // TIMER0.
//    if(PIR0bits.TMR0IF){
//        static uint8_t u8encoder = 0;
//        u8encoder = (u8encoder<<2) & 0x0F;
//        if(ROTARY_PHASE_A) u8encoder |= 0b10; // CW. todo 1
//        if(ROTARY_PHASE_B) u8encoder |= 0b01; // CCW. todo 0
//        i8encoderDelta += au8encoder[u8encoder];
//        PIR0bits.TMR0IF = 0b0;
//    }

//    if(PIR0bits.INTF){
//        if(PORTBbits.RB0){
//            CAT4002_ENABLE = 0b0;
//            __delay_ms(1000);
//            pcm5142_writeRegister(3, 0x00);
//            INTCONbits.INTEDG = 0b0;
//        } else {
//            CAT4002_ENABLE = 0b1;
//            pcm5142_writeRegister(3, 0x11);
//            INTCONbits.INTEDG = 0b1;
//        }
//    }
//    PIR0bits.INTF = 0b0;
}

// Main.
void main(void){
    // MCU Initialization.
    // Internal Oscillator Settings.
    OSCTUNE = 0b00000000;
    OSCFRQ = 0b00000011;
    while(!OSCSTATbits.HFOR){};
    // Ports Settings.
    // PORT Data Register.
    PORTA = 0b00000000;
    PORTB = 0b00000000;
    PORTC = 0b00000000;
    PORTE = 0b00000000;
    // TRIS Data Direction.
    TRISA = 0b00000000;
    TRISB = 0b00000000;
    TRISC = 0b00011000;
    // LATCH Outputs.
    LATA = 0b00000000;
    LATB = 0b00000000;
    LATC = 0b01011000;
    // PORTA Settings.
    ANSELA = 0b00000000;
    WPUA = 0b00000000;
    ODCONA = 0b00000000;
    SLRCONA = 0b11111111;
    INLVLA = 0b00000000;
    IOCAP = 0b00000000;
    IOCAN = 0b00000000;
    IOCAF = 0b00000000;
    // PORTB Settings.
    ANSELB = 0b00000000;
    WPUB = 0b00000000;
    ODCONB = 0b00000000;
    SLRCONB = 0b11111111;
    INLVLB = 0b00000000;
    IOCBP = 0b00000000;
    IOCBN = 0b00000000;
    IOCBF = 0b00000000;
    // PORTC Settings.
    ANSELC = 0b00000000;
    WPUC = 0b00000000;
    ODCONC = 0b00000000;
    SLRCONC = 0b11111111;
    INLVLC = 0b00000000;
    IOCCP = 0b00000000;
    IOCCN = 0b00000000;
    IOCCF = 0b00000000;
    // PORTE Settings.
    WPUE = 0b00000000;
    INLVLE = 0b00000000;
    IOCEP = 0b00000000;
    IOCEN = 0b00000000;
    IOCEF = 0b00000000;
    // PPS Settings.
    // PPS Enable Write.
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0b0;
    // PPS Inputs.
    SSP1CLKPPSbits.SSP1CLKPPS = 0x13;   // RC3 - MSSP1.SCL.
    SSP1DATPPSbits.SSP1DATPPS = 0x14;   // RC4 - MSSP1.SDA.
    RXPPSbits.RXPPS = 0x17;             // RC7 - EUSART.URX.
    // PPS Outputs.
    RC3PPS = 0x14;  // RC3 - MSSP1.SCL.
    RC4PPS = 0x15;  // RC4 - MSSP1.SDA.
    RC6PPS = 0x10;  // RC6 - EUSART.UTX.
    // PPS Disable Write.
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0b1;

    // TIMER0 Settings.
    // todo indiquer la frequence avec cette configuration
    TMR0L = 0;
    TMR0H = 255;
    T0CON0 = 0b00000000; // todo 0x00
    T0CON1 = 0b01000100; // todo 0x44 a priori 125k
    // TIMER0 Enable.
    T0CON0bits.T0EN = 0b1;

    // MSSP1 I2C Master Settings.
    SSP1BUF = 0;
    SSP1ADD = MSSP1_BAUDRATE;
    SSP1MSK = 0;
    SSP1STAT = 0x00;
    SSP1CON1 = 0x08;
    SSP1CON2 = 0x00;
    SSP1CON3 = 0x00;

    // EUSART Settings.
    RC1REG = 0;
    TX1REG = 0;
    SP1BRGL = BAUDRATE_GENERATOR;
    SP1BRGH = 0;
    RC1STA = 0x10;
    TX1STA = 0x24;
    BAUD1CON = 0x00;
    // EUSART Enable.
    RC1STAbits.SPEN = 0b1;

    // External Peripherals Initialization.
    // LCD Backlight Initialization.
    uint8_t u8Backlight = 25;
    CAT4002_ENABLE = 0b0;
    __delay_ms(CAT4002_DELAY_DIM_MS);
    CAT4002_ENABLE = 0b1;
    __delay_us(CAT4002_DELAY_LED_US);
    while(u8Backlight--){
        CAT4002_ENABLE = 0b0;
//        CAT4002_ENABLE = 0b1;
    }


/*
    // LCD Initialization.
    __delay_ms(ST7036_INITIALIZATION_DELAY_MS);
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_CONTINUOUS_COMMAND);
    mssp1Write(ST7036_FUNCTION_SET_8_BIT_ONE_LINE_FONT5x8_IS1);
    mssp1Write(ST7036_BIAS_SET_1_5);
    mssp1Write(ST7036_CONTRAST_LSB_12);
    mssp1Write(ST7036_POWER_ICON_ON_BOOST_ON_CONTRAST_MSB_1);
    mssp1Write(ST7036_FOLLOWER_CONTROL_ON_RAB_5);
    mssp1Write(ST7036_FUNCTION_SET_8_BIT_TWO_LINE_FONT5x8);
    mssp1Write(ST7036_DISPLAY_ON_CURSOR_OFF);
    mssp1Write(ST7036_ENTRY_MODE_SET_DDRAM_INCREMENT_NOSHIFT);
    mssp1Write(ST7036_CLEAR_DISPLAY);
    mssp1Stop();
    __delay_ms(ST7036_CLEAR_DISPLAY_DELAY_MS);
    // LCD Write 5x8 Dots Custom Patterns in ST7036 CGRAM.
    uint8_t u8Line, u8Pattern;
    lcd_writeInstruction(ST7036_SET_CGRAM_ADDRESS);
    for(uint8_t u8Pattern=0; u8Pattern<3; u8Pattern++){
        for(u8Line=0; u8Line<8; u8Line++){
            lcd_writeCharacter(au8LCDAudioPattern[u8Pattern][u8Line]);
        }
    }
    lcd_writeStringSetCursor(au8LCDTronix, C0220BiZ_CONFIGURATION_FIRST_LINE);
    //lcd_writeStringSetCursor(au8LCDStartup, C0220BiZ_CONFIGURATION_SECOND_LINE);
*/
    // EUSART Display Strings.
    eusartWriteString(au8EUSARTTronix);
    eusartWriteString(au8EUSARTWWW);
    eusartWriteString(au8EUSARTReady);

    // INTERRUPTS FLAGS Settings.
    PIR0 = 0x00;
    PIR1 = 0x00;
    PIR2 = 0x00;
    PIR3 = 0x00;
    PIR4 = 0x00;
    PIR5 = 0x00;
    PIR6 = 0x00;
    PIR7 = 0x00;
    PIR8 = 0x00;
    // INTERRUPTS Settings.
    PIE0 = 0b00100000;
    PIE1 = 0b00000000;
    PIE2 = 0b00000000;
    PIE3 = 0b00000000;
    PIE4 = 0b00000000;
    PIE5 = 0b00000000;
    PIE6 = 0b00000000;
    PIE7 = 0b00000000;
    PIE8 = 0b00000000;
    // INTERRUPTS Enable.
    INTCONbits.GIE = 0b0;

    // PCM1795.
//    PCM1795_RESET = 0b0;
//    __delay_ms(5);
    PCM1795_RESET = 0b1;
    __delay_ms(1);
    
    uint8_t u8Buffer[4];
    uint8_t u8Read;
/*
    u8Read = pcm1795_readRegister(16);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    
    
    u8Read = pcm1795_readRegister(17);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    

    u8Read = pcm1795_readRegister(18);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    

    u8Read = pcm1795_readRegister(19);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);  

    u8Read = pcm1795_readRegister(20);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);

    u8Read = pcm1795_readRegister(21);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);  
    
    u8Read = pcm1795_readRegister(22);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);
    
    u8Read = pcm1795_readRegister(23);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);
*/
    //
    pcm1795_writeRegister(16, 160);
    pcm1795_writeRegister(17, 160);
    pcm1795_writeRegister(18, 0b11000000);
//    pcm1795_writeRegister(19, 0b00000000);
//    pcm1795_writeRegister(20, 0b00000000);
//    pcm1795_writeRegister(21, 0b00000001);

    //
    eusartWriteString(au8EUSARTReady);
    u8Read = pcm1795_readRegister(16);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    
    
    u8Read = pcm1795_readRegister(17);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    

    u8Read = pcm1795_readRegister(18);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);    

    u8Read = pcm1795_readRegister(19);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);  

    u8Read = pcm1795_readRegister(20);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);

    u8Read = pcm1795_readRegister(21);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);  
    
    u8Read = pcm1795_readRegister(22);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);
    
    u8Read = pcm1795_readRegister(23);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTReady);
    CAT4002_ENABLE = 0b1;

    
/*
    u8Read = pcm1795_readRegister(17);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTNL);
    
    u8Read = pcm1795_readRegister(18);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTNL);
    
    u8Read = pcm1795_readRegister(19);
    u8toASCII(u8Read, u8Buffer, 16);
    eusartWriteString(u8Buffer);
    eusartWriteString(au8EUSARTNL);
*/    

    while(1){
    }
}

// Functions.
uint8_t eusartReadCharacter(void)
{
    if(RC1STAbits.FERR){
        RC1STAbits.SPEN = 0b0;
        RC1STAbits.SPEN = 0b1;
    }

    if(RC1STAbits.OERR){
        RC1STAbits.CREN = 0b0;
        RC1STAbits.CREN = 0b1;
    }
    while(!PIR3bits.RCIF){};

    return(RC1REG);
}

void eusartWriteCharacter(uint8_t u8Data)
{
    while(!TX1STAbits.TRMT){};
    TX1REG = u8Data;
}

void eusartWriteString(const uint8_t * u8Data)
{
    while(*u8Data != '\0')
        eusartWriteCharacter(*u8Data++);
}

// todo rename mssp1ReadFLAGS
uint8_t mssp1Read(uint8_t u8ACKDT)
{
    while(SSP1CON2 & 0x1F){};
    SSP1CON2bits.RCEN = 0b1;
    while(!SSP1STATbits.BF){};
    SSP1CON2bits.ACKDT = u8ACKDT;
    SSP1CON2bits.ACKEN = 0b1;
    while(SSP1CON2bits.ACKEN){};

    return(SSP1BUF);
}

uint8_t mssp1ReadACK(void)
{
    while(SSP1CON2 & 0x1F){};
    SSP1CON2bits.RCEN = 0b1;
    while(!SSP1STATbits.BF){};
    SSP1CON2bits.ACKDT = 0b0;
    SSP1CON2bits.ACKEN = 0b1;
    while(SSP1CON2bits.ACKEN){};

    return(SSP1BUF);
}

uint8_t mssp1ReadNACK(void)
{
    while(SSP1CON2 & 0x1F){};
    SSP1CON2bits.RCEN = 0b1;
    while(!SSP1STATbits.BF){};
    SSP1CON2bits.ACKDT = 0b1;
    SSP1CON2bits.ACKEN = 0b1;
    while(SSP1CON2bits.ACKEN){};

    return(SSP1BUF);
}

void mssp1Restart(void)
{
    while(SSP1CON2 & 0x1F){};
    SSP1CON2bits.RSEN = 0b1;
    while(SSP1CON2bits.RSEN){};
}

void mssp1Start(void)
{
    SSP1CON1bits.SSPEN = 0b1;
    while(SSP1CON2 & 0x1F){};
    SSP1CON2bits.SEN = 0b1;
    while(SSP1CON2bits.SEN){};
}

void mssp1Stop(void)
{
    SSP1CON2bits.PEN = 0b1;
    while(SSP1CON2bits.PEN){};
    SSP1CON1bits.SSPEN = 0b0;
}

void mssp1Write(uint8_t u8Data)
{
    SSP1BUF = u8Data;
    while(SSP1STATbits.BF){};
    while(SSP1CON2bits.ACKSTAT){};
}

void lcd_setCursor(uint8_t u8Cursor)
{
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_NO_CONTINUOUS_COMMAND);
    mssp1Write(u8Cursor);
    mssp1Stop();
}

void lcd_writeCharacter(uint8_t u8Data)
{
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_NO_CONTINUOUS_DATA);
    mssp1Write(u8Data);
    mssp1Stop();
}

void lcd_writeInstruction(uint8_t u8Data)
{
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_NO_CONTINUOUS_COMMAND);
    mssp1Write(u8Data);
    mssp1Stop();
}

void lcd_writeString(const uint8_t * u8Data)
{
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_CONTINUOUS_DATA);
    while(*u8Data != '\0')
        mssp1Write(*u8Data++);
    mssp1Stop();
}

void lcd_writeStringSetCursor(const uint8_t * u8Data, uint8_t u8Cursor)
{
    mssp1Start();
    mssp1Write(C0220BiZ_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(ST7036_I2C_CONTROL_NO_CONTINUOUS_COMMAND);
    mssp1Write(u8Cursor);
    mssp1Write(ST7036_I2C_CONTROL_CONTINUOUS_DATA);
    while(*u8Data != '\0')
        mssp1Write(*u8Data++);
    mssp1Stop();
}

uint8_t pcm1795_readRegister(uint8_t u8Register)
{
    mssp1Start();
    mssp1Write(PCM1795_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(u8Register | PCM1795_READ);
    mssp1Restart();
    mssp1Write(PCM1795_CONFIGURATION_I2C_ADDRESS | I2C_READ);
    uint8_t u8Read = mssp1ReadNACK(); // todo corriger
    mssp1Stop();

    return(u8Read);
}

void pcm1795_writeRegister(uint8_t u8Register, uint8_t u8Data)
{
    mssp1Start();
    mssp1Write(PCM1795_CONFIGURATION_I2C_ADDRESS | I2C_WRITE);
    mssp1Write(u8Register | PCM1795_WRITE);
    mssp1Write(u8Data);
    mssp1Stop();
}

void u8toASCII(uint8_t u8Data, uint8_t * au8Buffer, uint8_t u8Base)
{
    uint8_t u8Buffer;
    uint8_t data = u8Data;

    do{
        data /= u8Base;
        au8Buffer++;
    }while(data != '\0');
    *au8Buffer-- = 0;

    do{
        u8Buffer = (u8Data % u8Base);
        u8Data /= u8Base;
        if(u8Buffer >= 10)
            u8Buffer += 0x07;
        u8Buffer += 0x30;
        *au8Buffer-- = u8Buffer;
    }while(u8Data != '\0');
}

/*
int8_t rotary_i8encoderFilter(void)
{
    int8_t i8encoderFilter;

    PIE0bits.TMR0IE = 0b0;
    i8encoderFilter = i8encoderDelta;
    i8encoderDelta = i8encoderFilter & 3;
    PIE0bits.TMR0IE = 0b1;

    return(i8encoderFilter >> 2);
}
*/