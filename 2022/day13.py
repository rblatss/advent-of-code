import functools
from typing import Union


def compare(p1: Union[int, list], p2: Union[int, list]) -> bool:
    # Compare items in lists
    for i1, i2 in zip(p1, p2):
        if isinstance(i1, list) and isinstance(i2, list):
            r = compare(i1, i2)
            if r != 0:  # recurse
                return r

        # If exactly one value is an integer, compare integer
        # to the first value in the list
        elif isinstance(i1, list) and not isinstance(i2, list):
            r = compare(i1, [i2])
            if r != 0:
                return r

        elif not isinstance(i1, list) and isinstance(i2, list):
            r = compare([i1], i2)
            if r != 0:
                return r

        # If both values are integers, the lower value should come first.
        # If the values are the same, continue checking <-- key!
        else:
            if i1 > i2:
                return 1
            elif i1 < i2:
                return -1

    # If the right list runs out of items first, the lists are not in the correct order
    if len(p1) > len(p2):
        return 1

    if len(p1) == len(p2):
        return 0

    return -1


if __name__ == "__main__":
    indices = []
    packets = []
    index = 1
    with open("day13.txt", "r") as f:
        while (p1 := f.readline()) and (p2 := f.readline()):
            p1 = eval(p1.strip('\n'))
            p2 = eval(p2.strip('\n'))

            # Part 2
            packets.append(p1)
            packets.append(p2)

            # Compare
            # if compare(p1, p2):
            #     indices.append(index)

            f.readline()  # skip empty line

            # index += 1

    # print(indices)
    # print(sum(indices))

    # Part 2 - add divider packets
    d1 = [[2]]
    d2 = [[6]]
    packets.append([[2]])
    packets.append([[6]])

    # Part 2
    product = 1
    for i, p in enumerate(sorted(packets, key=functools.cmp_to_key(compare)), 1):
        if p == d1 or p == d2:
            product *= i
    print(product)
