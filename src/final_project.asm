;traffic_light.asm
;
; Created: 4/8/2026 5:26:35 PM
; Author : Simon Mekhail, Hansel Echavarria, Daniel Waterman
; Desc: A traffic light simulator state machine with a crosswalk button and ON/OFF button
;--------------------------------------------------

; LEDs
.equ GREEN_LED      = 4             ; PD4
.equ YELLOW_LED     = 5             ; PD5
.equ RED_LED        = 6             ; PD6
.equ DONT_CROSS_LED = 0             ; PB0
.equ CROSS_LED      = 1             ; PB1

; Buttons
.equ BUTTON_CROSS   = 2             ; PD2
.equ BUTTON_ONOFF   = 3             ; PD3

; Delay
.equ DELAY_SEC      = 2             ; delay in seconds for all states

; State values
.equ STATE_GREEN    = 0
.equ STATE_YELLOW   = 1
.equ STATE_ALLRED   = 2
.equ STATE_RED      = 3
.equ STATE_CROSS    = 4

; LED on/off values
.equ LED_ON         = 1
.equ LED_OFF        = 0

; Register definitions
.def STATE_REG      = r21
.def RED_REG        = r16
.def YELLOW_REG     = r17
.def GREEN_REG      = r18
.def DONTCROSS_REG  = r19
.def CROSS_REG      = r20
.def DELAY_REG      = r23


; Vector Table
;-----------------
.org 0x000
          jmp       main

.org      INT0addr
          jmp       cross_button_ISR

.org      INT1addr
          jmp       onoff_button_ISR

.org      INT_VECTORS_SIZE

.include "set_lights_new.inc"     ; include the set_lights function
.include "enter_state_new.inc"    ; include the enter_state function
.include "delay_new.inc"          ; include the delay function
.include "state_manager_new.inc"  ; include the state_manager function


main:
; Initialize stack pointers
; Initialize LED pins as outputs, all off
; Initialize button pins as inputs
; GREEN  = PD4, YELLOW = PD5, RED = PD6
; DONTCROSS = PB0, CROSS = PB1
; CROSSWALK BUTTON = PD2
; ON/OFF BUTTON = PD3
;-------------------------------------------------------
          ldi       r20, high(RAMEND)     ; initialize high stack pointer
          out       SPH, r20
          ldi       r20, low(RAMEND)      ; initialize low stack pointer
          out       SPL, r20

          sbi       DDRD, GREEN_LED       ; set GREEN LED (PD4) to output
          cbi       PORTD, GREEN_LED      ; GREEN off

          sbi       DDRD, YELLOW_LED      ; set YELLOW LED (PD5) to output
          cbi       PORTD, YELLOW_LED     ; YELLOW off

          sbi       DDRD, RED_LED         ; set RED LED (PD6) to output
          cbi       PORTD, RED_LED        ; RED off

          sbi       DDRB, DONT_CROSS_LED  ; set DONTCROSS LED (PB0) to output
          cbi       PORTB, DONT_CROSS_LED ; DONTCROSS off

          sbi       DDRB, CROSS_LED       ; set CROSS LED (PB1) to output
          cbi       PORTB, CROSS_LED      ; CROSS off

          ; crosswalk button (PD2) - input pull-up, falling-edge interrupt
          cbi       DDRD, BUTTON_CROSS    ; input mode
          sbi       PORTD, BUTTON_CROSS   ; pull-up mode

          ; on/off button (PD3) - input pull-up, falling-edge interrupt
          cbi       DDRD, BUTTON_ONOFF    ; input mode
          sbi       PORTD, BUTTON_ONOFF   ; pull-up mode

          ldi       r20, (0b10 << ISC00) | (0b10 << ISC10) ; falling-edge for INT0 and INT1
          sts       EICRA, r20            ; set sense bits
          sbi       EIMSK, INT0           ; enable INT0 (crosswalk)
          sbi       EIMSK, INT1           ; enable INT1 (on/off)

          clr       r22                   ; initialize cross button flag
          ldi       r24, 1                ; initialize on/off flag = ON

          ldi       STATE_REG, STATE_RED  ; initial state = RED
          rcall     enter_state           ; call enter_state

          sei                             ; enable global interrupts


main_loop:
          tst       r24                   ; check on/off flag
          breq      main_off              ; if off -> skip state machine
          rcall     state_manager         ; call state_manager function
          rjmp      main_loop             ; loop

main_off:
          cbi       PORTD, GREEN_LED      ; GREEN off
          cbi       PORTD, YELLOW_LED     ; YELLOW off
          cbi       PORTD, RED_LED        ; RED off
          cbi       PORTB, DONT_CROSS_LED ; DONTCROSS off
          cbi       PORTB, CROSS_LED      ; CROSS off
          rjmp      main_loop             ; keep looping until turned on


cross_button_ISR:
          push      r20                   ; save r20
          in        r20, SREG             ; save SREG
          push      r20                   ; push SREG onto stack
          ldi       r22, 1                ; button pressed -> set flag
          pop       r20                   ; pop SREG from stack
          out       SREG, r20             ; restore SREG
          pop       r20                   ; restore r20
          reti


onoff_button_ISR:
          push      r20                   ; save r20
          in        r20, SREG             ; save SREG
          push      r20                   ; push SREG onto stack

          tst       r24                   ; check current on/off state
          breq      onoff_turnon          ; if OFF -> turn ON
          clr       r24                   ; if ON  -> turn OFF
          rjmp      onoff_done

onoff_turnon:
          ldi       r24, 1                ; turn ON
          ldi       STATE_REG, STATE_RED  ; reset to RED state on turn-on

onoff_done:
          pop       r20                   ; pop SREG from stack
          out       SREG, r20             ; restore SREG
          pop       r20                   ; restore r20
          reti



