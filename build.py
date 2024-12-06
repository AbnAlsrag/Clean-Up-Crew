import os
import glob
import argparse
import pathlib
import subprocess

PROJECT_NAME = "Clean Up Crew"

parser = argparse.ArgumentParser(prog=PROJECT_NAME)
parser.add_argument("-m", "--mode", dest="mode", default="debug", choices=["debug", "dev", "release"], help="Specifies the mode of compilation e.g.(debug, dev, release)")
parser.add_argument("-t", "--tools", dest="is_tools", action="store_true", help="")
parser.add_argument("-a", "--all", dest="is_all", action="store_true", help="")
parser.add_argument("-r", "--run", dest="run", action="store_true", help="")
parser.add_argument("--platform", dest="platform", default="pc", choices=["pc"], help="")
args = parser.parse_args()

def bundle():
    pass

MODE = args.mode
IS_TOOLS = args.is_tools
IS_ALL = args.is_all
PLATFORM = args.platform
RUN = args.run
OUTPUT = "CUC"
CC = "gcc"
LIBS = "-Lexternal/raylib/ -l:libraylibdll.a -lm"

RUN_PROJECT = False
RUN_TOOLS = False

def get_platform_src():
    files = []
    if PLATFORM == "pc":
        files = glob.glob("src/platform/pc/**/.c")
    else:
        print("unkown platform expected either (pc)")
        exit(1)

    return files

def build_project():
    CFLAGS = "-std=c99 -Wall -Werror -pedantic -static -I src -I external/"
    TARGET = "bin/" + MODE + "/" + OUTPUT + ".exe"
    FILES = ""

    tmp = glob.glob("src/**/*.c", recursive=True)

    for file in tmp:
        FILES += file
        FILES += " "
    
    for file in get_platform_src():
        FILES += file
        FILES += " "

    DEBUG_FLAGS = "-g -O0 -DDEBUG_BUILD"
    DEV_FLAGS = "-g -O2 -DDEV_BUILD"
    RELEASE_FLAGS = "-O3 -flto -DRELEASE_BUILD"

    if MODE == "debug":
        CFLAGS = DEBUG_FLAGS + " " + CFLAGS
    elif MODE == "dev":
        CFLAGS = DEV_FLAGS + " " + CFLAGS
    elif MODE == "release":
        CFLAGS = RELEASE_FLAGS + " " + CFLAGS

    pathlib.Path("bin/" + MODE).mkdir(parents=True, exist_ok=True)

    COMMAND = CC + " -o " + TARGET + " " + FILES + " " + CFLAGS + " " + LIBS

    print(COMMAND)
    os.system(COMMAND)

    if RUN_PROJECT:
        print(TARGET)
        subprocess.call(TARGET)

def build_tools():
    CFLAGS = "-static -I external/"
    TARGET = "tools/bin/" + MODE + "/"
    TOOLS = [
        ["editor.exe", "tools/editor/src/*.c"]
    ]

    DEBUG_FLAGS = "-g -O0 -DDEBUG_BUILD"
    DEV_FLAGS = "-g -O2 -DDEV_BUILD"
    RELEASE_FLAGS = "-O3 -flto -DRELEASE_BUILD"

    if MODE == "debug":
        CFLAGS = DEBUG_FLAGS + " " + CFLAGS
    elif MODE == "dev":
        CFLAGS = DEV_FLAGS + " " + CFLAGS
    elif MODE == "release":
        CFLAGS = RELEASE_FLAGS + " " + CFLAGS

    pathlib.Path("tools/bin/" + MODE).mkdir(parents=True, exist_ok=True)

    for tool in TOOLS:
        COMMAND = CC + " -o " + TARGET + tool[0] + " " + tool[1] + " " + CFLAGS + " " + LIBS
        print(COMMAND)
        os.system(COMMAND)

        if RUN_TOOLS:
            print(TARGET)
            subprocess.call(TARGET + tool[0])


if __name__ == "__main__":
    if RUN:
        RUN_PROJECT = True
        RUN_TOOLS = True
        
    if IS_TOOLS:
        build_tools()
    elif IS_ALL:
        
        build_project()
        build_tools()
    else:
        build_project()