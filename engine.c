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

uint8_t* render_line(uint8_t line)
{
    uint8_t start_line;
    uint8_t render[8]; // 256 horizontal px
    uint8_t bits;

    // Sprites to draw may be as far as 7 lines above
    start_line = (line - 7 < 0) ? 0 : line - 7;

    for (i = start_line; i <= line; i++)
    {
        l = sprite_hash[i];
        while (l)
        {
            bits = l->s.tile[line - i];
            low_chunk = l->s.x / 8;
            hi_chunk = (l->s.x / 8) + 1;

            // Blit lower bits of sprite
            if (low_chunk < 8)
            {
                render[low_chunk] = bits << (sp.x % 8);
            }
            // Blit higher bits of sprite

}
