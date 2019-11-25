CFLAGS:= -Wall
CC:=g++

%.o: %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

memory: memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $<

riscv: riscv.o memory.o hart.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^

decoder: decoder.o riscv.o memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^

test: memory riscv decoder

clean:
	rm -rf *.o
