# Define the compiler
CXX = clang++

# Define the source files
SOURCES = ./game.cpp ./AudioPlayer.cpp ./sprites.cpp ./utils.cpp ./foe.cpp ./player.cpp ./particles.cpp

# Define the object files
OBJECTS = $(SOURCES:.cpp=.ll)

# Define the flags
CXXFLAGS = -O3 -ffast-math -march=native -flto -g

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
	$(CXX) -O3 $(OBJECTS) $(LIBS) -o $(TARGET)

# Pattern rule for building object files
%.ll: %.cpp
	$(CXX) $(FLAGS) -S -emit-llvm -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean run
