CC = g++
CFLAGS = -std=c++17
DEPS = graph.hpp node.hpp graphics.hpp
LIBS = -lSDL2 -lSDL2_gfx
OBJ = main.o graph.o node.o graphics.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hparg: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o