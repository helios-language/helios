#!/usr/bin/python3
import multiprocessing as mp
from multiprocessing.pool import ThreadPool
import subprocess as sp
import sys
import os

os.path.sep = "/"

ARGS = sys.argv
LIBDIR = "lib"
LIBS = []
BUILDDIR = "build"
BINDIR = "bin"
SRCDIR = "src"
INCLUDEDIR = "include"
EXECUTABLE = "helios"
CXX = "gcc"
CXXFLAGS = ["-Wall", "-g"]
LD = "gcc"
LDFLAGS = []
EXCLUDEDLIBS = []

files = [os.path.join(dp, f) for dp, dn, fn in os.walk(
    os.path.expanduser(SRCDIR)) for f in fn]


cppfiles = list(filter(lambda x: x.endswith(".cpp"), files))
cfiles = list(filter(lambda x: x.endswith(".c"), files))

if not os.path.exists(BUILDDIR):
    os.mkdir(BUILDDIR)

if not os.path.exists(BINDIR):
    os.mkdir(BINDIR)

if not os.path.exists(LIBDIR):
    os.mkdir(LIBDIR)


headerdirs = [
    os.path.join(o)
    for o in (i[0] for i in os.walk(SRCDIR))
    if os.path.isdir(os.path.join(o))
    if os.path.basename(o) == INCLUDEDIR
]

headervar = [
    "-I./{}".format(i)
    for i in headerdirs
]

libs = [
    f.replace(".a", "").replace("lib", "")
    for dp, dn, fn in os.walk(os.path.expanduser(LIBDIR))
    for f in fn
    if f.startswith("lib") and f.endswith(".a") and f not in EXCLUDEDLIBS
]

libvar = [
    "-l{}".format(i)
    for i in reversed(libs)
]

libvar = LIBS + libvar


objects = []


def compilecfile(filename):
    obj = os.path.basename(filename).replace(".c", ".o")
    print("compiling {} to {}".format(filename, obj))
    try:
        # print(" ".join([
        #     CXX,
        #     "-c",

        #     *CXXFLAGS,
        #     *headervar,
        #     "-o",
        #     BUILDDIR+"/"+obj,
        #     filename
        # ]))
        sp.check_output([
            CXX,
            "-c",

            *CXXFLAGS,
            *headervar,
            "-o",
            BUILDDIR+"/"+obj,
            filename
        ], stderr=sp.STDOUT, shell=True, timeout=3,
            universal_newlines=True)

    except Exception as e:
        print("Error: ", e.returncode, e.output)
        os._exit(-1)

    objects.append(BUILDDIR+"/"+obj)
    return 0


pool = ThreadPool(processes=mp.cpu_count())
pool.map(compilecfile, cfiles)

try:
    print("linking...")
    # print(" ".join([
    #     LD,
    #     *LDFLAGS,
    #     *objects,
    #     "-o",
    #     BINDIR+"/"+EXECUTABLE,

    #     "-L./"+LIBDIR,
    #     *libvar

    # ]))
    sp.check_output([
        LD,
        *LDFLAGS,
        *objects,
        "-o",
        BINDIR+"/"+EXECUTABLE,

        "-L./"+LIBDIR,
        *libvar

    ], stderr=sp.PIPE)

except Exception as e:
    print("Error: ", e)
    os._exit(-1)

print("compilation finished. executable can be found in {}".format(
    os.path.join(BINDIR, EXECUTABLE)))
print("executing:\n-------")

try:
    sp.call([
        BINDIR+"/"+EXECUTABLE,
        *ARGS
    ])
except Exception as e:
    print("Error: ", e)
    os._exit(-1)
