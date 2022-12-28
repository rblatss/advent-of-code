#include <stdio.h>
#include <vector>
#include <regex>

#define BUFFER_LENGTH 64
#define ISNUM(x) (x >= '0' && x <= '9')

int main()
{
    // Parse stacks file
    FILE *f_stacks = fopen("day5_stacks.txt", "r");
    char stacks_buffer[BUFFER_LENGTH][BUFFER_LENGTH];
    char line_buffer[BUFFER_LENGTH];
    int number_stacks;
    std::vector<std::vector<char>> stacks;
    std::vector<char> intermediate_stack;  // part 2

    // Determine number of stacks
    fgets(&line_buffer[0], BUFFER_LENGTH, f_stacks);
    for(int i = 0; i < strlen(line_buffer); i++)
    {
        if(ISNUM(line_buffer[i]))
        {
            number_stacks = line_buffer[i] - '0';
        }
    }
    for(int i = 0; i < number_stacks; i++)
    {
        std::vector<char> s;
        stacks.push_back(s);
    }

    // Read stacks into buffer
    int i;
    for(i = 0; fgets(&stacks_buffer[i][0], BUFFER_LENGTH, f_stacks) != NULL && feof(f_stacks) == 0; i++)
    { }
    fclose(f_stacks);

    // Parse buffer from bottom up and initialize stacks
    int rows = i;
    int cols = strlen(stacks_buffer[0]);
    for(int r = rows - 1; r >= 0; r--)
    {
        for(int c = 1, stack_i = 0; c < cols; c += 4, stack_i++)
        {
            if(stacks_buffer[r][c] != ' ')
            {
                stacks[stack_i].push_back(stacks_buffer[r][c]);
            }
        }
    }

    // Perform moves
    FILE *f_moves = fopen("day5_moves.txt", "r");
    int src, tgt, moves;

    std::regex regex("move ([0-9]+) from ([0-9]) to ([0-9])");
    std::smatch match;
    std::stringstream ss;

    while(fgets(&line_buffer[0], BUFFER_LENGTH, f_moves) != NULL && feof(f_moves) == 0)
    {
        // Search line
        std::string line(line_buffer);
        std::regex_search(line, match, regex);

        // Parse match
        // ughh
        moves = atoi(match[1].str().c_str());
        src = atoi(match[2].str().c_str());
        tgt = atoi(match[3].str().c_str());

        // Do move
        for(int i = 0; i < moves; i++)
        {
            intermediate_stack.push_back(stacks[src - 1].back());
            stacks[src - 1].pop_back();
        }
        
        // Part 2 - complete move
        for(int i = 0; i < moves; i++)
        {
            stacks[tgt - 1].push_back(intermediate_stack.back());
            intermediate_stack.pop_back();
        }
    }
    fclose(f_moves);

    for(int i = 0; i < number_stacks; i++)
    {
        printf("%c", stacks[i].back());
    }
    printf("\n");
    return 0;
}