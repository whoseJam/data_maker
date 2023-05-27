import os
import sys
import re

cnt = 0

def check_and_open_header(module: str):
    path_to_header = "{}/{}.h".format(path_to_dtstructure_header, module)
    if os.path.exists(path_to_header):
        return open(path_to_header, "r")
    path_to_header = "{}/{}.h".format(path_to_abstructure_header, module)
    if os.path.exists(path_to_header):
        return open(path_to_header, "r")
    path_to_header = "{}/{}.h".format(path_to_utility_header, module)
    if os.path.exists(path_to_header):
        return open(path_to_header, "r")
    return None

def check_and_open_src(module: str):
    path_to_src = "{}/{}.cpp".format(path_to_dtstructure_src, module)
    if os.path.exists(path_to_src):
        return open(path_to_src, "r")
    path_to_src = "{}/{}.cpp".format(path_to_abstructure_src, module)
    if os.path.exists(path_to_src):
        return open(path_to_src, "r")
    path_to_src = "{}/{}.cpp".format(path_to_utility_src, module)
    if os.path.exists(path_to_src):
        return open(path_to_src, "r")
    return None

def find_all_modules(file, is_header: bool, name = "None"):
    line: str = file.readline()
    while line:
        if line.startswith("#") and line.__contains__("include"):
            header = re.search('"(.*?).h"', line)
            if header is None:
                line = file.readline()
                continue
            module = header.group(1)
            if module in modules:
                line = file.readline()
                continue
            modules.append(module)
            
            if is_header:
                header = check_and_open_header(module)
                source = check_and_open_src(module)
                if header is not None:
                    find_all_modules(header, True, module)
                if source is not None:
                    find_all_modules(source, False, module)
            else:
                if name == module:
                    line = file.readline()
                    continue
                header = check_and_open_header(module)
                source = check_and_open_src(module)
                if header is not None:
                    find_all_modules(header, True, module)
                if source is not None:
                    find_all_modules(source, False, module)
        line = file.readline()

def parse_header(file, output_file):
    line: str = file.readline()
    ifcnt: int = 0
    while line:
        if line.startswith("#") and line.__contains__("include"):
            header = re.search('"(.*?).h"', line)
            if header is not None:
                line = file.readline()
                continue
        elif line.startswith("#ifndef"):
            ifcnt = ifcnt + 1
            if ifcnt == 1:
                line = file.readline()
                line = file.readline()
                continue
        elif line.startswith("#endif"):
            ifcnt = ifcnt - 1
            if ifcnt == 0:
                line = file.readline()
                continue
        output_file.write(line)
        line = file.readline()
    output_file.write("\n")

def parse_source(file, output_file):
    line: str = file.readline()
    while line:
        if line.startswith("#") and line.__contains__("include"):
            header = re.search('"(.*?).h"', line)
            if header is not None:
                line = file.readline()
                continue
        output_file.write(line)
        line = file.readline()
    output_file.write("\n")


def parse_file(file, output_file):
    for i in reversed(range(len(modules))):
        module = modules[i]
        header = check_and_open_header(module)
        if header is not None:
            parse_header(header, output_file)
        
    line: str = file.readline()
    while line:
        if line.startswith("#") and line.__contains__("include"):
            header = re.search('"(.*?).h"', line)
            if header is not None:
                line = file.readline()
                continue
        output_file.write(line)
        line = file.readline()
    output_file.write("\n")

    for i in range(len(modules)):
        module = modules[i]
        source = check_and_open_src(module)
        if source is not None:
            parse_source(source, output_file)


if __name__=="__main__":
    args = sys.argv
    modules = []
    # path_to_datamaker = args[1]
    # path_to_source_code = args[2]
    path_to_datamaker = "/home/hsj/data"
    path_to_source_code = "/home/hsj/data/example/test/test.cpp"
    path_to_dtstructure_header = "{}/include/DtStructure".format(path_to_datamaker)
    path_to_abstructure_header = "{}/include/AbStructure".format(path_to_datamaker)
    path_to_utility_header = "{}/include/Utility".format(path_to_datamaker)
    path_to_dtstructure_src = "{}/src/DtStructure".format(path_to_datamaker)
    path_to_abstructure_src = "{}/src/AbStructure".format(path_to_datamaker)
    path_to_utility_src = "{}/src/Utility".format(path_to_datamaker)

    output_file = open("./output.cpp", "w")
    find_all_modules(open(path_to_source_code, "r"), False)
    parse_file(open(path_to_source_code, "r"), output_file)
