#include <stdio.h>
#include <string.h>
#include <map> // pair
#include <functional>
#include <unordered_map>

#define NUMBER_KNOTS 10

namespace std 
{
    template <>
    struct hash <std::pair<int, int>> {
        std::size_t operator () (const std::pair<int, int> &p) const {
            size_t seed = 0;
            seed ^= std::hash<int>{}(p.first) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            seed ^= std::hash<int>{}(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
            return seed;
        }
    };
}

int main()
{
    int knots_x[NUMBER_KNOTS], knots_y[NUMBER_KNOTS];
    memset(knots_x, 0, NUMBER_KNOTS * sizeof(int));
    memset(knots_y, 0, NUMBER_KNOTS * sizeof(int));
    FILE *f = fopen("day9.txt", "r");
    char line_buffer[10];
    char dxn;
    char intensity;
    std::unordered_map<std::pair<int, int>, bool> locations;

    locations[std::make_pair(knots_x[NUMBER_KNOTS - 1], knots_y[NUMBER_KNOTS - 1])] = true;
    while(fgets(line_buffer, 10, f) != NULL && feof(f) == 0)
    {
        dxn = line_buffer[0];
        line_buffer[strlen(line_buffer) - 1] = 0;
        intensity = atoi(&line_buffer[2]);

        for(int i = 0; i < intensity; i++)
        {
            // Move head
            if(dxn == 'R')
            {
                knots_y[0]++;
            }
            else if(dxn == 'L')
            {
                knots_y[0]--;
            }
            else if(dxn == 'U')
            {
                knots_x[0]--;
            }
            else if(dxn == 'D')
            {
                knots_x[0]++;
            }

            /*
                cases:
                    unaligned         aligned
                    t . .    . h .    h . .    . . .
                    . . h    . . .    . . .    . . .
                    t . .    t . t    t . .    h . t

                    . . t    t . t    t . .    . . .
                    h . .    . . .    . . .    . . .
                    . . t    . h .    h . .    t . h
            */
            for(int ith_knot = 1; ith_knot < NUMBER_KNOTS; ith_knot++)
            {
                int diff_x = knots_x[ith_knot - 1] - knots_x[ith_knot]; 
                int diff_y = knots_y[ith_knot - 1] - knots_y[ith_knot];
                int abs_diff_x = abs(diff_x); 
                int abs_diff_y = abs(diff_y);

                // Adjust tail knot position if head and tail aren't adjacent
                if(abs_diff_x > 1 || abs_diff_y > 1)
                {
                    if(abs_diff_x >= 1)
                    {
                        knots_x[ith_knot] += diff_x / abs_diff_x;
                    }

                    if(abs_diff_y >= 1)
                    {
                        knots_y[ith_knot] += diff_y / abs_diff_y;
                    }
                }
            }
            locations[std::make_pair(knots_x[NUMBER_KNOTS - 1], knots_y[NUMBER_KNOTS - 1])] = true;
        }
    }
    fclose(f);

    int count = 0;
    for(auto itr = locations.begin(); itr != locations.end(); itr++)
    {
        count++;
    }
    printf("%u\n", count);

    return 0;
}