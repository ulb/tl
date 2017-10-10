#replace with the absolute path to the folder /nauty26r7
#download from http://pallini.di.uniroma1.it/#howtogetit 
#and install
NAUTYHOME = /PATH/TO/nauty26r7 

OBJS = $(patsubst %.o,$(NAUTYHOME)/%.o,nautyL.o naurng.o nautil.o schreier.o naugraph.o naugroup.o)
prog: $(OBJS) $(NAUTYHOME)
	g++- -g 2level-enum.cpp -o 2level-enum $(OBJS) -I$(NAUTYHOME)