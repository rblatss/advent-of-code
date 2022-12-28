#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define KB 1024
#define NUMBER_LINES (KB * 3)
#define LINELENGTH 20

typedef enum
{
    ROCK = 1,
    PAPER = 2,
    SCISSORS = 3
} RPS;

typedef enum
{
    LOSE = 1,
    DRAW = 2,
    WIN = 3
} LDW;

void readfile(const char *filename, char filebuffer[NUMBER_LINES][LINELENGTH])
{
    FILE *fp;
    int line_index = 0;

    fp = fopen("day2.txt", "r");
    while(feof(fp) == 0)
    {
        fgets(filebuffer[line_index], LINELENGTH, fp);
        line_index++;
    }
    filebuffer[line_index][0] = 0;
}

int day2(const char filebuffer[NUMBER_LINES][LINELENGTH])
{
    LDW outcome;
    RPS opp, you;
    int score = 0;
    for(int i = 0; i < NUMBER_LINES && filebuffer[i][0] != 0; i++)
    {
        outcome = (filebuffer[i][2] - 'X' + 1); // X == 1 (LOSE), Y == 2 (DRAW), Z == 3 (WIN)
        opp = (filebuffer[i][0] - 'A' + 1); // A == 1, B == 2, C == 3

        if(outcome == LOSE)
        {
            if(opp == ROCK)
            {
                you = SCISSORS;
            }
            else
            {
                you = opp - 1;
            }
        }
        else if(outcome == DRAW)
        {
            you = opp;
        }
        else if(outcome == WIN)
        {
            if(opp == SCISSORS)
            {
                you = ROCK;
            }
            else
            {
                you = opp + 1;
            }
        }

        //printf("you should %u: you %u, opp %u\n", outcome, you, opp);

        // DRAW
        if(you == opp)
        {
            score += you + 3;
        }
        // Loss
        else if((you == ROCK && opp == PAPER) || 
                (you == PAPER && opp == SCISSORS) ||
                (you == SCISSORS && opp == ROCK))
        {
            score += you;
        }
        // WIN
        else if((you == PAPER && opp == ROCK) || 
                (you == SCISSORS && opp == PAPER) ||
                (you == ROCK && opp == SCISSORS))
        {
            score += you + 6;
        }
    }
    return score;
}

int main()
{
    char filebuffer[NUMBER_LINES][LINELENGTH];
    readfile("day2.txt", filebuffer);
    printf("%d\n", day2(filebuffer));
    return 0;
}