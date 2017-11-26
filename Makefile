# Makefile for Mini RPG
# Created August 10, 2017

BUILD_DIR = obj
SRC_DIR = src
BIN_DIR = bin

CXX = g++ -std=c++14
CXXFLAGS = -Wall -O -g -MMD                          # use -MMD to generate dependencies
GTKFLAGS = `pkg-config --cflags --libs`
SFMLFLAGS = -lsfml-system -lsfml-window -lsfml-graphics -lsfml-network -lsfml-audio -ljsoncpp

# compile all .cc files in source directory & it's subdirectories
SOURCES = $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/*/*.cc)

# .o files depend upon .cc files with same names
OBJECTS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(SOURCES:.cc=.o))

# .d file is list of dependencies for corresponding .cc file
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
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CXX)  -c -o $@ $< $(CXXFLAGS) $(GTKFLAGS)

### Clean target
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

########################################### Testing

# Google test docs: https://github.com/google/googletest/blob/master/googletest/README.md
# User example makefile: https://github.com/tjun/googletest-circleci/blob/master/make/Makefile

TEST_DIR = test
GTEST_DIR = googletest/googletest

TEST_SRCS = $(TEST_DIR)/sample.cc
TEST_OBJS = $(patsubst $(TEST_DIR)/%, $(BUILD_DIR)/%, $(TEST_SRCS:.cc=.o))
TEST_TARGET = $(BIN_DIR)/gtest

# Google Test headers & required flags.  Usually you shouldn't change this definition.
CPPFLAGS += -isystem $(GTEST_DIR)/include
CXXFLAGS += -Wextra -pthread
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h $(GTEST_DIR)/include/gtest/internal/*.h
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

# For simplicity and to avoid depending on Google Test's implementation details, the
# dependencies specified below are conservative and not optimized.  This is fine as
# Google Test compiles fast and for ordinary users its source rarely changes.
$(BUILD_DIR)/gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ $(GTEST_DIR)/src/gtest-all.cc

$(BUILD_DIR)/gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c -o $@ $(GTEST_DIR)/src/gtest_main.cc

$(BIN_DIR)/gtest.a : $(BUILD_DIR)/gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

$(BIN_DIR)/gtest_main.a : $(BUILD_DIR)/gtest-all.o $(BUILD_DIR)/gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

### Test target
.PHONY: test
test: $(TEST_TARGET)

### Link
# Note: A test should link with either gtest.a or gtest_main.a, depending on whether it
# defines its own main() function
$(TEST_TARGET): $(TARGET) $(TEST_OBJS) $(BIN_DIR)/gtest_main.a
	@[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)
	$(CXX) $(LDFLAGS) -o $@ $(TEST_OBJS) $(BIN_DIR)/gtest_main.a $(LIBS) -lpthread

### Compile
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cc $(GTEST_HEADERS)
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCS) -o $@ -c $<