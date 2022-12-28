#include <stdio.h>
#include <string.h>

#include "constants.h"

#define LINES_PER_GROUP 3

char findBadge(const unsigned char ht[LINES_PER_GROUP][256])
{
    for(int i = 0; i < 256; i++)
    {
        if(ht[0][i] && ht[1][i] && ht[2][i])
        {
            return i;
        }
    }
    return -1;
} 

int main()
{
    FILE *f = fopen("day3.txt", "r");
    unsigned char item;
    unsigned char line_buffer[LINES_PER_GROUP][256];
    unsigned char rucksack_size[LINES_PER_GROUP];
    unsigned char ht[LINES_PER_GROUP][256];
    unsigned int priority_sum = 0;
    for(int i = 0; i < 3; i++)
    {
        memset(ht[i], 0, 256 * sizeof(unsigned char));
    }
    char badge = 0;
    int endoffile = 0;
    while(true)
    {
        for(int  i = 0; i < LINES_PER_GROUP && (endoffile = feof(f)) == 0; i++)
        {
            fgets(line_buffer[i], 256, f);
            rucksack_size[i] = strlen(line_buffer[0]) - 1; // subtract 1 for newline
        }

        // Count character occurrences
        for(int i = 0; i < LINES_PER_GROUP; i++)
        {
            for(int j = 0; j < rucksack_size[i]; j++)
            {
                ht[i][line_buffer[i][j]]++;
            }
        }

        // Find badge
        badge = findBadge(ht);
        if(badge == -1)
        {
            printf("%c\n", badge);
        }

        // Reached end of group
        if(badge >= 'a' && badge <= 'z')
        {
            priority_sum += (unsigned int) (badge - 'a') + 1;
        }

        if(badge >= 'A' && badge <= 'Z')
        {
            priority_sum += (unsigned int) (badge - 'A') + 27;
        }

        // Zero count
        for(int i = 0; i < LINES_PER_GROUP; i++)
        {
            memset(ht[i], 0, 256 * sizeof(unsigned char));
        }
    }
    printf("Common character priority sum: %u\n", priority_sum);
    fclose(f);
    return 0;
}