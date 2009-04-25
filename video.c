// Copyright 2009 Devrin Talen
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.

#include "video.h"
#include "engine.h"

#define WIDTH 144
#define HEIGHT 200
// buffer size = (width / 8) * height = 144/8 * 200 = 3600
#define FRAMEBUFFERSIZE 3600

#define TOPLINE 30
#define BOTLINE 230
#define V_SYNC_START 243
#define V_SYNC_END 262
#define LASTLINE 262

#define VIDEO PORTA
#define H_SYNC 0x0
#define H_NOSYNC 0x1
#define V_SYNC 0x1
#define V_NOSYNC 0x0

uint16_t line;

void video_init(void)
{
    uint16_t i;

    // Set up timer0 to the sync frequency (63.5us)
    // 63.5E-6 / (1/16E6) = 1016 = (127 * 8)
    // So set up prescalar = clk/8
    // and compare value = 127
    TCCR0 = 0x42;
    OCR0 = 127;
    TIMSK = 0x0;

    return 1;
}

void video_enable(void)
{
    // Turn on timer0 to start the h_sync
    TIMSK = 0x2;
}

void video_disable(void)
{
    // Disable timer0
    TIMSK = 0x0;
}

/* Horizontal sync interrupt.
 *
 * This must be entered from sleep mode or else the timing will be thrown off.
 */
ISR(TIMER0_COMP_vect)
{
    // generate sync pulse (5us)
    VIDEO = (line < V_SYNC_START) ? H_SYNC : V_SYNC;
    _delay_us(5);
    VIDEO = (line < V_SYNC_START) ? H_NOSYNC : V_NOSYNC;

    // start a new frame after line 262
    if (line == BOTLINE)
        line = 0;

    // blast line buffer to screen
    if ((line > TOPLINE) && (line < BOTLINE))
    {
        _delay_us(12);

        // draw line
        byteblast(render_line((uint8_t)line));
    }

    line++;
}

void byteblast(uint8_t bytes[])
{
    uint8_t *top = &bytes[32];

    asm(    "lds r26, top\n\t"
            "lds r27, top+1\n\t"

            // load byte and render
            ".MACRO loadrender\n\t"
            "ld %1, -x\n\t"
            "blastbits\n\t"
            ".ENDM\n\t"

            // render byte
            ".MACRO blastbits\n\t"
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            "lsl %1\n\t" 
            "out %0, %1\n\t"
            ".ENDM"

            // run 32 times...
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            "loadrender\n\t"
            :: "I" (_SFR_IO_ADDR(PORTA))
            : "r"
            : "top"
            );
    return;
}
