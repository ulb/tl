#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit 
#and install
NAUTYHOME = /Applications/pynauty-0.6.0/nauty26r7
HEADER_DIR = h

ifeq ($(use),clang)
	CXX = clang++
	CXXFLAGS = -std=c++11 -Wall -Wno-c++14-extensions -O3
else
	CXX = g++-7
	CXXFLAGS = -O3
endif

OBJS = $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)

2level-enum: 2level-enum.cpp $(OBJS) $(NAUTYHOME) $(HEADER_DIR)
	$(CXX) $(CXXFLAGS) -g 2level-enum.cpp -o 2level-enum $(OBJS) -I$(NAUTYHOME) -I$(HEADER_DIR)