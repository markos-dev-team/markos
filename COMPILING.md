# Compiling

## Details of package contents

* include - MarkOs library interface
* src - MarkOs library source (task scheduler)
* examples - Small programs based on uITRON or MarkOs native API

## Build from sources

You will need:

* Linux distribution, software has been tested on Ubuntu Linux 18.10.
* Make program (GNU Make 3.81 or later).
* C compiler (such as gcc, or clang or arm-none-eabi-gcc)

Run from the root of the source directory:

```
$ make compile
$ make run
```

This will build a simulator using native platform, and run examples with the simulator.

## Make targets

* `compile` will compile libraries and examples.
* `run` will run examples (uses qemu to execute ARM examples)
* ` clean` removes already built binaries.

## Make additional parameters

* `DEBUG=1` enable debug build (for debugging via gdb)
* `DEBUGF=1` enable debugging prinouts for tracing software execution.
* `NDEBUG=1` disable asserts (required for minimum footprint.) 
* `TARGET=native` build for Linux (simulator).
* `TARGET=lm3s6965evb-semihosting` compile for Stellaris LM3S6965 platform.
* Standard make variables, such as `CC`, `CFLAGS`, `LDFLAGS`, `LDLIBS` can be used to set or override build options as needed.

Note: details of targets are found in `target/`*name*/`flags.mk`. The existing targets can be used as templates for further targets.
When switching between targets, it may be neccessary to `make clean` in between. 