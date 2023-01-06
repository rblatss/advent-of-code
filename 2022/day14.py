import re
import numpy as np

# TODO preprocess input for the min
y_offset = 0  # arbitrary
DISPLAY_SIZE = 1000
sand_entry_point = (0, 500 - y_offset)  # from the problem prompt
floor = -1

def find_grain_of_sand_loc(display: np.ndarray) -> tuple:
    # None -> fell into abyss
    # display -> sand landed, display updated)
    x, y = (sand_entry_point[0], sand_entry_point[1])

    while x != DISPLAY_SIZE - 1:
        # Try down
        if display[x + 1, y] == '.':
            x += 1
        # Try left
        elif display[x + 1, y - 1] == '.':
            x += 1
            y -= 1
        # Try right
        elif display[x + 1, y + 1] == '.':
            x += 1
            y += 1
        # Stopped
        else:
            break

    if x != DISPLAY_SIZE - 1:
        return (x, y)
    else:
        return None


# Print display in readable format
def print_display(display: np.ndarray) -> None:
    for row in display:
        print(''.join(row))
    print()


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
                display[p1[0], p1[1]] = '#'
                p1[0] += x_dxn
                p1[1] += y_dxn
            display[p1[0], p1[1]] = '#'

            # Part 2 - find floor
            if p1[0] > floor:
                floor = p1[0]
            if p2[0] > floor:
                floor = p2[0]

# Part 2 - create floor
display[floor + 2] = '#'

sand_count = 0
# print_display(display)
while True:
    pos = find_grain_of_sand_loc(display)
    display[pos] = 'o'
    sand_count += 1
    if pos == sand_entry_point:
        break
# print_display(display)
print(sand_count)
