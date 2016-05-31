# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH = /home/chi777/Desktop/DSP/srilm-1.5.10
MACHINE_TYPE = i686-m64
LM = bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w -std=c++11
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	for i in $(shell seq 1 10) ; do \
		echo "Running $$i.txt"; \
		./mydisambig testdata/$$i.txt result2/$$i.txt; \
	done;
map: mapping
	echo "Mapping!"
	./mapping $(FROM) $(TO)
clean:
	$(RM) $(OBJ) $(TARGET) mapping ZhuYin-Big5.map
