# markos <a href="https://scan.coverity.com/projects/mnippula-markos"><img alt="Coverity Scan Build Status" src="https://scan.coverity.com/projects/17998/badge.svg" align="right"></a> <a href="https://travis-ci.com/markos-dev-team/markos"><img alt="Travis CI status" src="https://travis-ci.com/markos-dev-team/markos.svg?branch=master" align="right"></a> 
MarkOs minimal realtime operating system

The project is a minimal kernel for embedded environments (mainly bare metal environments). 

C Compilers for bare metal such as *arm-none-eabi-gcc* tend to come with full or partial support for ISO C99 hosted environment. This means you can readily use services such as:
- copy data within memory
- work with c strings
- possibly: read and parse input 
- produce console or terminal output

This set of services allows creation of very simple programs. For more complex programs, it is often desirable to process multiple tasks at the same time. The purpose of **MarkOs** to provide simple multitasking, while avoiding significant growth for the memory footprint requirements.

# Goals

The MarkOs goals include:
- Easy portability (minimal amount of platform specific code)
- Small footprint (full functionality takes less than 1.5 kB)
- Standards compliance & compatibility: (&micro;ITRON 4.0 minimum requirements compatible)
- Readability (simple to read C code)
- Minimal dependencies (uses only a small subset of standard C libraries)

# Supported targets

Current distribution supports target environments as follows:
- Linux (86-64) native target (simulation of RTOS environment).
- arm-none-eabi-gcc compiling code for Stellaris LM3S6965 (with semihosting)
     - This target can be supported via qemu if target hardware is not available.

Easy porting to other ARMv7-M/R/A SoCs.

# Getting Started

See `COMPILING.md` for building instructions.

# License

The MarkOs is available under GPL license version 2 or (at your option) later version, such as 3.

# References 

*&micro;ITRON 4.0*

&micro;ITRON 4.0 Specification, http://www.ertl.jp/ITRON/SPEC/FILE/mitron-400e.pdf
