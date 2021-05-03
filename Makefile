# Master Makefile dependencies
TARGET := fpga_program
INCLUDE_LIB := true
GCC_OPTS += -O2

include ./Makefile.gcc.in
