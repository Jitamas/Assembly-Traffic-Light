
; state_manager - blocking state machine, runs one full state then transitions
; Parameters: none
; Uses: DELAY_REG/r23 (delay), STATE_REG/r21 (state), r22 (button flag)
; State values: 0=GREEN, 1=YELLOW, 2=ALLRED, 3=RED, 4=CROSS
;------------------------------------------------------
state_manager:
          cpi       STATE_REG, STATE_GREEN  ; check if current state is GREEN
          breq      tick_green              ; if GREEN -> branch

          cpi       STATE_REG, STATE_YELLOW ; check if current state is YELLOW
          breq      tick_yellow             ; if YELLOW -> branch

          cpi       STATE_REG, STATE_ALLRED ; check if current state is ALLRED
          breq      tick_allred             ; if ALLRED -> branch

          cpi       STATE_REG, STATE_RED    ; check if current state is RED
          breq      tick_red                ; if RED -> branch

          cpi       STATE_REG, STATE_CROSS  ; check if current state is CROSS
          breq      tick_cross              ; if CROSS -> branch

          rjmp      end_state_manager       ; unknown state, do nothing

tick_green:
          ldi       DELAY_REG, DELAY_SEC    ; GREEN delay
          rcall     delay                   ; wait

          sbis      PIND, BUTTON_CROSS      ; check crosswalk button (PD2)
          ldi       r22, 1                  ; button pressed -> set flag

          ldi       STATE_REG, STATE_YELLOW ; next state = YELLOW
          rcall     enter_state
          rjmp      end_state_manager

tick_yellow:
          ldi       DELAY_REG, DELAY_SEC    ; YELLOW delay
          rcall     delay                   ; wait

          ldi       STATE_REG, STATE_ALLRED ; next state = ALLRED
          rcall     enter_state
          rjmp      end_state_manager

tick_allred:
          ldi       DELAY_REG, DELAY_SEC    ; ALLRED delay
          rcall     delay                   ; wait

          tst       r22                     ; check button flag
          breq      go_green                ; flag = 0 -> go to GREEN

          ldi       STATE_REG, STATE_CROSS  ; flag = 1 -> next state = CROSS
          clr       r22                     ; clear button flag
          rcall     enter_state
          rjmp      end_state_manager

go_green:
          ldi       STATE_REG, STATE_GREEN  ; next state = GREEN
          rcall     enter_state
          rjmp      end_state_manager

tick_red:
          ldi       DELAY_REG, DELAY_SEC    ; RED delay
          rcall     delay                   ; wait

          sbis      PIND, BUTTON_CROSS      ; check crosswalk button (PD2)
          ldi       r22, 1                  ; button pressed -> set flag

          ldi       STATE_REG, STATE_ALLRED ; next state = ALLRED
          rcall     enter_state
          rjmp      end_state_manager

tick_cross:
          ldi       DELAY_REG, DELAY_SEC    ; CROSS delay
          rcall     delay                   ; wait

          ldi       STATE_REG, STATE_GREEN  ; next state = GREEN
          rcall     enter_state

end_state_manager:
          ret