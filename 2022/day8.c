#include <stdio.h>
#include <string.h>

#include "constants.h"

int calculateScenicScore(char tall_trees[KB][KB], int row, int col, int rows, int cols)
{
    int score;
    int scenic_score = 1;
    int i, j;

    // Up
    score = 0;
    for(i = row - 1, j = col; i >= 0 && tall_trees[row][col] > tall_trees[i][j]; i--)
    {
        score++;
    }
    if(i >= 0) score++;
    scenic_score *= score;

    // Down
    score = 0;
    for(i = row + 1, j = col; i < rows && tall_trees[row][col] > tall_trees[i][j]; i++)
    {
        score++;
    }
    if(i < rows) score++;
    scenic_score *= score;

    // Right
    score = 0;
    for(i = row, j = col - 1; j >= 0 && tall_trees[row][col] > tall_trees[i][j]; j--)
    {
        score++;
    }
    if(j >= 0) score++;
    scenic_score *= score;
    
    // Left
    score = 0;
    for(i = row, j = col + 1; j < cols && tall_trees[row][col] > tall_trees[i][j]; j++)
    {
        score++;
    }
    if(j < cols) score++;
    scenic_score *= score;
    
    return scenic_score;
}


int main()
{
    FILE *f = fopen("day8.txt", "r");
    char tall_trees[KB][KB];

    // Read map into memory, calculate size
    int rows, cols;
    while(fgets(tall_trees[rows], KB, f) != NULL && feof(f) == 0)
    {
        rows++;
    }
    fclose(f);
    cols = strlen(tall_trees[0]) - 1; // ignore '\n'!

    // Remove '\n's
    for(int i = 0; i < rows; i++)
    {
        tall_trees[i][cols] = 0;
    }

    // Find tree with max scenic score
    int max_scenic_score = 0;
    int current_scenic_score;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            current_scenic_score = calculateScenicScore(tall_trees, i, j, rows, cols);
            if(current_scenic_score > max_scenic_score)
            {
                max_scenic_score = current_scenic_score;
            }
        }
    }
    printf("%u\n", max_scenic_score);
    return 0;
}
