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

uint8_t framebuffer[FRAMEBUFFERSIZE];
uint8_t *pos;
uint16_t line;

uint8_t video_init(void)
{
    uint16_t i;

    // Set up timer0 to the sync frequency (63.5us)
    // 63.5E-6 / (1/16E6) = 1016 = (127 * 8)
    // So set up prescalar = clk/8
    // and compare value = 127
    TCCR0 = 0x42;
    OCR0 = 127;
    TIMSK = 0x0;

    // Initialize the framebuffer
    for (i=0; i<FRAMEBUFFERSIZE; i++)
    {
        framebuffer[i] = 0;
    }

    return 1;
}

uint8_t video_enable(void)
{
    // Turn on timer0 to start the h_sync
    TIMSK = 0x2;
}

uint8_t video_disable(void)
{
    // Disable timer0
    TIMSK = 0x0;
}

// Sync interrupt _must_ be entered from sleep mode.
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
        // position = base + (line * (width/8))
        // 144 / 8 = (2^7 + 2^4) / 2^3 = 2^4 + 2^1
        pos = framebuffer + ((line - TOPLINE) << 4) + ((line - TOPLINE) << 1);
        _delay_us(12);

        // draw line
        byteblast();
    }

    line++;
}


// The following code is copied from the ECE476 website at
// http://instruct1.cit.cornell.edu/courses/ee476/video/videoGCC/video_144x200_GCC644.c
// Copyright 2009 Shane Pryor and Bruce Land
asm(".MACRO videobits\n\t"

    "BST  R4,7\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,6\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,5\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,4\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,3\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,2\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,1\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    //"NOP \n\t" 
    "NOP \n\t"
    "BST  R4,0\n\t"
    "BLD  R30,5\n\t"
    "OUT  0x0b,R30\n\t"

    ".ENDM\n\t"
);

// puts 18 bytes (1 line) to the screen
void byteblast() {
    asm("LDS  R26,pos\n\t"
        "LDS  R27,pos+1\n\t"
            
        "LDI R30, 0x40\n\t"		

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X+\n\t"
        "videobits\n\t"

        "LD  R4,X\n\t"
        "videobits\n\t"

        "CLT\n\t"
        "LDI R30, 0x40\n\t"

        "BLD  R30,5\n\t"
        "OUT  0x0b,R30\n\t"	
    );
}

