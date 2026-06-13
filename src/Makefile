all : flash

PREFIX ?= riscv32-unknown-elf
CC:=$(PREFIX)-gcc
GDB:=$(PREFIX)-gdb
CXX:=$(PREFIX)-g++
LD:=$(PREFIX)-ld
OBJCOPY:=$(PREFIX)-objcopy
OBJDUMP:=$(PREFIX)-objdump


TARGET := main
BUILD_DIR := build
TARGET_MCU?=CH32V003
include ../ch32fun/ch32fun/ch32fun.mk
CFLAGS +=

flash : cv_flash
clean : cv_clean
