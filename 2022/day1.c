#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    FILE *f = fopen("day1.txt", "r");
    char line_buffer[32];
    unsigned short line_length;
    int current = 0;
    int top1 = 0, top2 = 0, top3 = 0;
    while(fgets(line_buffer, 32, f) != NULL && feof(f) == 0)
    {   
        line_length = strlen(line_buffer);
        
        // Sum 
        if(line_length == 1) // '\n' only
        {
            // Shift tops using swap
            if(current > top3)
            {
                swap(&current, &top3);
                if(top3 > top2) swap(&top2, &top3);
                if(top2 > top1) swap(&top1, &top2);
            }
            current = 0;
        }
        else
        {
            line_buffer[line_length - 1] = 0;
            current += (atoi(line_buffer));
        }
    }
    printf("Top 3 most: %u, %u, %u\n", top1, top2, top3);
    printf("Total %u\n", top3 + top2 + top1);

    return 0;
}