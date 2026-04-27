
; enter_state - loads LED parameters and calls set_lights
; Parameters:
;   STATE_REG     = r21   (0=GREEN, 1=YELLOW, 2=ALLRED, 3=RED, 4=CROSS)
;   RED_REG       = r16   (on = 1, off = 0) -- same registers used by set_lights
;   YELLOW_REG    = r17   (on = 1, off = 0)
;   GREEN_REG     = r18   (on = 1, off = 0)
;   DONTCROSS_REG = r19   (on = 1, off = 0)
;   CROSS_REG     = r20   (on = 1, off = 0)
;------------------------------------------------------
enter_state:
          cpi       STATE_REG, STATE_GREEN  ; if state = GREEN -> branch
          breq      state_grn

          cpi       STATE_REG, STATE_YELLOW ; if state = YELLOW -> branch
          breq      state_yell

          cpi       STATE_REG, STATE_ALLRED ; if state = ALLRED -> branch
          breq      state_allr

          cpi       STATE_REG, STATE_RED    ; if state = RED -> branch
          breq      state_r

          cpi       STATE_REG, STATE_CROSS  ; if state = CROSS -> branch
          breq      state_crs

          rjmp      end_enter_state         ; unknown state, do nothing

state_grn:
          ldi       RED_REG, LED_OFF        ; RED off
          ldi       YELLOW_REG, LED_OFF     ; YELLOW off
          ldi       GREEN_REG, LED_ON       ; GREEN on
          ldi       DONTCROSS_REG, LED_ON   ; DONTCROSS on
          ldi       CROSS_REG, LED_OFF      ; CROSS off
          rcall     set_lights
          rjmp      end_enter_state

state_yell:
          ldi       RED_REG, LED_OFF        ; RED off
          ldi       YELLOW_REG, LED_ON      ; YELLOW on
          ldi       GREEN_REG, LED_OFF      ; GREEN off
          ldi       DONTCROSS_REG, LED_ON   ; DONTCROSS on
          ldi       CROSS_REG, LED_OFF      ; CROSS off
          rcall     set_lights
          rjmp      end_enter_state

state_allr:
          ldi       RED_REG, LED_ON         ; RED on
          ldi       YELLOW_REG, LED_OFF     ; YELLOW off
          ldi       GREEN_REG, LED_OFF      ; GREEN off
          ldi       DONTCROSS_REG, LED_ON   ; DONTCROSS on
          ldi       CROSS_REG, LED_OFF      ; CROSS off
          rcall     set_lights
          rjmp      end_enter_state

state_r:
          ldi       RED_REG, LED_ON         ; RED on
          ldi       YELLOW_REG, LED_OFF     ; YELLOW off
          ldi       GREEN_REG, LED_OFF      ; GREEN off
          ldi       DONTCROSS_REG, LED_OFF  ; DONTCROSS off
          ldi       CROSS_REG, LED_OFF      ; CROSS off
          rcall     set_lights
          rjmp      end_enter_state

state_crs:
          ldi       RED_REG, LED_ON         ; RED on
          ldi       YELLOW_REG, LED_OFF     ; YELLOW off
          ldi       GREEN_REG, LED_OFF      ; GREEN off
          ldi       DONTCROSS_REG, LED_OFF  ; DONTCROSS off
          ldi       CROSS_REG, LED_ON       ; CROSS on
          rcall     set_lights

end_enter_state:
          ret