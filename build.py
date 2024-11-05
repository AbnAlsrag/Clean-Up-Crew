import os
import glob
import argparse
import pathlib

parser = argparse.ArgumentParser()
parser.add_argument("-m", "--mode", dest="mode", default="debug", help="Specifies the mode of compilation e.g.(debug, dev, release)")
args = parser.parse_args()

def bundle():
    pass

MODE = args.mode
OUTPUT = "CUC"

CC = "gcc"
LIBS = "-Lexternal/raylib/ -l:libraylibdll.a"
CFLAGS = "-std=c17 -Wall -Werror -pedantic -I src -I external/"
TARGET = "bin/" + MODE + "/" + OUTPUT + ".exe"
FILES = ""

tmp = glob.glob("src/**/*.c", recursive=True)

for file in tmp:
    FILES += file
    FILES += " "

DEBUG_FLAGS = "-g -O0 -DDEBUG_BUILD"
DEV_FLAGS = "-g -O2 -DDEV_BUILD"
RELEASE_FLAGS = "-03 -flto -DRELEASE_BUILD"

if MODE == "debug":
    CFLAGS = DEBUG_FLAGS + " " + CFLAGS
elif MODE == "dev":
    CFLAGS = DEV_FLAGS + " " + CFLAGS
    bundle()
elif MODE == "release":
    CFLAGS = RELEASE_FLAGS + " " + CFLAGS
    bundle()

pathlib.Path("bin/" + MODE).mkdir(parents=True, exist_ok=True)

COMMAND = CC + " -o " + TARGET.format(MODE) + " " + FILES + " " + CFLAGS + " " + LIBS

if __name__ == "__main__":
    print(COMMAND)
    os.system(COMMAND)