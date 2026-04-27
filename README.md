# Assembly-Traffic-Light
This is a traffic light simulator implemented using AVR assembly language, running on an Arduino Uno R3 (ATmega328p). This project features a full state machine, a crosswalk button, an ON/OFF button, and five LED outputs.

**Team:** Simon Mekhail & Daniel Waterman

---

## Hardware

| Component | Pin |
|-----------|-----|
| Green LED | PD4 (D4) |
| Yellow LED | PD5 (D5) |
| Red LED | PD6 (D6) |
| Don't Cross LED | PB0 (D8) |
| Cross LED | PB1 (D9) |
| Crosswalk Button | PD2 (D2) — INT0 |
| ON/OFF Button | PD3 (D3) — INT1 |

All buttons use internal pull-ups (active LOW). LEDs are connected to GND via current-limiting resistors.

### Circuit Diagram

<img width="645" height="535" alt="image" src="https://github.com/user-attachments/assets/8a82d759-b95d-4129-91b9-f0bd68c8cc3c" />


---

## State Machine

```
RED -> ALLRED -> GREEN -> YELLOW -> ALLRED -> ...
                              ^
                              | (if crosswalk button pressed)
                           CROSS -> GREEN
```

| State | Red | Yellow | Green | Don't Cross | Cross |
|-------|-----|--------|-------|-------------|-------|
| RED | ON | off | off | off | off |
| ALLRED | ON | off | off | ON | off |
| GREEN | off | off | ON | ON | off |
| YELLOW | off | ON | off | ON | off |
| CROSS | ON | off | off | off | ON |

All states hold for `DELAY_SEC = 2` seconds. The crosswalk button can be pressed during RED or GREEN; the flag is checked at the ALLRED transition to decide whether to enter CROSS or return to GREEN.

---

## File Structure

```
src/
  final_project.asm       — Main program: setup, ISRs, main loop
  set_lights_new.inc      — Sets all 5 LED outputs based on register values
  enter_state_new.inc     — Loads LED parameters for a given state and calls set_lights
  state_manager_new.inc   — Blocking state machine: runs one state then transitions
  delay_new.inc           — Blocking delay (DELAY_REG seconds)

traffic_light_lab1.ino    — Original Arduino C++ prototype (Lab 1)

; Earlier lab exercises (reference only)
loops.asm
conditionals.asm
gpio.asm
main.asm
hwq1.asm / hwq2.asm / hwq3.asm
```

---

## How It Works
set_lights — Takes five register values (one per LED) and directly drives the corresponding PORT pins high or low. It is the only place in the program that touches the hardware output registers, so all LED changes go through here.

enter_state — Takes a state value in STATE_REG and loads the correct ON/OFF values for all five LEDs based on that state, then calls set_lights. Every state transition goes through this function to ensure LEDs always match the current state.

delay — Blocking delay that stalls the CPU for exactly DELAY_REG seconds using three nested countdown loops calibrated to the 16 MHz clock. Nothing else runs while it executes.

state_manager — The core of the FSM. Reads STATE_REG, calls delay for the appropriate duration, checks the crosswalk button flag if relevant, sets STATE_REG to the next state, and calls enter_state to apply it. Runs one complete state per call and returns to main_loop.

### Interrupts

- **INT0 (PD2)** — Crosswalk button ISR. Sets flag register `r22 = 1` on falling edge.
- **INT1 (PD3)** — ON/OFF button ISR. Toggles `r24` between 0 (off) and 1 (on). Resets to the RED state on turn-on.

Both ISRs save and restore SREG to avoid corrupting flags in the main loop.

### Delay

`delay_new.inc` uses three nested counting loops calibrated for a 16 MHz clock. `DELAY_REG` (r23) holds the number of seconds to wait.

### Main Loop

```
main_loop:
    if r24 == 0 -> turn all LEDs off, loop
    else        -> call state_manager, loop
```

`state_manager` runs one full state (enter + delay + check button + transition) then returns.

---

## Requirements Met (per project spec)

| Requirement | Implementation |
|-------------|----------------|
| ≥ 2 independent inputs | Crosswalk button (INT0), ON/OFF button (INT1) |
| Hardware timer | Blocking delay loop calibrated to 16 MHz (Timer 1 in `ledspeed.asm` prototype) |
| Hardware interrupt | INT0 (crosswalk), INT1 (on/off) |
| Hardware output | 5 LEDs on PORTD and PORTB |

---

## Building & Flashing

Open `src/final_project.asm` in **Microchip Studio (Atmel Studio)**. Set the device to **ATmega328P**. Build and upload via the Arduino Uno's USB connection or an ISP programmer.

The `.inc` files must be in the same directory as `final_project.asm` at build time.























