import os
import sys


def replace_temp_with_real(dir):
    # rename files to be deleted with -d suffix
    for subdir, dirs, files in os.walk(args.dir):
        for file in files:
            if ".c" in file and "temp" not in file:
                os.rename(file, file[:-2] + "-d.c")
    # delete files marked for deletion and rename temp fils
    for subdir, dirs, files in os.walk(args.dir):
        for file in files:
            if ".c" in file and "temp" not in file:
                os.rename(file, file[:-2] + "-d.c")


def delete_temp_files(dir):
    for subdir, dirs, files in os.walk(args.dir):
        for file in files:
            if "-temp.c" in file:
                os.system("rm {:}".format(subdir + file))


def insert_docstrings_new(old_file, new_file, func_inds, func_infos):
    old_file.seek(0)
    f_ind = 0
    # print func_infos
    for ind, line in enumerate(old_file):
        new_file.write(line)
        if len(func_inds) != f_ind:
            if ind == func_inds[f_ind] - 1:
                new_file.write("// Summary - \n")
                args = func_infos[f_ind][2]
                for arg in args:
                    arg_split = arg.split(" ")
                    if len(arg_split) == 2:
                        arg_type = arg_split[0]
                        arg_name = arg_split[1]
                    else:
                        continue  # no input arguments
                    if arg_name[0] == "*":
                        arg_type = arg_type + " *"
                        arg_name = arg_name[1:]
                    new_file.write("// param ({:}) {:} - \n".format(arg_type, arg_name))
                if func_infos[f_ind][0].lower().strip() != "void":
                    new_file.write("// retval ({:}) - \n".format(func_infos[f_ind][0]))
                f_ind += 1


def parse_function_string(func_str):
    # get retval type
    for ind, s in enumerate(func_str):
        if s != " ":
            continue
        else:
            retval = func_str[0:ind]
            break
    # get function name & arg string
    for ind, s in enumerate(func_str):
        # exception for ISR's (they don't declare return type)
        if func_str[0:3] == "ISR":
            name_start = 0
            retval = "void"
        if s == " ":
            name_start = ind + 1
        elif s == "(":
            func_name = func_str[name_start:ind]
            arg_start = ind + 1
        elif s == ")":
            arg_str = func_str[arg_start:ind]
    # get function arguments
    func_args = arg_str.split(", ")
    return (retval, func_name, func_args)


def get_function_strings(file, function_linenumbers):
    function_strings = []
    function_ind = 0
    file.seek(0)
    for ind, line in enumerate(file):
        if ind != function_linenumbers[function_ind]:
            continue
        else:
            function_strings.append(line[:-1])
            function_ind += 1
            if function_ind == len(function_linenumbers):
                return function_strings


def get_function_indices(file):
    bracket_count = 0
    in_function = False
    function_linenumbers = []
    file.seek(0)
    for ind, line in enumerate(file):
        # print line,
        # increment/decrement bracket count
        if "{" in line and line[0:2] != "//" and line[0:2] != "/*":
            bracket_count += 1
        if "}" in line and line[0:2] != "//" and line[0:2] != "/*":
            bracket_count -= 1
        # check if we're in a function or not
        if in_function is False:
            if bracket_count == 1:
                    if line[0] == "{":
                        function_linenumbers.append(ind - 1)
                        in_function = True
        else:
            # in function, check for exit
            if bracket_count == 0:
                # print "exit function"
                in_function = False
    return function_linenumbers


def main(filename):
    with open(filename, "r") as f:
        with open(filename[:-2] + "-temp.c", "w") as f_mod:
            function_linenumbers = get_function_indices(f)
            # print "function inds: {:}".format(function_linenumbers)
            if len(function_linenumbers) == 0:
                return
            function_strings = get_function_strings(f, function_linenumbers)
            func_str_parsed = []
            for f_str in function_strings:
                func_str_parsed.append(parse_function_string(f_str))
            insert_docstrings_new(f, f_mod, function_linenumbers, func_str_parsed)


if __name__ == '__main__':
    import argparse

    parser = argparse.ArgumentParser(description='Generates or updates C source file function docstrings.')
    parser.add_argument('--dir', '-d', metavar='dir', type=str, default=None,
                        help='Directory of source files')
    parser.add_argument('--file', '-f', dest='file', type=str, default=None,
                        help='Source file')
    parser.add_argument("--delete_temp", action="store_true",
                        help='delete temp files generated.')
    args = parser.parse_args()

    if args.delete_temp is True:
        if args.dir is not None:
            delete_temp_files(args.dir)
        elif args.file is not None:
            os.system("rm {:}".format(args.file[:-2] + "-temp.c"))
        else:
            print "delete needs a directory or file input!"

    if args.file is None and args.dir is None:
        print "Need either a directory of source files or a single source file to run!"
        exit()
    elif args.file is not None and args.dir is not None:
        print "Need either a directory of source files or a single source file, not both, to run!"
        exit()
    # run the script on a single file
    if args.file is not None:
        main(args.file)
        exit()
    if "~" in args.dir:
        user_path = os.path.expanduser("~")
        print args.dir
        print user_path
        args.dir = user_path + args.dir[2:]
        print args.dir
    # run the script on a directory of files
    for subdir, dirs, files in os.walk(args.dir):
        for file in files:
            if ".c" in file or ".C" in file:
                main(os.path.join(subdir, file))
