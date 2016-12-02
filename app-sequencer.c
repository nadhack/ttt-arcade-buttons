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

#include "app-sequencer.h"


static volatile uint8_t m_speed;
static volatile uint8_t m_index;
static uint8_t m_steps[APP_SEQUENCER_SEQUENCE_LENGTH];


ISR(TIMER1_COMPA_vect)
{
    static uint8_t divider;
    
    if ( ++divider >= m_speed )
    {
        divider = 0;
        if ( ++m_index >= APP_SEQUENCER_SEQUENCE_LENGTH ) m_index = 0;
    }
}


void app_sequencer_set_pattern(const AppSequencerPattern* pattern)
{
    if ( pattern )
    {
        m_speed = pattern->speed;
        
        for (uint8_t i = 0; i < APP_SEQUENCER_SEQUENCE_LENGTH; i++)
        {
            m_steps[i] = pattern->steps[i] << 4;
        }
    }
    else
    {
        m_speed = 0xF;
        
        for (uint8_t i = 0; i < APP_SEQUENCER_SEQUENCE_LENGTH; i++)
        {
            m_steps[i] = 0;
        }
    }
}


void app_sequencer_main(void)
{
    PORTD = (PORTD & 0x0F) | m_steps[m_index];
}


void app_sequencer_init(void)
{
    // configure TIMER1 to generate COMPA interrupts @ 20Hz
    power_timer1_enable();
    
    OCR1A = 3124;
    TCCR1B = (1 << WGM12) | (1 << CS12);
    TIMSK1 = (1 << OCIE1A);
}
