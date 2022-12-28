#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm> // std::sort
#include <vector> // write custom predicate function

#define N_MONKEYS 4 /* ughhhh */
#define KB (1024)

typedef struct monkey
{
    int n_items;
    unsigned long long int items[100];
    char op;
    char left_op[4], right_op[4];
    unsigned long long int div;
    int throw_true;
    int throw_false;
    unsigned long long int number_items_inspected;
} monkey;

#define IS_DIGIT(c) (c >= '0' && c <= '9')

bool monkey_sorter(monkey const& lhs, monkey const& rhs) {
    return lhs.number_items_inspected > rhs.number_items_inspected;
}

void strrmc(char *buffer, char c)
{
    int i = 0, j = 0;
    while (buffer[i] != 0)
    {
        if (buffer[i] != c)
        {
            buffer[j] = buffer[i];
            j++;
        }
        i++;
    }
    buffer[j] = 0;
}

void straddnullc(char *buffer, const char *chars)
{
    for (int i = 0; buffer[i] != 0; i++)
    {
        for (int j = 0; chars[j] != 0; j++)
        {
            if (buffer[i] == chars[j])
            {
                buffer[i] = 0;
                break;
            }
        }
    }
}

void createMonkeyRecords(const char *filename, std::vector<struct monkey> &monkeys)
{
    FILE *f = fopen(filename, "r");
    char line_buffer[KB];
    int number_bytes;
    int mindex;
    int line_buffer_len;
    while (fgets(line_buffer, KB, f) != NULL && feof(f) == 0)
    {
        // Skip empty line
        if (line_buffer[0] == '\n')
        {
            continue;
        }

        if (line_buffer[0] == 'M') // Monkey
        {
            mindex = line_buffer[7] - '0';
            monkey m;
            monkeys.push_back(m);
            monkeys[mindex].number_items_inspected = 0;
        }
        else if (line_buffer[2] == 'S') // Starting items
        {
            monkeys[mindex].n_items = 0;
            line_buffer_len = strlen(line_buffer) - 1;
            straddnullc(line_buffer, ",\n"); // terminators for atoi's
            for (int i = 0; i < line_buffer_len;)
            {
                if (IS_DIGIT(line_buffer[i]))
                {
                    monkeys[mindex].items[monkeys[mindex].n_items] = atoi(&line_buffer[i]);
                    monkeys[mindex].n_items++;
                    i += strlen(&line_buffer[i]) + 1; // skip rest of digits and 0
                }
                else
                {
                    i++;
                }
            }
        }
        else if (line_buffer[2] == 'O') // Operation
        {
            int i;
            for (i = 0; line_buffer[i] != '*' &&
                        line_buffer[i] != '+' &&
                        line_buffer[i] != '/' &&
                        line_buffer[i] != '-';
                 i++)
            {
            }
            monkeys[mindex].op = line_buffer[i];
            line_buffer[i - 1] = 0;
            strcpy(monkeys[mindex].right_op, &line_buffer[i + 2]);
            for(i; line_buffer[i] != '='; i--)
            { }
            strcpy(monkeys[mindex].left_op, &line_buffer[i + 2]);
        }
        else if (line_buffer[2] == 'T')  // Test
        {
            line_buffer[strlen(line_buffer) - 1] = 0;
            monkeys[mindex].div = (unsigned long long int) atoi(&line_buffer[21]);
            fgets(line_buffer, KB, f);
            monkeys[mindex].throw_true = line_buffer[strlen(line_buffer) - 2] - '0';
            fgets(line_buffer, KB, f);
            monkeys[mindex].throw_false = line_buffer[strlen(line_buffer) - 2] - '0';
        }
    }
    fclose(f);
}

void do_round(std::vector<monkey> &monkeys)
{
    unsigned long long int worry_level;
    unsigned long long int left, right;
    int tgt;
    int number_starting_items;
    for(int src = 0; src < N_MONKEYS; src++)
    {
        monkey &m = monkeys[src];
        number_starting_items = m.n_items;
        for(int i = 0; i < number_starting_items; i++)
        {
            // Monkey inspects item
            worry_level = m.items[0];
            m.number_items_inspected++;

            // Operate
            if(strstr(m.left_op, "old"))
            {
                left = worry_level;
            }
            else
            {
                left = atoi(m.left_op);
            }

            if(strstr(m.right_op, "old"))
            {
                right = worry_level;
            }
            else
            {
                right = atoi(m.right_op);
            }

            switch (m.op)
            {
                case '+':
                    worry_level = left + right;
                    break;
                case '*':
                    worry_level = left * right;
                    break;
                // case '-':
                //     worry_level = left - right;
                //     break;
                // case '/':
                //     worry_level = left / right;
                //     break;
                default:
                    break;
            }

            // Monkey gets bored
            // Part 2: no longer divide by 3
            // worry_level /= 3;
            // printf("%llu ", worry_level);

            // Where to throw?
            if(worry_level % m.div == 0)
            {
                tgt = m.throw_true;
            }
            else
            {
                tgt = m.throw_false;
            }
        
            // Throw
            monkeys[tgt].items[monkeys[tgt].n_items] = worry_level;
            monkeys[tgt].n_items++;

            // Shift current monkey's items to left
            for(int j = 0; j < m.n_items - 1; j++)
            {
                m.items[j] = m.items[j + 1];
            }
            m.n_items--;
        }
        printf("\n");
    }
}


int main(int argc, char **argv)
{
    if(argc < 2 || argc > 2)
    {
        printf("How many rounds do you want to perform??\n");
        return -1;
    }
    int number_rounds = atoi(argv[1]);
    std::vector<struct monkey> monkeys;
    createMonkeyRecords("day11.txt", monkeys);
    // for (int i = 0; i < N_MONKEYS; i++)
    // {
    //     printf("%u, %s %c %s", i, monkeys[i].left_op, monkeys[i].op, monkeys[i].right_op);
    //     printf("%u\n", monkeys[i].n_items);
    //     printf("%u, %u, %u\n", monkeys[i].div, monkeys[i].throw_false, monkeys[i].throw_true);
    //     for (int j = 0; j < monkeys[i].n_items; j++)
    //     {
    //         printf("%u ", monkeys[i].items[j]);
    //     }
    //     printf("\n\n");
    // }

    for(int i = 0; i < number_rounds; i++)
    {
        do_round(monkeys);
    }

    for(int i = 0; i < N_MONKEYS; i++)
    {
        printf("Monkey %u # inspections: %llu", i, monkeys[i].number_items_inspected);
        // for (int j = 0; j < monkeys[i]->n_items; j++)
        // {
        //     printf("%u ", monkeys[i]->items[j]);
        // }
        printf("\n");
    }
    printf("\n");

    std::sort(monkeys.begin(), monkeys.end(), &monkey_sorter);
    printf("Top 2: %llu and %llu\n", monkeys[0].number_items_inspected, monkeys[1].number_items_inspected);
    printf("Total monkey business %llu\n", monkeys[0].number_items_inspected * monkeys[1].number_items_inspected);
    return 0;
}