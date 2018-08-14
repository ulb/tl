.PHONY: default all runnables profilables debugables clean

#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit
#and install

NAUTYHOME := $(HOME)/.opt/nauty

CXX := clang++
CXXFLAGS := -std=c++14 -Wall -W 

HEADER_DIR := h

#OBJS := $(NAUTYHOME)/nauty.a
OBJS := $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)
HEADER_FILES := $(shell find $(HEADER_DIR) | grep '.hpp$$')

SOURCES := $(shell find src -type f)
RUNNABLES := $(patsubst src/%.cpp,run/%,$(SOURCES))
PROFILABLES := $(patsubst src/%.cpp,prof/%,$(SOURCES))
DEBUGABLES := $(patsubst src/%.cpp,debug/%,$(SOURCES))

default: runnables
all: runnables profilables debugables
runnables: run $(RUNNABLES)
profilables: prof $(PROFILABLES)
debugables: debug $(DEBUGABLES)

run/%: src/%.cpp $(OBJS) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -O3 $< -o $@ $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)

prof/%: src/%.cpp $(OBJS) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -O3 -no-pie -pg $< -o $@ $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)

debug/%: src/%.cpp $(OBJS) $(HEADER_FILES)
	$(CXX) $(CXXFLAGS) -g $< -o $@ $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)

run:
	mkdir -p run

prof:
	mkdir -p prof

debug:
	mkdir -p debug

clean:
	rm -rf run prof debug
