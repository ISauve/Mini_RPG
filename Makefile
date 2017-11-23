# Makefile for Mini-RPG
# August 10, 2017

CXX = g++ -std=c++14
CXXFLAGS = -Wall -O -g -MMD                          # use -MMD to generate dependencies
GTKFLAGS = `pkg-config --cflags --libs` -std=c++14
LIBFLAGS = -framework OpenGL
SFMLFLAGS = -lsfml-system -lsfml-window -lsfml-graphics
SOURCES = $(wildcard *.cpp) $(wildcard */*.cpp)      # compile all .cpp files in curr directory & it's subdirectories
OBJECTS = ${SOURCES:.cpp=.o}                         # .o files depend upon .cpp files with same names
DEPENDS = ${OBJECTS:.o=.d}                           # .d file is list of dependencies for corresponding .cpp file
EXEC=game

$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC)  $(GTKFLAGS) $(LIBFLAGS) $(SFMLFLAGS)

%.o: %.cc 
	$(CXX) -c -o $@ $< $(CXXFLAGS) $(GTKFLAGS)

-include ${DEPENDS}

# Include clean target
.PHONY: clean
clean:
	rm  -f $(OBJECTS) $(DEPENDS)

