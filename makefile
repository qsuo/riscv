CFLAGS:= -Wall -std=c++17
CC:=g++

%.o: %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

memory: memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $<

riscv: riscv.o memory.o hart.o log_service.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^

decoder: decoder.o riscv.o memory.o hart.o log_service.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^

simulator: decoder.o riscv.o hart.o memory.o log_service.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^

test: memory riscv decoder simulator

clean:
	rm -rf *.o *.exe
