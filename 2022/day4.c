#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    FILE *f = fopen("day4.txt", "r");
    char line_buffer[256];
    int number_indices[4] = {0};
    int line_size;
    int number_index;
    int left_1, right_1, left_2, right_2;
    unsigned int npairs = 0;
    while(fgets(line_buffer, 256, f) != NULL && feof(f) == 0)
    {
        line_size = strlen(line_buffer);
        line_buffer[line_size - 1] = 0; // replace \n with \0

        // Null terminators everywhere
        number_index = 1;
        for(int i = 0; i < line_size; i++)
        {
            if(line_buffer[i] == '-' || line_buffer[i] == ',')
            {
                line_buffer[i] = 0;
                number_indices[number_index] = i + 1;
                number_index++;
            }
        }

        // Get ranges
        //printf("%u-%u,%u-%u\n", number_indices[0], number_indices[1], number_indices[2], number_indices[3]);
        left_1 = atoi(&line_buffer[number_indices[0]]);
        right_1 = atoi(&line_buffer[number_indices[1]]);
        left_2 = atoi(&line_buffer[number_indices[2]]);
        right_2 = atoi(&line_buffer[number_indices[3]]);
        //printf("%u-%u,%u-%u\n", left_1, right_1, left_2, right_2);

        // Is 1 range within the other?
        if((left_1 >= left_2 && left_1 <= right_2) ||
           (right_1 >= left_2 && right_1 <= right_2) ||
           (left_2 >= left_1 && left_2 <= right_1) ||
           (right_2 >= left_1 && right_2 <= right_1))
        {
            npairs++;
        }
    }
    printf("npairs: %u\n", npairs);
    fclose(f);
    return 0;
}