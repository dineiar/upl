CC = gcc
CPP = g++ -std=c++1y
FLAGS = -Wall -O3 
INCLUDE = ../include/upl


all: 
	$(CC) $(FLAGS) -I $(INCLUDE) -L ../lib libtester.c -o cctester -lupl -lm 
	$(CPP) $(FLAGS) -I $(INCLUDE) -L ../lib libtester.cpp -o cpptester -lupl -lm 
clean:
	rm -rf cpptester cctester

