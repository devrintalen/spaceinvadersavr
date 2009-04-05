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

#define WIDTH 144
#define HEIGHT 200

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

uint8_t framebuffer[WIDTH][HEIGHT];
uint8_t pos;
uint16_t line;

uint8_t init(void)
{
    uint8_t i,j;

    // Set up timer0 to the sync frequency (63.5us)
    // 63.5E-6 / (1/16E6) = 1016 = (127 * 8)
    // So set up prescalar = clk/8
    // and compare value = 127
    TCCR0 = 0x42;
    OCR0 = 127;
    TIMSK = 0x0;

    // Initialize the framebuffer
    for (i=0; i<WIDTH; i++)
    {
        for (j=0; j<HEIGHT; j++)
        {
            framebuffer[i][j] = 0;
        }
    }

    return 1;
}

uint8_t enable(void)
{
    // Turn on timer0 to start the h_sync
    TIMSK = 0x2;
}

uint8_t disable(void)
{
    // Disable timer0
    TIMSK = 0x0;
}

uint8_t set_pixel(uint8_t x, uint8_t y)
{
    if ((x >= WIDTH) || (y >= HEIGHT))
    {
        return 0;
    }

    framebuffer[x][y] = 1;
    
    return 1;
}

// vertical sync interrupt
ISR(TIMER0_COMP_vect)
{
    // generate sync pulse (5us)
    VIDEO = (line < V_SYNC_START) ? V_SYNC : H_SYNC;
    _delay_us(5);
    VIDEO = (line < V_SYNC_START) ? V_NOSYNC : H_NOSYNC;

    // start a new frame after line 262
    if (line == BOTLINE)
        line = 0;

    // blast line buffer to screen
    if ((line > TOPLINE) && (line < BOTLINE))
    {
        // TODO
    }

    line++;
}
