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

#include <stdbool.h>

#include "app-sequencer.h"
#include "app-serial.h"


#define BUFFER_SIZE     (APP_SEQUENCER_SEQUENCE_LENGTH + 1)


static volatile uint8_t m_pending;
static char m_buffer[BUFFER_SIZE];
static uint8_t m_buffer_in;
static bool m_invalid;


static bool is_digit(char c)
{
    return c >= '0' && c <= '9';
}


static bool is_upper_hex(char c)
{
    return c >= 'A' && c <= 'F';
}


static bool is_lower_hex(char c)
{
    return c >= 'a' && c <= 'f';
}


static bool is_hex(char c)
{
    return is_digit(c) || is_upper_hex(c) || is_lower_hex(c);
}


ISR(USART_RX_vect)
{
    uint8_t c = (char) UDR0;
    uint8_t status = UCSR0A;
    
    if ( m_pending || status & (1 << FE0) ) return;
    
    if ( c == '\r' )
    {
        if ( m_invalid || m_buffer_in < BUFFER_SIZE )
        {
            m_invalid = false;
            m_buffer_in = 0;
        }
        else
        {
            m_pending = true;
            m_buffer_in = 0;
        }
    }
    else if ( c >= ' ' && !m_invalid )
    {
        if ( is_hex(c) && m_buffer_in < BUFFER_SIZE )
        {
            m_buffer[m_buffer_in++] = c;
        }
        else
        {
            m_invalid = true;
        }
    } 
}


void app_serial_send_event(uint8_t state)
{
    state &= 0xF;
    
    while ( !(UCSR0A & (1 << UDRE0)) );
    UDR0 = (state < 10) ? (state + '0') : (state + 'A' - 10);
}


static uint8_t decode_hex(char c)
{
    if ( is_digit(c) ) return c - '0';
    else if ( is_upper_hex(c) ) return c - 'A' + 10;
    else if ( is_lower_hex(c) ) return c - 'a' + 10;
    else return 0;
}


void app_serial_main(void)
{
    if ( m_pending )
    {
        AppSequencerPattern p;
        const char* c = m_buffer;
        
        p.speed = decode_hex(*c++);
        
        for (uint8_t i = 0; i < APP_SEQUENCER_SEQUENCE_LENGTH; i++)
        {
            p.steps[i] = decode_hex(*c++);
        }
        
        app_sequencer_set_pattern(&p);
        
        m_pending = 0;
    }
}


void app_serial_init(void)
{
    // configure USART0 for 38400n81 operation
    power_usart0_enable();
    
    UBRR0 = 25;
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    UCSR0B = (1 << RXCIE0) | (1 << RXEN0) | (1 << TXEN0);
    
    m_pending = 1;
}
