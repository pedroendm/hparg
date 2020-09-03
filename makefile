OBJS = main.cpp graphics.cpp graph.cpp node.cpp

CC = g++

COMPILER_FLAGS = -std=c++17

LINKER_FLAGS = -lSDL2 -lSDL2_gfx

OBJ_NAME = hparg

all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)