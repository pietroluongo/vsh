# Componentes do Grupo: Gabriel Pietroluongo e Otávio Sales
OBJS = ./build/main.o ./build/vsh.o ./build/utils.o ./build/command.o

SOURCE = client/main.c src/vsh.c src/utils.c src/command.c

OUT = vsh
CC	 = gcc
FLAGS = -c -DNDEBUG -std=gnu11
LFLAGS	 = -lm 

all: | mkBuildDir $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)

build/main.o: client/main.c
	$(CC) $(FLAGS) client/main.c -o ./build/main.o

build/vsh.o: src/vsh.c
	$(CC) $(FLAGS) src/vsh.c -o ./build/vsh.o

build/utils.o: src/utils.c
	$(CC) $(FLAGS) src/utils.c -o ./build/utils.o

build/command.o: src/command.c
	$(CC) $(FLAGS) src/command.c -o ./build/command.o

clean:
	rm -rf build/

mkBuildDir:
	@if [ ! -d "./build/" ]; then \
		mkdir build; \
	fi

run:
	./vsh
