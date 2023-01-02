import re
from typing import List, Iterable

# TODO determine these offsets from the input
# lowest x <-> highest x
# lowest y <-> highest y
# largest man distance (hmm well, it'd really be the man dist that reaches beyond the most exterior b and s)
MAP_SIZE = 400000000
HALF_SIZE = int(MAP_SIZE / 2)


def calc_man_dist(p1: tuple, p2: tuple) -> int:
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])


# def fill(p1: tuple, p2: tuple, map: List[List]) -> List[List]:
#     # Fill map bw points. Skip over Bs and Ss.
#     src = [p1[0], p1[1]]
#     tgt = [p2[0], p2[1]]

#     # Determine dxn
#     x_diff = p2[0] - p1[0]
#     y_diff = p2[1] - p1[1]
#     x_dxn = 0 if not x_diff else int((x_diff) / abs(x_diff))
#     y_dxn = 0 if not y_diff else int((y_diff) / abs(y_diff))

#     while src[0] != tgt[0] or src[1] != tgt[1]:
#         # Step in x dxn
#         if abs(tgt[0] - src[0]) > 0:
#             src[0] += x_dxn
#         # Step in y dxn
#         if abs(tgt[1] - src[1]) > 0:
#             src[1] += y_dxn
#         # Fill
#         if map[src[0]][src[1]] != 'B' and map[src[0]][src[1]] != 'S':
#             map[src[0]][src[1]] = '#'

#     return map


def fill_row(p1: Iterable, p2: Iterable, map: List[List]) -> List[List]:
    x = p1[0]
    for y in range(p1[1], p2[1] + 1):
        if map[x][y] == '.':
            map[x][y] = '#'
    return map


def do_scan(s: tuple, b: tuple, dist: int, map: List[List]) -> List[List]:

    # Mark north, south, east, west relative to sensor position 
    p_north = [s[0] - dist, s[1]]
    p_south = [s[0] + dist, s[1]]
    p_east = [s[0], s[1] + dist]
    p_west = [s[0], s[1] - dist]
    map[p_north[0]][p_north[1]] = '#' if map[p_north[0]][p_north[1]] != 'B' and map[p_north[0]][p_north[1]] != 'S' else map[p_north[0]][p_north[1]]
    map[p_south[0]][p_south[1]] = '#' if map[p_south[0]][p_south[1]] != 'B' and map[p_south[0]][p_south[1]] != 'S' else map[p_south[0]][p_south[1]]
    map[p_east[0]][p_east[1]] = '#' if map[p_east[0]][p_east[1]] != 'B' and map[p_east[0]][p_east[1]] != 'S' else map[p_east[0]][p_east[1]]
    map[p_west[0]][p_west[1]] = '#' if map[p_west[0]][p_west[1]] != 'B' and map[p_west[0]][p_west[1]] != 'S' else map[p_west[0]][p_west[1]]

    # From north to south, fill in diamond east to west, as it's drawn
    p_left = [p_north[0], p_north[1]]
    p_right = [p_north[0], p_north[1]]
    while p_left != p_west and p_right != p_east:
        print(p_left, p_west, p_right, p_east)
        map = fill_row(p_left, p_right, map)

        # North to equator
        p_left[0] += 1
        p_right[0] += 1
        p_left[1] -= 1
        p_right[1] += 1

    while p_left != p_south and p_right != p_south:
        print(p_left, p_west, p_right, p_east)
        map = fill_row(p_left, p_right, map)

        # Equator to south
        p_left[0] += 1
        p_right[0] += 1
        p_left[1] += 1
        p_right[1] -= 1

    return map


def count_positions(map: List[List], x: int) -> int:
    return len(list(filter(lambda pos: pos == '#', map[x + HALF_SIZE])))


def print_map(map: List[List]) -> None:
    for l in map:
        s = ''
        for x in l:
            s += x
        print(s)


if __name__ == "__main__":
    with open("day15.txt", "r") as f:
        lines = f.readlines()
    map = []
    for i in range(MAP_SIZE):
        map.append(['.'] * MAP_SIZE)
    for line in lines:
        m = re.search(
            r"Sensor at x=(-?\d+), y=(-?\d+): closest beacon is at x=(-?\d+), y=(-?\d+)",
            line,
        )

        s = (int(m.group(2)) + HALF_SIZE, int(m.group(1)) + HALF_SIZE)
        b = (int(m.group(4)) + HALF_SIZE, int(m.group(3)) + HALF_SIZE)
        man_dist = calc_man_dist(s, b)
        map[s[0]][s[1]] = 'S'
        map[b[0]][b[1]] = 'B'
        map = do_scan(s, b, man_dist, map)
        #print_map(map)
        print(count_positions(map, 10))
