# This is just a simple Makefile to make compilation less of a hassle
# and should be improved later

CC	=	g++
EXE	=	Snake
SRC	=	./*.cpp
STD	=	-std=c++11
LIB	=	-lSDL2

default: clean snake

snake:
	$(CC) -o $(EXE) $(SRC) $(STD) $(LIB)

clean:
	rm -f $(EXE)
