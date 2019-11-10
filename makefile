CFLAGS:= -Wall 
CC:=g++

%.o: %.cpp
	$(CC) $(CFLAGS)  -o $@ -c $<

memory: memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $<

simulator: simulator.o memory.o
	$(CC) $(CFLAGS) -o test/$@ test/$@.cpp $^


test: memory simulator

clean:
	rm -rf *.o
