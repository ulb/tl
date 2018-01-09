#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit
#and install

NAUTYHOME := $(HOME)/.opt/nauty

CXX := clang++
CXXFLAGS := -std=c++14 -Wall -W -O3

HEADER_DIR = h

OBJS = $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)
HEADER_FILES = $(shell find $(HEADER_DIR) | grep '.hpp$$')

RUNNABLES = $(patsubst %,run/%,inflate dedup sift count_polar)

all: prepare $(RUNNABLES)

run/%: src/%.cpp $(OBJS) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -g $< -o $@ $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)

prepare: run

run:
	mkdir -p run

clean:
	rm -rf run
