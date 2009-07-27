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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/* Sprite data structure.
 *
 * Sprite coordinates are measured from the upper-left corner of the screen,
 * with (0,0) the top-left and (255, 255) the bottom-right.
 */
struct sprite {
    uint8_t x;
    uint8_t y;
    uint8_t tile[8]; // 8x8 bits
};

typedef struct node* node_t;
struct node {
    sprite_t s;
    struct node* next;
};

node_t sprite_hash[256];

sprite_t create_sprite(uint8_t x, uint8_t y, uint8_t tile[])
{
    sprite_t sp;
    node_t n;
    node_t iter;
   
    sp = malloc(sizeof(struct sprite));
    n = malloc(sizeof(struct node));
    assert(sp && n);

    sp->x = x;
    sp->y = y;
    memcpy(sp->tile, tile, 8);

    n->s = sp;
    n->next = NULL;

    iter = sprite_hash[sp->y];
    if (iter)
    {
        while (iter->next) {
            iter = iter->next;
        }
        iter->next = n;
    }
    else
    {
        sprite_hash[sp->y] = n;
    }

    return sp;
}

void delete_sprite(sprite_t s)
{
    node_t prev, iter;

    assert(s);
    assert(sprite_hash[s->y]);

    prev = sprite_hash[s->y];
    iter = prev->next;
    if (prev->s == s)
    {
        sprite_hash[s->y] = prev->next;
        iter = prev;
    }
    while ((iter->s != s) && iter)
    {
        prev = iter;
        iter = iter->next;
    }
    assert(iter->s == s);
    prev->next = iter->next;
    free(iter);
    free(s);
}

/* Setter/getter methods for sprites. Abstracts sprite details from outside
 * code.
 */
void sprite_set_x(sprite_t s, uint8_t x)
{
    s->x = x;
}
void sprite_set_y(sprite_t s, uint8_t y)
{
    s->y = y;
}
void sprite_set_data(sprite_t s, uint8_t data[])
{
    memcpy(data, s->tile, 8);
}
uint8_t sprite_get_x(sprite_t s)
{
    return s->x;
}
uint8_t sprite_get_y(sprite_t s)
{
    return s->y;
}
uint8_t* sprite_get_data(sprite_t s)
{
    return s->tile;
}

/* Returns uint8_t[32] for a total of 256 horizontal px. Bytes are to be
 * rendered left to right from render[31].msb -> render[0].lsb.
 */
uint8_t* render_line(uint8_t line)
{
    uint8_t start_line;
    uint8_t* render = malloc(32 * sizeof(uint8_t));
    uint8_t bits, left_chunk, right_chunk;
    uint8_t i;
    sprite_t sp;
    node_t iter;

    // Sprites to draw may be as far as 7 lines above
    start_line = (line - 7 < 0) ? 0 : line - 7;

    for (i = start_line; i <= line; i++)
    {
        iter = sprite_hash[i];
        while (iter)
        {
            sp = iter->s;
            bits = sp->tile[line - i];
            left_chunk = 31 - (sp->x / 8);
            right_chunk = left_chunk - 1;

            // Blit left bits of sprite
            if (left_chunk < 31)
            {
                render[left_chunk] |= bits >> (sp->x % 8);
            }
            // Blit higher bits of sprite
            if (right_chunk < 31)
            {
                render[right_chunk] |= bits << (8 - (sp->x % 8));
            }

            iter = iter->next;
        }
    }

    return render;
}
