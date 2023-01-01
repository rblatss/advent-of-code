def find_integer(l: list) -> int:
    for x in l:
        if isinstance(x, list):
            val = find_integer(x)
            if val:
                return val
        else:
            return x
    return None


def compare(p1: list, p2: list) -> bool:
    # If the right list runs out of items first, the lists are not in the correct order
    if len(p1) > len(p2):
        return False

    # Compare items in lists
    for i1, i2 in zip(p1, p2):
        if isinstance(i1, list) and isinstance(i2, list):
            if not compare(i1, i2):  # recurse
                return False

        # If exactly one value is an integer, compare integer
        # to the first value in the list
        elif isinstance(i1, list) and not isinstance(i2, list):
            i1 = find_integer(i1)
            if i1 and i1 > i2:
                return False
        elif not isinstance(i1, list) and isinstance(i2, list):
            i2 = find_integer(i2)
            if not i2 or i1 > i2:
                return False

        # If both values are integers, the lower value should come first
        else:
            if i1 > i2:
                return False

    return True


if __name__ == "__main__":
    indices = []
    index = 1
    with open("day13.txt", "r") as f:
        while (p1 := f.readline()) and (p2 := f.readline()):
            p1 = eval(p1.strip('\n'))
            p2 = eval(p2.strip('\n'))

            # Compare
            if compare(p1, p2):
                indices.append(index)

            f.readline()  # skip empty line

            index += 1

    print(indices)
    print(sum(indices))
