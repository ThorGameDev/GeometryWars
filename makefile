# Define the compiler
CXX = clang++

# Define the source files
SOURCES = ./game.cpp ./AudioPlayer.cpp ./sprites.cpp ./utils.cpp ./foe.cpp

# Define the object files
OBJECTS = $(SOURCES:.cpp=.o)

# Define the flags
CXXFLAGS = -O3

# SDL flags
#`pkg-config --libs --cflags sdl3`
SDL_FLAGS = `pkg-config --cflags sdl3 sdl3-image`
SDL_LIBS = `pkg-config --libs sdl3 sdl3-image`

# Combine all flags and libraries
FLAGS = $(CXXFLAGS) $(SDL_FLAGS) $(SDL_IMAGE_FLAGS)
LIBS = $(SDL_LIBS) $(SDL_IMG)

# Define the target executable
TARGET = Game

# Default rule
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBS) -o $(TARGET)

# Pattern rule for building object files
%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean run
