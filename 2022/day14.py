import re
import numpy as np
from typing import List

# TODO preprocess input for the min
y_offset = 490  # arbitrary
DISPLAY_SIZE = 20
sand_entry_point = (0, 500 - y_offset)  # from the problem prompt


def simulate_sand_point(display: List[List]) -> List[List]:
    # None -> fell into abyss
    # display -> sand landed, display updated)
    x, y = (sand_entry_point[0], sand_entry_point[1])
    while display[x + 1, y] == '.':
        x += 1

    x_, y_ = x + 1, y + 1

    # This could be recursive
    # Try left
    while display[x_ + 1, y_ - 1] == '.':
        x_ += 1
        y_ += 1


    if x_ != x:
        display[x_, y_] = 'o'

    # Try right
    while display[x_ + 1, y_ + 1] == '.':
        x_ += 1
        y_ += 1
    if x_ != x:
        display[x_, y_] = 'o'

    # else, stay
    else:
        display[x, y] = 'o'


# Create display
display = np.full((DISPLAY_SIZE, DISPLAY_SIZE), '.', dtype='U1')

# Draw rocks
with open("day14.txt", "r") as infile:
    while line := infile.readline():
        m = re.findall(r"(\d+,\d+)+", line)

        # For each pair of points
        for p1, p2 in zip(m[:-1], m[1:]):
            p1 = p1.split(',')
            p2 = p2.split(',')
            # input file is y,x
            # but i want x,y
            # also, convert to int's
            p1 = [int(p1[1]), int(p1[0]) - y_offset]
            p2 = [int(p2[1]), int(p2[0]) - y_offset]

            # Draw rocks from coordinate to next coordinate
            x_diff = p2[0] - p1[0]
            y_diff = p2[1] - p1[1]
            x_dxn = 0 if not x_diff else int(x_diff / abs(x_diff))
            y_dxn = 0 if not y_diff else int(y_diff / abs(y_diff))
            while p1 != p2:
                display[p1] = '#'
                p1[0] += x_dxn
                p1[1] += y_dxn

display[sand_entry_point[0], sand_entry_point[1]] = '+'
print(display)
