all:
	g++ -Wall -Wextra -pedantic -g -std=c++11 -lsfml-system -lsfml-window -lsfml-graphics main.cpp slime.cpp ball.cpp
