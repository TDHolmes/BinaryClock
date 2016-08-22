import os


def main(directory):
    c_lines = 0
    h_lines = 0
    for subdir, dirs, files in os.walk(directory):
        for file in files:
            if ".c" in file:
                with open(subdir + file, "r") as f:
                    for line in f:
                        c_lines += 1
            elif ".h" in file:
                with open(subdir + file, "r") as f:
                    for line in f:
                        h_lines += 1
    print "{:} contains {:} lines of code ({:} in c files, {:} in h files)".format(
        directory, c_lines + h_lines, c_lines, h_lines)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Generates or updates C source file function docstrings.')
    parser.add_argument('dir', metavar='dir', type=str, default=None,
                        help='Directory of source files')
    args = parser.parse_args()

    if "~" in args.dir:
        user_path = os.path.expanduser("~")
        args.dir = user_path + args.dir[2:]
    # run the script
    main(args.dir)
