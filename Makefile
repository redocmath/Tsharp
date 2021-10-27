ifeq ($(OS), Windows_NT)
	exec = tsharp.exe
else
	exec = tsharp.out
endif

sources = $(wildcard src/*.c)
objects = $(sources:.c=.o)
flags = -g

$(exec): $(objects)
	gcc $(objects) $(flags) -o $(exec)

%.o: %.c include/%.h
	gcc -c $(flags) $< -o $@

install:
	make
ifeq ($(OS), Windows_NT)
	cp ./tsharp.exe /usr/local/bin/t#
else
	echo "Not Available in Windows"
endif

clean:
ifeq ($(OS), Windows_NT)
	-rm *.exe
else
	-rm *.out
endif
	-rm *.o
	-rm src/*.o
