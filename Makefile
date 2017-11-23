# Makefile for Mini RPG
# Created August 10, 2017

BUILD_DIR = obj
SRC_DIR = src
BIN_DIR = bin

CXX = g++ -std=c++14
CXXFLAGS = -Wall -O -g -MMD                          # use -MMD to generate dependencies
GTKFLAGS = `pkg-config --cflags --libs` -std=c++14
LIBFLAGS = -framework OpenGL
SFMLFLAGS = -lsfml-system -lsfml-window -lsfml-graphics

# compile all .cpp files in source directory & it's subdirectories
SOURCES = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*/*.cpp)

# .o files depend upon .cpp files with same names
OBJECTS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cpp=.o))

# .d file is list of dependencies for corresponding .cpp file
DEPENDS = ${OBJECTS:.o=.d}

TARGET = $(BIN_DIR)/game

### Set default make
.PHONY: default
default: $(TARGET)

### Pull in dependency info for (existing) .o files
-include ${DEPENDS}

### Link
$(TARGET): $(OBJECTS)
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET) $(GTKFLAGS) $(LIBFLAGS) $(SFMLFLAGS)

### Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CXX)  -c -o $@ $< $(CXXFLAGS) $(GTKFLAGS)

### Clean target
.PHONY: clean
clean:
	rm -f $(OBJECTS) $(TEST_OBJS) $(DEPENDS)
