/**
 * Tic-Tac-Turtle arcade button <--> USB interface
 * 
 * https://github.com/nadhack/ttt-arcade-buttons
 * 
 * Copyright (c) 2016 Matthew T. Bucknall
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISIN
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <stdint.h>

#include "io.h"

#include "app-input.h"
#include "app-sequencer.h"
#include "app-serial.h"


int main(void)
{
    // disable all peripherals (those needed will be re-enabled later)
    power_all_disable();
    
    // initialize GPIO
    PORTB = IO_PORTB_INIT;
    DDRB = IO_DDRB_INIT;
    PORTC = IO_PORTC_INIT;
    DDRC = IO_DDRC_INIT;
    PORTD = IO_PORTD_INIT;
    DDRD = IO_DDRD_INIT;
    
    // initialize modules
    app_input_init();
    app_sequencer_init();
    app_serial_init();
    
    // prepare for main loop
    set_sleep_mode(SLEEP_MODE_IDLE);
    sei();
    
    // main loop
    for (;;)
    {
        wdt_reset();
        
        app_input_main();
        app_sequencer_main();
        app_serial_main();
        
        cli();
        sleep_enable();
        sei();
        sleep_cpu();
        sleep_disable();
    }
    
    // execution never gets here
    return 0;
}
