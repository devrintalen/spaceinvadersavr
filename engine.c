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
    uint8_t tile[8]; // up to 8x8 bits
    sprite_t *next;
} sprite;

uint8_t* render_line(uint8_t line)
{
}
