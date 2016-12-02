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

#ifndef IO_H
#define IO_H

/* Port B Pin Assignments (Arduino Port D8-D13) */
#define IO_ARDUINO_AMBER_LED        (1 << 5)

#define IO_PORTB_INIT               ((1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | \
                                    (1 << 0))
                                    
#define IO_DDRB_INIT                (IO_ARDUINO_AMBER_LED)


/* Port C Pin Assignments (Arduino Port A0-A7) */
#define IO_SWITCH_0                 (1 << 0)
#define IO_SWITCH_1                 (1 << 1)
#define IO_SWITCH_2                 (1 << 2)
#define IO_SWITCH_3                 (1 << 3)

#define IO_PORTC_INIT               ((1 << 5) | (1 << 4) | \
                                    IO_SWITCH_3 | IO_SWITCH_2 | IO_SWITCH_1 | \
                                    IO_SWITCH_0)
                                    
#define IO_DDRC_INIT                0


/* Port D Pin Assignments (Arduino Port D0-D7) */
#define IO_RX                       (1 << 0)
#define IO_TX                       (1 << 1)
#define IO_BACKLIGHT_0              (1 << 4)
#define IO_BACKLIGHT_1              (1 << 5)
#define IO_BACKLIGHT_2              (1 << 6)
#define IO_BACKLIGHT_3              (1 << 7)

#define IO_PORTD_INIT               ((1 << 3) | (1 << 2) | IO_TX | IO_RX)

#define IO_DDRD_INIT                (IO_BACKLIGHT_3 | IO_BACKLIGHT_2 | \
                                    IO_BACKLIGHT_1 | IO_BACKLIGHT_0)

#endif /* IO_H */
