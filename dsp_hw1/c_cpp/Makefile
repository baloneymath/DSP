###############
CFLAGS = -g -Wall
CFLAGS = -O2
###############
LDFLAGS+=-lm     # link to math library

CC=gcc
CXX=g++
CXXFLAGS=-O2 -std=c++11

TARGET=train test acc

.PHONY: all clean

all: $(TARGET)
# type make/make all to compile test_hmm

train: train.cpp
	$(CXX) train.cpp $(CXXFLAGS) -o train

test: test.cpp
	$(CXX) test.cpp $(CXXFLAGS) -o test
acc: acc.cpp
	$(CXX) acc.cpp $(CXXFLAGS) -o acc

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
