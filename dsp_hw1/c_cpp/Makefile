###########3###
CFLAGS = -g -Wall
CFLAGS = -O2
###############
LDFLAGS+=-lm     # link to math library

CC=gcc
CXX=g++
CXXFLAGS=-O2

TARGET=train test

.PHONY: all clean

all: $(TARGET)
# type make/make all to compile test_hmm

train: train.cpp
	$(CXX) train.cpp $(CXXFLAGS) -o train

test:

clean: 
	$(RM) $(TARGET)   # type make clean to remove the compiled file
