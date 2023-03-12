; Configuration Registers.


#include <xc.inc>
; PIC16F18877 - Compile with PIC-AS(v2.40).
; PIC16F18877 - @4MHz Internal Oscillator.
; -preset_vec=0000h, -pintentry=0004h, -pcinit=0005h, -pstringtext=3F00h.

;

; GPR BANK0.
PSECT cstackBANK0,global,class=BANK0,space=1,delta=1,noexec
u8BANK0:    DS	1
u8delay:    DS	1


; Common RAM.
psect cstackCOMMON,global,class=COMMON,space=1,delta=1,noexec
u8COMMON:  DS  1
  
; MCU Definitions.
; BANKS.
#define	BANK0	    0x0
#define	BANK1	    0x1
#define	BANK2	    0x2
#define	BANK3	    0x3
#define	BANK4	    0x4
#define	BANK5	    0x5
#define	BANK6	    0x6
#define	BANK7	    0x7
#define	BANK9	    0x9
#define	BANK11	    0xB
#define	BANK12	    0xC
#define	BANK14	    0xE
#define BANK15	    0xF
#define	BANK16	    0x10
#define	BANK17	    0x11
#define	BANK18	    0x12
#define	BANK19	    0x13
#define	BANK31	    0x1F
#define BANK60	    0x3C
#define BANK61	    0x3D
#define BANK62	    0x3E
#define BANK63	    0x3F
; SFR - Special Function Registers.
; SFR STATUS Bits.
#define	C	    0x0
#define	Z	    0x2

; USER Definitions.
#define LED_DEBUG	LATA, 0x6
; SRC4392.

; Reset Vector.
PSECT reset_vec,global,class=CODE,merge=1,delta=2
resetVector:
    GOTO    main

; ISR Vector.
PSECT intentry,global,class=CODE,space=0,delta=2
interruptVector:
    GOTO    isr

; Main.
PSECT cinit,global,class=CODE,merge=1,delta=2
main:
    ; MCU Initialization.
    ; Internal Oscillator Settings.
    MOVLB   BANK17
    MOVLW   0b00000000
    MOVWF   OSCTUNE
    MOVLW   0b00000010
    MOVWF   OSCFRQ
;    BTFSS   HFOR
;    BRA	    $-1
    ; Ports Settings.
    ; PORT Data Register.
    MOVLB   BANK0
    CLRF    PORTA
    CLRF    PORTB
    CLRF    PORTC
    ; TRIS Data Direction.
    MOVLW   0b00000000
    MOVWF   TRISA
    MOVLW   0b00000000
    MOVWF   TRISB
    MOVLW   0b00000000
    MOVWF   TRISC
    ; LATCH Outputs.
    CLRF    LATA
    CLRF    LATB
    CLRF    LATC
    ; PORTA Settings.
    MOVLB   BANK62
    CLRF    ANSELA
    CLRF    WPUA
    CLRF    ODCONA
    MOVLW   0b11111111
    MOVWF   SLRCONA
    CLRF    INLVLA
    CLRF    IOCAP
    CLRF    IOCAN
    CLRF    IOCAF
    ; PORTB Settings.
    CLRF    ANSELB
    CLRF    WPUB
    CLRF    ODCONB
    MOVLW   0b11111111
    MOVWF   SLRCONB
    CLRF    INLVLB
    CLRF    IOCBP
    CLRF    IOCBN
    CLRF    IOCBF
    ; PORTC Settings.
    CLRF    ANSELC
    CLRF    WPUC
    CLRF    ODCONC
    MOVLW   0b11111111
    MOVWF   SLRCONC
    CLRF    INLVLC
    CLRF    IOCCP
    CLRF    IOCCN
    CLRF    IOCCF
    ; PPS Settings.
    ; PPS Sequence Enable.
;    MOVLB   BANK61
;    MOVLW   0x55
;    MOVWF   PPSLOCK
;    MOVLW   0xAA
;    MOVWF   PPSLOCK
;    BCF	    PPSLOCK, 0x0

loop:

    BRA	    loop

; Interrupt Service Routine.
isr:
    RETFIE

; u8Delay = 1 ~775µs.
; u8Delay = 255 ~198ms.
_u8Delay:
    MOVLB   BANK0
    MOVWF   u8delay
    MOVLW   255
    DECFSZ  WREG, F
    BRA	    $-1
    DECFSZ  u8delay, F
    BRA	    $-3
    RETURN

; PFM Strings.
;;PSECT	stringtext,class=CONST,space=0,delta=2
;PSECT	stringtext,class=STRCODE,space=0,delta=2

    END resetVector