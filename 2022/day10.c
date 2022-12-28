#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CRT_WIDTH 40
#define CRT_HEIGHT 6

void draw(char crt_screen[CRT_HEIGHT][CRT_WIDTH + 1], int X, int cycle)
{
    int crt_x = cycle / CRT_WIDTH;
    int crt_y = cycle % CRT_WIDTH;
    if( X - 1 >= 0 &&
        X < CRT_WIDTH &&
        (crt_y == X - 1 || crt_y == X || crt_y == X + 1))
    {
        crt_screen[crt_x][crt_y] = '#';
    }
    else
    {
        crt_screen[crt_x][crt_y] = '.';
    }
}


int main()
{
    FILE *f = fopen("day10.txt", "r");
    char line_buffer[10];
    char *ptr;
    int cycle = 0;
    int X = 1;
    int signal_strength;
    int signal_strength_sum = 0;

    // Define / initialize CRT screen
    char crt_screen[CRT_HEIGHT][CRT_WIDTH + 1];
    for(int i = 0; i < CRT_HEIGHT; i++)
    {
        crt_screen[i][CRT_WIDTH] = 0;
    }

    while(fgets(line_buffer, 10, f) != NULL && feof(f) == 0)
    {
        // Process instruction
        if(strstr(line_buffer, "noop"))
        {
            draw(crt_screen, X, cycle);
            cycle++;
            // if(cycle == 20 || ((cycle - 20) % 40) == 0)
            // {
            //     signal_strength = cycle * X;
            //     //printf("cycle %d, X %d, strength %d\n", cycle, X, signal_strength);
            //     signal_strength_sum += signal_strength;
            // }
        }
        else
        {
            draw(crt_screen, X, cycle);
            cycle++;

            draw(crt_screen, X, cycle);
            cycle++;

            ptr = strchr(line_buffer, '\n');
            *ptr = 0;
            X += atoi(&line_buffer[5]);
        }
    }
    fclose(f);

    // Draw screen
    for(int i = 0; i < CRT_HEIGHT; i++)
    {
        printf("%s\n", crt_screen[i]);
    }

    return 0;
}