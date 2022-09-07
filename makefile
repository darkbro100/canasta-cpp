CC=g++
CFLAGS=-std=c++11 -Wextra -Wall -I.
 
main: main.cpp
	$(CC) -o main main.cpp card.cpp deck.cpp $(CFLAGS)
