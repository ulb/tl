#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit 
#and install
NAUTYHOME = /Applications/pynauty-0.6.0/nauty26r7

OBJS = $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)
2level-enum: 2level-enum.cpp $(OBJS) $(NAUTYHOME)
	g++-7 -g 2level-enum.cpp -o 2level-enum -O3 $(OBJS) -I$(NAUTYHOME)