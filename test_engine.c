#include <stdlib.h>
#include <stdio.h>
#include "engine.h"

int main(void)
{
    sprite_t sp1, sp2, sp3;
    uint8_t sp1data[8] = {
        0xee, //0b11101110,
        0x44, //0b01000100,
        0x44, //0b01000100,
        0x28, //0b00101000,
        0x28, //0b00101000,
        0x10, //0b00010000,
        0x10, //0b00010000,
        0x60 //0b01100000
        };
    uint8_t sp2data[8] = {
        0x7c, //0b01111100,
        0x82, //0b10000010,
        0x82, //0b10000010,
        0x82, //0b10000010,
        0x82, //0b10000010,
        0x7c, //0b01111100,
        0x0, //0b00000000,
        0x0 //0b00000000
        };
    uint8_t sp3data[8] = {0};
    uint8_t* line;
    int8_t i, j;

    printf("Beginning test.\n");

    printf("Creating sprite 1...\n");
    sp1 = create_sprite(0, 0, sp1data);
    printf("Creating sprite 2...\n");
    sp2 = create_sprite(8, 0, sp2data);
    printf("Creating sprite 3...\n");
    sp3 = create_sprite(16, 0, sp3data);

    printf("Rendering...\n");
    for (i=0; i<8; i++)
    {
        line = render_line(i);
        for (j=31; j>=0; j--)
        {
            printf("%02x", line[j]);
        }
        printf("\n");
    }

    printf("Garbage cleaning...\n");
    delete_sprite(sp3);
    delete_sprite(sp2);
    delete_sprite(sp1);

    printf("End of test.\n");

    return 0;
}
