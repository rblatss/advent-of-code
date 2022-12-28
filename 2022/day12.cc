#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <queue>

// Number elements
#define NELS 100

typedef struct progress_point
{
    unsigned char x;
    unsigned char y;
    int steps;
    // bool right, down, left, up;
    progress_point(unsigned char _x, unsigned char _y, int _steps)
    {
        x = _x;
        y = _y;
        steps = _steps;
        // right = false;
        // down = false;
        // left = false;
        // up = false;
    }
} progress_point;

// Globals, yuck, i know
int width;
int height;
unsigned char map[NELS][NELS];
bool visited_map[NELS][NELS];
unsigned char Ex;
unsigned char Ey;
unsigned char Sx;
unsigned char Sy;

void printMap()
{
    for(int i = 0; i < height; i++)
    {
        printf("%s\n", map[i]);
    }
}

void readMap(const char *filename)
{
    FILE *f = fopen(filename, "r");

    // Read a line to get map width and remove \n's
    if (fgets((char *)&map[0], NELS, f) == NULL || feof(f) != 0)
    {
        return;
    }
    width = strlen((const char *) map[0]) - 1;
    map[0][width] = 0;
    height++;

    // Read the rest
    int i = 1;
    while (fgets((char *)&map[i], NELS, f) != NULL && feof(f) == 0)
    {
        map[i][width] = 0;
        height++;
        i++;
    }

    fclose(f);

    // Print what was read
    printMap();

    // Find 'S', cache coordinates, replace with 'a'
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'S')
            {
                map[i][j] = 'a';
                Sx = i;
                Sy = j;
                break;
            }
        }
    }

    // Find 'E', cache coordinates, replace with 'z'
    for(int i = 0;i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'E')
            {
                map[i][j] = 'z';
                Ex = i;
                Ey = j;
                break;
            }
        }
    }

    // Initialize visited_map
    for(int i = 0; i < height; i++)
    {
        memset(visited_map[i], 0, sizeof(bool) * width);
    }
}

// int findMinPathDFS()
// {
//     int min_steps = -1;

//     std::vector<progress_point *> stack;
//     int curr_x = Sx;
//     int curr_y = Sy;
//     int steps = 0;

//     // We've visited the starting point
//     visited_map[curr_x][curr_y].visited = true;

//     // First step
//     progress_point *next_p = new progress_point(curr_x, curr_y, steps);
//     stack.push_back(next_p);

//     while(true)
//     {
//         // Where are we? How many steps?
//         progress_point *curr_p = stack.back();
//         curr_y = curr_p->y;
//         curr_x = curr_p->x;
//         steps = curr_p->steps;

//         printf("x/y: %u/%u\n", curr_x, curr_y);
//         break;
//         // Check if we're at 'E'
//         if(map[curr_x][curr_y] == 'z')
//         {
//             if(curr_x == Ex && curr_y == Ey)
//             {
//                 if(min_steps == -1 || steps < min_steps)
//                 {
//                     min_steps = steps;
//                 }
//                 visited_map[curr_x][curr_y].visited = false;
//                 progress_point *curr_p = stack.back();
//                 stack.pop_back();
//                 delete curr_p;
//                 continue;
//             }
//         }

//         // Check adjacent squares starting at right, going clockwise
//         if(curr_p->right == false)
//         {
//             curr_p->right = true;

//             if (curr_y + 1 < width &&
//                 (map[curr_x][curr_y + 1] - map[curr_x][curr_y] <= 1) &&
//                 visited_map[curr_x][curr_y + 1].visited == false)
//             {
//                 // Take step
//                 visited_map[curr_x][curr_y].visited = true;
//                 progress_point *next_p = new progress_point(curr_x, curr_y + 1, steps + 1);
//                 stack.push_back(next_p);
//             }
//         }
//         else if(curr_p->down == false)
//         {
//             curr_p->down = true;

