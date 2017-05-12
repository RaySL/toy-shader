CC = g++ -c
LD = g++
#LD = ld

CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lGL -lstdc++

OBJS = shadertoy.o sdlGlApp.o test.o
MAIN = test

$(MAIN): $(OBJS)
	$(LD) -o $(MAIN) $^ $(LDFLAGS)

%.o: %.cpp %.h
	$(CC) -o $@ $< $(CFLAGS)
