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

#include <stdint.h>

/* Sprite data structure.
 *
 * Sprite coordinates are measured from the upper-left corner of the screen,
 * with (0,0) the top-left and (255, 255) the bottom-right.
 */
struct {
    uint8_t x;
    uint8_t y;
    uint8_t tile[8]; // 8x8 bits
} sprite;

struct {
    struct sprite *s;
    struct list_el *next;
} list_el;
    
struct list_el sprite_hash[256];

/* Returns uint8_t[32] for a total of 256 horizontal px. Bytes are to be
 * rendered left to right from render[31].msb -> render[0].lsb.
 */
uint8_t* render_line(uint8_t line)
{
    uint8_t start_line;
    uint8_t render[32]; // 256 horizontal px
    uint8_t bits;
    struct list_el l;
    struct sprite sp;

    // Sprites to draw may be as far as 7 lines above
    start_line = (line - 7 < 0) ? 0 : line - 7;

    for (i = start_line; i <= line; i++)
    {
        l = sprite_hash[i];
        while (l)
        {
            sp = l->s;
            bits = sp.tile[line - i];
            left_chunk = 31 - (sp.x / 8);
            right_chunk = left_chunk - 1;

            // Blit left bits of sprite
            if (left_chunk < 31)
            {
                render[left_chunk] |= bits >> (sp.x % 8);
            }
            // Blit higher bits of sprite
            if (right_chunk < 31)
            {
                render[right_chunk] |= bits << (8 - (sp.x % 8));
            }

            l = l->next;
        }
    }

    return render;
}
