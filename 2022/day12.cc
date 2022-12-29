#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <vector>
#include <queue>
#include <chrono>

// Number elements
#define NELS 100

// Globals, yuck, i know
int width;
int height;
unsigned char map[NELS][NELS];
int visited_map[NELS][NELS];
unsigned char Ex;
unsigned char Ey;
std::vector<std::pair<int, int>> starting_positions;

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
                // Sx = i;
                // Sy = j;
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
        memset(visited_map[i], 0, sizeof(int) * width);
    }

    // Part 2 - Find starting positions
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(map[i][j] == 'a')
            {
                starting_positions.push_back(std::make_pair(i, j));
            }
        }
    }
}

int findMinPathDFS(unsigned char Sx, unsigned char Sy)
{
    std::vector<std::pair<unsigned char, unsigned char> > pos_s;
    std::vector<int> steps_s;
    int steps = 0;
    unsigned char curr_x = Sx;
    unsigned char curr_y = Sy;

    pos_s.push_back(std::make_pair(curr_x, curr_y));
    steps_s.push_back(steps);
    while(!pos_s.empty())
    {
        curr_x = pos_s.back().first;
        curr_y = pos_s.back().second;
        steps = steps_s.back();
        pos_s.pop_back();
        steps_s.pop_back();

        // If we already found a route that is less steps, continue
        if(visited_map[curr_x][curr_y] != 0 && steps >= visited_map[curr_x][curr_y])
        {
            continue;
        }

        // Record min steps
        visited_map[curr_x][curr_y] = steps;

        // If 'E', stop
        if(curr_x == Ex && curr_y == Ey)
        {
            continue;
        }

        // Consider all dxns
        if (curr_y + 1 < width &&
            map[curr_x][curr_y + 1] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x][curr_y + 1] == 0 ||
             visited_map[curr_x][curr_y + 1] > steps + 1) &&
             !(curr_x == Sx && curr_y + 1 == Sy) )  // TODO not needed because next step needs to be +1 character
        {
            pos_s.push_back(std::make_pair(curr_x, curr_y + 1));
            steps_s.push_back(steps + 1);
        }
        if(curr_x + 1 < height &&
            map[curr_x + 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x + 1][curr_y] == 0 ||
             visited_map[curr_x + 1][curr_y] > steps + 1) &&
             !(curr_x + 1 == Sx && curr_y == Sy))
        {
            pos_s.push_back(std::make_pair(curr_x + 1, curr_y));
            steps_s.push_back(steps + 1);
        }
        if(curr_y - 1 >= 0 &&
            map[curr_x][curr_y - 1] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x][curr_y - 1] == 0 ||
             visited_map[curr_x][curr_y - 1] > steps + 1) &&
             !(curr_x - 1 == Sx && curr_y == Sy))
        {
            pos_s.push_back(std::make_pair(curr_x, curr_y - 1));
            steps_s.push_back(steps + 1);
        }
        if(curr_x - 1 >= 0 &&
            map[curr_x - 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x - 1][curr_y] == 0 ||
             visited_map[curr_x - 1][curr_y] > steps + 1) &&
             !(curr_x - 1 == Sx && curr_y == Sy))
        {
            pos_s.push_back(std::make_pair(curr_x - 1, curr_y));
            steps_s.push_back(steps + 1);
        }
    }

    return visited_map[Ex][Ey];
}

int findMinPathBFS(unsigned char Sx, unsigned char Sy)
{
    std::queue<std::pair<unsigned char, unsigned char> > pos_q;
    std::queue<int> steps_q;
    int steps = 0;
    unsigned char curr_x = Sx;
    unsigned char curr_y = Sy;

    pos_q.push(std::make_pair(curr_x, curr_y));
    steps_q.push(steps);
    while(!pos_q.empty())
    {
        curr_x = pos_q.front().first;
        curr_y = pos_q.front().second;
        steps = steps_q.front();
        pos_q.pop();
        steps_q.pop();

        // If we already found a route that is less steps, continue
        if(visited_map[curr_x][curr_y] != 0 && steps >= visited_map[curr_x][curr_y])
        {
            continue;
        }

        // Record min steps
        visited_map[curr_x][curr_y] = steps;

        // If 'E', stop
        if(curr_x == Ex && curr_y == Ey)
        {
            continue;
        }

        // Consider all dxns
        if (curr_y + 1 < width &&
            map[curr_x][curr_y + 1] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x][curr_y + 1] == 0 ||
             visited_map[curr_x][curr_y + 1] > steps + 1) &&
             !(curr_x == Sx && curr_y + 1 == Sy) )  // TODO not needed because next step needs to be +1 character
        {
            pos_q.push(std::make_pair(curr_x, curr_y + 1));
            steps_q.push(steps + 1);
        }
        if(curr_x + 1 < height &&
            map[curr_x + 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x + 1][curr_y] == 0 ||
             visited_map[curr_x + 1][curr_y] > steps + 1) &&
             !(curr_x + 1 == Sx && curr_y == Sy))
        {
            pos_q.push(std::make_pair(curr_x + 1, curr_y));
            steps_q.push(steps + 1);
        }
        if(curr_y - 1 >= 0 &&
            map[curr_x][curr_y - 1] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x][curr_y - 1] == 0 ||
             visited_map[curr_x][curr_y - 1] > steps + 1) &&
             !(curr_x - 1 == Sx && curr_y == Sy))
        {
            pos_q.push(std::make_pair(curr_x, curr_y - 1));
            steps_q.push(steps + 1);
        }
        if(curr_x - 1 >= 0 &&
            map[curr_x - 1][curr_y] - map[curr_x][curr_y] <= 1 &&
            (visited_map[curr_x - 1][curr_y] == 0 ||
             visited_map[curr_x - 1][curr_y] > steps + 1) &&
             !(curr_x - 1 == Sx && curr_y == Sy))
        {
            pos_q.push(std::make_pair(curr_x - 1, curr_y));
            steps_q.push(steps + 1);
        }
    }

    return visited_map[Ex][Ey];
}

int main()
{
    readMap("day12.txt");
    int min_steps_bfs = 0, min_steps_dfs = 0;
    int steps;

    // Compare BFS and DFS time
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::duration elapsed_time;

    // BFS
    start_time = std::chrono::high_resolution_clock::now();
    for(auto &sp : starting_positions)
    {
        min_steps_bfs = findMinPathBFS(sp.first, sp.second);
        if(min_steps_bfs == 0 || steps < min_steps_bfs)
        {
            min_steps_bfs = steps;
        }
    }
    elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
    printf("BFS time = %luus\n", std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count());

    // DFS
    start_time = std::chrono::high_resolution_clock::now();
    for(auto &sp : starting_positions)
    {
        min_steps_dfs = findMinPathDFS(sp.first, sp.second);
        if(min_steps_dfs == 0 || steps < min_steps_dfs)
        {
            min_steps_dfs = steps;
        }
    }
    elapsed_time = std::chrono::high_resolution_clock::now() - start_time;
    printf("DFS time = %luus\n", std::chrono::duration_cast<std::chrono::microseconds>(elapsed_time).count());
    printf("min steps: %u\n", min_steps_bfs);
    printf("dfs result == bfs result: %u\n", min_steps_bfs == min_steps_dfs);
    return 0;
}
