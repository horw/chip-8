#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>


#define WIDTH 64
#define HEIGHT 32

uint8_t memory[4096];
uint16_t start_region = 0x200;
uint16_t I = 0;

uint8_t screen[WIDTH * HEIGHT];

void out_screen() {
    for (uint8_t i = 0; i < HEIGHT; i++) {
        for (uint8_t j = 0; j < WIDTH; j++) {
            printf("%d", screen[i * WIDTH + j]);
        }
        printf("\n");
    }
}

// 00E0 (clear screen)
// 1NNN (jump)
// 6XNN (set register VX)
// 7XNN (add value to register VX)
// ANNN (set index register I)
// DXYN (display/draw)
uint8_t* PC;

uint8_t V[16];


int main() {
    FILE* fptr = fopen("IBM Logo.ch8", "r");
    PC = &memory[start_region];

    
    memset(memory, 0, sizeof(memory));
    memset(V, 0, sizeof(V));
    memset(screen, 0, sizeof(screen));

    uint8_t* pnt = &memory[start_region];
    fgets(pnt, 4096, fptr);
    
    while (true){
        uint16_t command = *PC << 8 | *(PC+1);
        PC += 2;
        uint16_t N = command & 0xF;
        uint16_t NN = command & 0xFF;
        uint16_t NNN = command & 0xFFF;
        uint16_t F = (command>>12) & 0xF;
        
        uint8_t X = (command>>8) & 0xF;
        uint8_t Y = (command>>4) & 0xF;
        // printf("%d\n",F);
        switch (F)
        {
            case 0:
                memset(screen, 0, sizeof(screen));
                break;
            case 1:
                PC = &memory[NNN];
                break;
            case 6:
                V[X] = NN;
                break;
            case 7:
                V[X] += NN;
                break;
            case 0x0A:
                I = NNN;
                break;
            case 0x0D:
                printf("%d %d %d\n", X, Y, command);
                uint8_t XX = X%WIDTH;
                uint8_t YY = Y%HEIGHT;
                for (uint8_t i = 0; i < N; i++)
                {
                    const uint8_t sprite_data = memory[I + i];
                    XX=X%WIDTH;
                    for (uint8_t j = 0; j < 8; j++)
                    {
                        screen[YY*WIDTH+XX]^=(sprite_data>>(8-j)&1);
                        XX+=1;
                        if (XX==WIDTH){
                            break;
                        }
                    }
                    YY+=1;
                    if (YY==HEIGHT){
                        break;
                    }

                }

                // for (uint8_t i = 0; i < HEIGHT; i++) {
                //     for (uint8_t j = 0; j < WIDTH; j++) {
                //         printf("%d", screen[i * WIDTH + j]);
                //     }
                //     printf("\n");
                // }
                // printf("\n");

                break;

            default:
                printf("Default\n");
                break;
        }
        
        // for (uint8_t i = 0; i < HEIGHT; i++) {
        //     for (uint8_t j = 0; j < WIDTH; j++) {
        //         printf("%d", screen[i * WIDTH + j]);
        //     }
        //     printf("\n");
        // }

    }
    uint8_t NNN;

    return 0;
}