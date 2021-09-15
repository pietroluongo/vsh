# OBJS	= ./build/fileSorter.o ./build/fileMerger.o ./build/tokenizer.o ./build/heap.o ./build/main.o

OBJS = ./build/main.o ./build/vsh.o ./build/utils.o

# SOURCE	= client/main.c src/fileSorter.c src/fileSorter.c src/tokenizer.c src/heap.c

SOURCE = client/main.c src/vsh.c src/utils.c

# HEADER	=
# OUT	= trab2
OUT = vsh
CC	 = gcc
# FLAGS	 = -O3 -c -DNDEBUG
FLAGS = -g -c -std=gnu11
LFLAGS	 = -lm 

all: | mkBuildDir $(OBJS)
	$(CC) $(OBJS) -o $(OUT) $(LFLAGS)

# build/fileSorter.o: src/fileSorter.c
# 	$(CC) $(FLAGS) src/fileSorter.c -o ./build/fileSorter.o -std=c11

# build/fileMerger.o: src/fileMerger.c
#	$(CC) $(FLAGS) src/fileMerger.c -o ./build/fileMerger.o -std=c11

#build/tokenizer.o: src/tokenizer.c
#	$(CC) $(FLAGS) src/tokenizer.c -o ./build/tokenizer.o -std=c11

#build/heap.o: src/heap.c
#	$(CC) $(FLAGS) src/heap.c -o ./build/heap.o -std=c11

build/main.o: client/main.c
	$(CC) $(FLAGS) client/main.c -o ./build/main.o

build/vsh.o: src/vsh.c
	$(CC) $(FLAGS) src/vsh.c -o ./build/vsh.o

build/utils.o: src/utils.c
	$(CC) $(FLAGS) src/utils.c -o ./build/utils.o

clean:
	rm -rf build/

mkBuildDir:
	@if [ ! -d "./build/" ]; then \
		mkdir build; \
	fi

run:
	./vsh