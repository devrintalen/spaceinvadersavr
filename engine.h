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

typedef struct sprite* sprite_t;

// Sprite operations
sprite_t create_sprite(uint8_t x, uint8_t y, uint8_t tile[]);
void delete_sprite(sprite_t s);
// Setters/getters
void sprite_set_x(sprite_t s, uint8_t x);
void sprite_set_y(sprite_t s, uint8_t y);
void sprite_set_data(sprite_t s, uint8_t data[]);
uint8_t sprite_get_x(sprite_t s);
uint8_t sprite_get_y(sprite_t s);
uint8_t* sprite_get_data(sprite_t s);

// Video interface
uint8_t* render_line(uint8_t line);
