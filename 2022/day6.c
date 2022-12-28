#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define MB (1024 * 1024)
#define SEQ_LENGTH 14

typedef enum
{
    false = 0,
    true = 1
} bool;

int main()
{
    // Some variables
    int i;
    bool found;
    char line_buffer[MB];
    uint8_t ascii_char_count[256];
    memset(ascii_char_count, 0, 256 * sizeof(uint8_t));
    FILE *f = fopen("day6.txt", "r");
    int marker_index = -1;

    // Read file, line by line
    while(feof(f) == 0)
    {
        fgets(line_buffer, MB, f);

        // Count first sequence
        for(int offset = 0; offset < SEQ_LENGTH - 1; offset++)
        {
            ascii_char_count[line_buffer[offset]]++;
        }

        // Find marker in line
        for(i = SEQ_LENGTH - 1; line_buffer[i] != '\n'; i++)
        {
            ascii_char_count[line_buffer[i]]++;

            // Check current sequence
            found = true;
            for(int offset = 0; offset < SEQ_LENGTH; offset++)
            {
                if(ascii_char_count[line_buffer[i - offset]] != 1)
                {
                    found = false;
                    break; // short circuit
                }
            }

            if(found)
            {
                marker_index = i + 1;
                break;
            }

            ascii_char_count[line_buffer[i - (SEQ_LENGTH - 1)]]--;  // oh man - removing the parantheses gives a totally different answer
        }
        printf("%d\n", marker_index);

        // Reset counter and marker_index
        for(int offset = 0; offset < SEQ_LENGTH; offset++)
        {
            ascii_char_count[line_buffer[i - offset]] = 0;
        }
        marker_index = -1;
    }
    fclose(f);



    return 0;
}