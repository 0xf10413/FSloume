CXX=g++
CXXFLAGS=-Wall -Wextra -pedantic -g -std=c++11
LDFLAGS=-lsfml-system -lsfml-window -lsfml-graphics
EXEC=fsloume




all: fsloume

fsloume: slime.o ball.o main.o
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< 

clean:
	rm -rfv *.o
