
; delay - blocking delay of DELAY_REG seconds
; Parameters:
;   DELAY_REG = r23   (number of seconds to delay)
; Uses: r16 (inner loop k), r17 (inner loop j), r23 (outer loop seconds counter)
;------------------------------------------------------
delay:
          ; DELAY_REG (r23) = number of seconds to wait
delay_seconds:
          push      r23                   ; save seconds remaining
          ldi       r23, 64               ; 64 outer iterations = 1 second
delay_l3:
          ldi       r17, 200
delay_l2:
          ldi       r16, 250
delay_l1:
          nop                             ;            (1cy)
          nop                             ;            (1cy)
          dec       r16                   ;            (1cy)
          brne      delay_l1              ;         }  (2cy : 1cy)

          dec       r17
          brne      delay_l2              ;    }

          dec       r23
          brne      delay_l3              ; } (1 second elapsed)

          pop       r23                   ; restore seconds remaining
          dec       r23                   ; one second done
          brne      delay_seconds         ; repeat for each remaining second

          ret