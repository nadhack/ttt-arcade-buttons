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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>

#include "io.h"

#include "app-input.h"
#include "app-serial.h"


#define DEBOUNCE_PERIOD         4


static volatile uint8_t m_debounce_counter;
static volatile uint8_t m_pending;


ISR(TIMER2_OVF_vect)
{
    if ( m_debounce_counter )
    {
        if ( --m_debounce_counter == 0 )
        {
            m_pending = 1;
        }
    }
}


ISR(PCINT1_vect)
{
    m_debounce_counter = DEBOUNCE_PERIOD;
}


static void send_event(void)
{
    app_serial_send_event((~PINC) & 0x0F);
}


void app_input_force_event(void)
{
    send_event();
}


void app_input_main(void)
{
    if ( m_pending )
    {
        m_pending = 0;
        send_event();
    }
}


void app_input_init(void)
{
    // configure TIMER2 to generate overflow interrupt @ ~244.14Hz
    power_timer2_enable();
    
    TCCR2B = (1 << CS22) | (1 << CS21);
    TIMSK2 = (1 << TOIE2);
    
    // enable pin-change interrupt 1 (Port C)
    PCICR |= (1 << PCIE1);
    
    // unmask pin-change detection on switch inputs
    PCMSK1 |= IO_SWITCH_3 | IO_SWITCH_2 | IO_SWITCH_1 | IO_SWITCH_0;
}