//             if (curr_x + 1 < height &&
//                 (map[curr_x + 1][curr_y] - map[curr_x][curr_y] <= 1) &&
//                 visited_map[curr_x + 1][curr_y].visited == false)
//             {
//                 // Take step
//                 visited_map[curr_x][curr_y].visited = true;
//                 progress_point *next_p = new progress_point(curr_x + 1, curr_y, steps + 1);
//                 stack.push_back(next_p);
//             }
//         }
//         else if(curr_p->left == false)
//         {
//             curr_p->left = true;

//             if (curr_y - 1 >= 0 &&
//                 (map[curr_x][curr_y - 1] - map[curr_x][curr_y] <= 1) &&
//                 visited_map[curr_x][curr_y - 1].visited == false)
//             {
//                 // Take step
//                 visited_map[curr_x][curr_y].visited = true;
//                 progress_point *next_p = new progress_point(curr_x, curr_y - 1, steps + 1);
//                 stack.push_back(next_p);
//             }
//         }
//         else if(curr_p->up == false)
//         {
//             curr_p->up = true;

//             if (curr_x - 1 >= 0 && 
//                 map[curr_x - 1][curr_y] - map[curr_x][curr_y] <= 1 &&
//                 visited_map[curr_x - 1][curr_y].visited == false)
//             {
//                 // Take step
//                 visited_map[curr_x][curr_y].visited = true;
//                 progress_point *next_p = new progress_point(curr_x - 1, curr_y, steps + 1);
//                 stack.push_back(next_p);
//             }
//         }
//         else // backtrack, we've tried every dxn
//         {
//             visited_map[curr_x][curr_y].visited = false;
//             curr_p = stack.back();
//             stack.pop_back();
//             delete curr_p;
            
//             // Check if we're done
//             if(curr_x == 0 && curr_y == 0)
//             {
//                 break;
//             }
//         }
//     }

//     return min_steps;
// }

int findMinPathBFS()
{
    unsigned char curr_x;
    unsigned char curr_y;
    int steps;
    std::queue<std::pair<unsigned char, unsigned char> > pos_q;
    std::queue<int> steps_q;
    pos_q.push(std::make_pair(curr_x, curr_y));
    steps_q.push(steps);
    int min_steps = -1;

    while(!pos_q.empty())
    {
        curr_x = pos_q.front().first;
        curr_y = pos_q.front().second;
        steps = steps_q.front();
        pos_q.pop();
        steps_q.pop();

        // Why does this work?
        visited_map[curr_x][curr_y] = true;

        // Check if end is reached
        if(map[curr_x][curr_y] == 'z' &&
           curr_x == Ex && curr_y == Ey)
        {
            if(min_steps == -1 || steps < min_steps)
            {
                min_steps = steps;
            }
            continue;
        }

        // Consider all dxns
        if (curr_y + 1 < width &&
            map[curr_x][curr_y + 1] - map[curr_x][curr_y] <= 1 &&
            visited_map[curr_x][curr_y + 1] == false)
        {
            pos_q.push(std::make_pair(curr_x, curr_y + 1));
            steps_q.push(steps + 1);
        }
        if(curr_x + 1 < height &&
            map[curr_x + 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            visited_map[curr_x + 1][curr_y] == false)
        {
            pos_q.push(std::make_pair(curr_x + 1, curr_y));
            steps_q.push(steps + 1);
        }
        if(curr_y - 1 >= 0 &&
            map[curr_x][curr_y - 1] - map[curr_x][curr_y] <= 1 &&
            visited_map[curr_x][curr_y - 1] == false)
        {
            pos_q.push(std::make_pair(curr_x, curr_y - 1));
            steps_q.push(steps + 1);
        }
        if(curr_x - 1 >= 0 &&
            map[curr_x - 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            visited_map[curr_x - 1][curr_y] == false)
        {
            pos_q.push(std::make_pair(curr_x - 1, curr_y));
            steps_q.push(steps + 1);
        }
    }

    return min_steps;
}

int main()
{
    readMap("day12.txt");
    printf("%d\n", findMinPathBFS());
    return 0;
}
