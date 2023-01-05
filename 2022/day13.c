#include <stdio.h>
#include <stdlib.h>

#define BLEN 64
// Arbitrarily choosing @. But it looks like the numbers in the problem input are 1-10 only.
#define ISNUMBER(x) (x >= '0' && x <= '@')

typedef enum
{
    false = 0,
    true = 1
} bool;

int myfgets(char *line_buffer, const int count, FILE *f)
{
    int i = 0;
    while(feof(f) == 0 && (line_buffer[i] = fgetc(f)) != '\n' && line_buffer[i] != 0)
    {
        i++;
    }
    line_buffer[i] = 0;
    return i;
}

void swap(char *a, char *b)
{
    char t = *a;
    *a = *b;
    *b = t;
}

void insert_char(char *buf, int index, char c)
{
    // Assumes buf has space
    while(c != 0)
    {
        swap(&c, &buf[index]);
        index++;
    }
    buf[index] = c;
}

void reduce_char_digit(char *buf, int index)
{
    // Convert "10" to ':'
    // Find next non digit char
    int next_i = index;
    while(buf[next_i] != ',' && buf[next_i] != ']')
    {
        next_i++;
    }

    // Convert number to integer
    char t = buf[next_i];
    buf[next_i] = 0;
    int number = atoi(&buf[index]);
    buf[next_i] = t;
    buf[index] = number + '0';

    // Shift characters to left
    index++;
    for(; buf[next_i]; index++, next_i++)
    {
        buf[index] = buf[next_i];
    }
    buf[index] = 0;
}

void preprocess_lists(char *buf_left, char *buf_right)
{
    // Convert 10s to :'s
    for(int i = 0; buf_left[i] != 0; i++)
    {
        if(ISNUMBER(buf_left[i]) && ISNUMBER(buf_left[i + 1]))
        {
            reduce_char_digit(buf_left, i);
        }
    }

    for(int j = 0; buf_right[j] != 0; j++)
    {
        if(ISNUMBER(buf_right[j]) && ISNUMBER(buf_right[j + 1]))
        {
            reduce_char_digit(buf_right, j);
        }
    }

    // // Convert ints to lists when needed
    // int ldepth = 0, rdepth = 0;
    // for(int i = 0, j = 0; buf_left[i] != 0 || buf_right[j] != 0;)
    // {
    //     if(buf_left[i] == '[' && ISNUMBER(buf_right[j]))
    //     {
    //         insert_char(buf_right, j, '[');
    //         insert_char(buf_right, j + 2, ']');
    //     }
    //     else if(ISNUMBER(buf_left[i]) && buf_right[j] == '[')
    //     {
    //         insert_char(buf_left, i, '[');
    //         insert_char(buf_left, i + 2, ']');
    //     }

    //     if(buf_left[i] == '[') ldepth++;
    //     else if(buf_left[i] == ']') ldepth--;
    //     if(buf_right[j] == '[') rdepth++;
    //     else if(buf_right[j] == ']') rdepth--;

    //     if(ldepth < rdepth)
    //     {
    //         i++; // skip ']'

    //         // Skip remainder of sublist
    //         for(; buf_right[j] && rdepth > ldepth; j++)
    //         {
    //             if(buf_right[j] == '[')
    //             {
    //                 rdepth++;
    //             }
    //             else if (buf_right[j] == ']')
    //             {
    //                 rdepth--;
    //             }
    //         }
    //     }
    //     else if(rdepth < ldepth)
    //     {
    //         j++; // skip ']'

    //         // Skip remainder of sublist
    //         for(; buf_left[i] && ldepth > rdepth; i++)
    //         {
    //             if(buf_left[i] == '[')
    //             {
    //                 ldepth++;
    //             }
    //             else if (buf_left[i] == ']')
    //             {
    //                 ldepth--;
    //             }
    //         }
    //     }

    //     if(buf_left[i]) i++;
    //     if(buf_right[j]) j++;
    // }
}

bool compare_list(const char *left, const char *right)
{
    int lindex = 0;
    int rindex = 0;
    int ldepth = 0;
    int rdepth = 0;
    while(left[lindex] && right[rindex])
    {
        // Skip commas
        if(left[lindex] == ',' || left[lindex] == ']')
        {
            lindex++;
        }
        if(right[rindex] == ',' || right[rindex] == ']')
        {
            rindex++;
        }

        // Compare list to single digit
        if(left[lindex] == '[' && right[rindex] != '[')
        {
            lindex++;
            ldepth++;

            // Compare
            if(left[lindex] > right[rindex])
            {
                return false;
            }

            // Skip remainder of sublist
            while(left[lindex] && ldepth > rdepth)
            {
                lindex++;
                if(left[lindex] == '[')
                {
                    ldepth++;
                }
                else if (left[lindex] == ']')
                {
                    ldepth--;
                }
            }
            lindex++; // skip ']'
        }
        else if(right[rindex] == '[' && left[lindex] != '[')
        {
            rindex++;
            rdepth++;

            // Compare
            if(left[lindex] > right[rindex])
            {
                return false;
            }

            // Skip remainder of sublist
            while(right[rindex] && rdepth > ldepth)
            {
                rindex++;
                if(right[rindex] == '[')
                {
                    rdepth++;
                }
                else if (right[rindex] == ']')
                {
                    rdepth--;
                }
            }
            rindex++; // skip ']'
        }

        // Compare
        else if(left[lindex] > right[rindex])
        {
            return false;
        }

        lindex++;
        rindex++;
    }
    return true;
}

int main()
{
    FILE *f = fopen("day13.txt", "r");
    char buf_left[BLEN];
    char buf_right[BLEN];
    int linesize;
    int count = 0;

    while(feof(f) == 0)
    {
        myfgets(buf_left, BLEN, f);
        myfgets(buf_right, BLEN, f);

        // Preprocess
        preprocess_lists(buf_left, buf_right);
        printf("%s\n", buf_left);
        printf("%s\n", buf_right);

        // Compare
        if(compare_list(buf_left, buf_right))
        {
            printf("1\n");
            count++;
        }
        else
        {
            printf("0\n");
        }
        printf("\n");

        // Skip empty line
        fgets(buf_left, BLEN, f);
    }
    printf("%d\n", count);
    return 0;
}