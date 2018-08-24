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
