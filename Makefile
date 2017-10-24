#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit
#and install

HEADER_DIR = h

NAUTYHOME = /Applications/pynauty-0.6.0/nauty26r7
ifeq ($(dude),omar)
	NAUTYHOME = $(HOME)/.opt/nauty
endif

ifeq ($(use),gcc)
	CXX = g++
	CXXFLAGS = -O3
else
	CXX = clang++
	CXXFLAGS = -std=c++14 -Wall -W -O3
endif

OBJS = $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)
HEADER_FILES = $(shell find $(HEADER_DIR) | grep '.hpp$$')

RUNNABLES = $(patsubst %,run/%,inflate dedup sift)

all: prepare $(RUNNABLES)

run/%: src/%.cpp $(OBJS) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -g $< -o $@ $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)

prepare: run

run:
	mkdir -p run

clean:
	rm -rf run
