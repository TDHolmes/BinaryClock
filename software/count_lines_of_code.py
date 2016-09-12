import os


def main(directory):
    """
    Simple function that goes through a directory (including sub-directories) and counts
    the number of lines in .c files, .h file, and .py files and prints it out.

    :param str directory: root directory for the function to start at.
    """
    c_lines = 0
    h_lines = 0
    py_lines = 0
    for subdir, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".c"):
                with open(subdir + "/" + file, "r") as f:
                    for line in f:
                        c_lines += 1
            elif file.endswith(".h"):
                with open(subdir + "/" + file, "r") as f:
                    for line in f:
                        h_lines += 1
            elif file.endswith(".py"):
                with open(subdir + "/" + file, "r") as f:
                    for line in f:
                        py_lines += 1
    print "\n{:} contains {:} lines of code\n({:} in c files, {:} in h files, {:} in py files)\n".format(
        directory, c_lines + h_lines + py_lines, c_lines, h_lines, py_lines)


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
