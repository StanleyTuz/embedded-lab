
# [-128, 127]
def make_sawtooth():
    y = list(range(-128, 128, 1))

    s = "int8_t fullSawtooth[256] = {\n"
    for row in range(32):
        s += ', '.join([str(t) for t in y[8*row:8*row + 8]])
        if row != 31:
            s += ','
        s += '\n'

    s += '};'

    with open("sawtooth.h", 'w') as fout:
        fout.write(s)


def make_square():
    y = [-128]*128 + [127]*128;

    s = "int8_t fullSquare[256] = {\n"
    for row in range(32):
        s += ', '.join([str(t) for t in y[8*row:8*row + 8]])
        if row != 31:
            s += ','
        s += '\n'

    s += '};'

    with open("square.h", 'w') as fout:
        fout.write(s)
    





if __name__ == '__main__':

    make_square()