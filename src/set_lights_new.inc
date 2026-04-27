
; set_lights - sets all 5 LED pins according to parameters
; Parameters:
;   RED_REG       = r16   (on = 1, off = 0)
;   YELLOW_REG    = r17   (on = 1, off = 0)
;   GREEN_REG     = r18   (on = 1, off = 0)
;   DONTCROSS_REG = r19   (on = 1, off = 0)
;   CROSS_REG     = r20   (on = 1, off = 0)
;------------------------------------------------------
set_lights:
          ; RED - PD6
          tst       RED_REG               ; checks if register is 0
          breq      red_off               ; if (RED_REG is 0) -> branch
          sbi       PORTD, RED_LED        ; otherwise, turn led on
          rjmp      do_yellow
red_off:
          cbi       PORTD, RED_LED        ; turn off red led

do_yellow:
          ; YELLOW - PD5
          tst       YELLOW_REG            ; checks if register is 0
          breq      yellow_off            ; if (YELLOW_REG is 0) -> branch
          sbi       PORTD, YELLOW_LED     ; otherwise, turn led on
          rjmp      do_green
yellow_off:
          cbi       PORTD, YELLOW_LED     ; turn off yellow led

do_green:
          ; GREEN - PD4
          tst       GREEN_REG             ; checks if register is 0
          breq      green_off             ; if (GREEN_REG is 0) -> branch
          sbi       PORTD, GREEN_LED      ; otherwise, turn led on
          rjmp      do_dontcross
green_off:
          cbi       PORTD, GREEN_LED      ; turn off green led

do_dontcross:
          ; DONTCROSS - PB0
          tst       DONTCROSS_REG         ; checks if register is 0
          breq      dontcross_off         ; if (DONTCROSS_REG is 0) -> branch
          sbi       PORTB, DONT_CROSS_LED ; otherwise, turn led on
          rjmp      do_cross
dontcross_off:
          cbi       PORTB, DONT_CROSS_LED ; turn off dontcross led

do_cross:
          ; CROSS - PB1
          tst       CROSS_REG             ; checks if register is 0
          breq      cross_off             ; if (CROSS_REG is 0) -> branch
          sbi       PORTB, CROSS_LED      ; otherwise, turn led on
          rjmp      end_set_lights
cross_off:
          cbi       PORTB, CROSS_LED      ; turn off cross led

end_set_lights:
          ret